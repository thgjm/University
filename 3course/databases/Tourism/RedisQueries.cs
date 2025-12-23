using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using StackExchange.Redis;

namespace Tourism.Redis
{
    public static class RedisQueries
    {
        private const string RedisConnectionString = "localhost:6379";
        private static ConnectionMultiplexer _redis;
        private static IDatabase _db;

        // ініціалізація підключення до Redis
        private static void InitializeRedis()
        {
            try
            {
                if (_redis == null || !_redis.IsConnected)
                {
                    var config = ConfigurationOptions.Parse(RedisConnectionString);
                    config.ConnectTimeout = 5000;
                    _redis = ConnectionMultiplexer.Connect(config);
                }
                _db = _redis.GetDatabase();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"\n[ПОМИЛКА REDIS] Не вдалося підключитися до Redis на {RedisConnectionString}. Переконайтеся, що сервер запущено.");
                Console.WriteLine($"Деталі: {ex.Message}");
                _db = null;
            }
        }

        public static async Task Run()
        {
            Console.WriteLine("Запити в Redis:");

            InitializeRedis();

            if (_db == null)
            {
                Console.WriteLine("Помилка підключення до Redis.");
                return;
            }

            // кешування деталей туру
            await CacheTourDetails();

            // кешування списку доступних дат для туру
            await CacheScheduleDates();

            // кешування кошика користувача
            await UserShoppingCartCache();

            // рейтинг популярності турів
            await TourPopularityRanking();

            // сповіщення оновлення ціни в реальному часі
            await RealTimePriceUpdatePubSub();

            // керування бронюванням
            await InventoryManagementBooking();
        }

        // для реалізації певних запитів будемо користуватися не даними з БД SSMS, а задаваними локально
        private static async Task CacheTourDetails()
        {
            string tourId = "TOUR:T123-PARIS-WEEKEND"; //кеш-ключ - айді туру, який користувач часто переглядає\шукає
            Console.WriteLine($"\nКешування деталей туру.");

            // hash для зберігання даних
            await _db.HashSetAsync(tourId, new HashEntry[]
            {
                new HashEntry("Title", "Париж: Романтичні вихідні"),
                new HashEntry("BasePrice", "500.00"),
                new HashEntry("DurationDays", "3"),
                new HashEntry("LastUpdated", DateTime.UtcNow.ToString())
            });
            // термін життя ключа - нехай 1 година
            await _db.KeyExpireAsync(tourId, TimeSpan.FromSeconds(3600));
            Console.WriteLine($"   -> Записано деталі туру ID {tourId} з TTL 3600с.");

            // читання
            var tourDetails = await _db.HashGetAllAsync(tourId);
            Console.WriteLine($"   -> Прочитано з кешу:");
            foreach (var entry in tourDetails)
                Console.WriteLine($"       - {entry.Name}: {entry.Value}");

            // перевірка ціни
            string price = await _db.HashGetAsync(tourId, "BasePrice");
            Console.WriteLine($"   -> Перевірка ціни: {price}");
        }

        private static async Task CacheScheduleDates()
        {
            string scheduleKey = "SCHEDULES:TOUR-456"; // айді туру для якого кешуєм доступні дати
            Console.WriteLine($"\nКешування доступних дат.");

            await _db.KeyDeleteAsync(scheduleKey);

            // list для доступних дат
            await _db.ListRightPushAsync(scheduleKey, new RedisValue[]
            {
                "2026-03-01",
                "2026-04-08",
                "2026-05-15"
            });
            await _db.ListRightPushAsync(scheduleKey, "2026-05-22");
            Console.WriteLine($"   -> Додано 4 доступні дати для туру {scheduleKey}.");

            // читання
            var availableDates = await _db.ListRangeAsync(scheduleKey, 0, -1);
            Console.WriteLine($"   -> Доступні дати: {string.Join(", ", availableDates)}");

            // перевірка першої наступної дати
            string firstDate = await _db.ListGetByIndexAsync(scheduleKey, 0);
            Console.WriteLine($"   -> Наступна доступна дата: {firstDate}");
        }

        private static async Task UserShoppingCartCache()
        {
            string userId = "USER:AB45C6";
            string cartKey = $"CART:{userId}";
            Console.WriteLine($"\nКешування кошика користувача.");

            // додавання товарів до кошика
            await _db.HashSetAsync(cartKey, new HashEntry[]
            {
                new HashEntry("TOUR-500", "qty:2|date:2026-07-10"),
                new HashEntry("HOTEL-99", "qty:1|nights:3"),
                new HashEntry("INSURANCE", "qty:2|type:basic")
            });
            // термін життя ключа (TTL) - 1 година
            await _db.KeyExpireAsync(cartKey, TimeSpan.FromHours(1));
            Console.WriteLine($"   -> Кошик користувача {userId} збережено з 3 товарами.");

            // оновлення кількості 1 товару
            await _db.HashSetAsync(cartKey, "TOUR-500", "qty:3|date:2026-07-10");
            Console.WriteLine($"   -> Оновлено кількість для TOUR-500 до 3.");

            // читання
            var cartItems = await _db.HashGetAllAsync(cartKey);

            Console.WriteLine($"   -> Поточний вміст кошика:");
            foreach (var item in cartItems)
                Console.WriteLine($"       - {item.Name}: {item.Value}");

            // видалення товару
            await _db.HashDeleteAsync(cartKey, "INSURANCE-ADDON");
            Console.WriteLine($"   -> Видалено INSURANCE з кошика.");

            // перевірка кількості
            long totalItems = await _db.HashLengthAsync(cartKey);
            Console.WriteLine($"   -> Залишок товарів у кошику: {totalItems}");
        }

        private static async Task TourPopularityRanking()
        {
            string rankingKey = "TOUR:POPULARITY_RANKING";
            Console.WriteLine($"\nРейтинг популярності турів.");

            // очищення ключа
            await _db.KeyDeleteAsync(rankingKey);

            // тур Київ щойно був переглянутий 3 рази
            await _db.SortedSetIncrementAsync(rankingKey, "TOUR:1000-KYIV", 3);
            // тур Рим щойно був переглянутий 1 раз
            await _db.SortedSetIncrementAsync(rankingKey, "TOUR:5000-ROME", 1);
            // тур Париж був переглянутий 5 разів
            await _db.SortedSetIncrementAsync(rankingKey, "TOUR:3000-PARIS", 5);
            // +1 перегляд до туру Рім
            await _db.SortedSetIncrementAsync(rankingKey, "TOUR:5000-ROME", 2);

            // отримання топ-3 турів
            var topTours = await _db.SortedSetRangeByRankWithScoresAsync(rankingKey, 0, 2, Order.Descending);

            Console.WriteLine($"   -> ТОП-3 найпопулярніших тури:");
            int rank = 1;
            foreach (var entry in topTours)
            {
                Console.WriteLine($"       {rank}. {entry.Element} (Переглядів: {entry.Score})");
                rank++;
            }
        }

        private static async Task RealTimePriceUpdatePubSub()
        {
            string channelName = "TOUR_UPDATES:PRICE"; // канал
            Console.WriteLine($"\nСповіщення про ціну.");

            // підписник
            var sub = _redis.GetSubscriber();

            string receivedMessage = null;

            // підписка на канал
            await sub.SubscribeAsync(channelName, (channel, message) =>
            {
                // коли прийде повідомлення
                receivedMessage = message;
                Console.WriteLine($"   [SUBSCRIBER] Отримано оновлення: {message}");
            });

            Console.WriteLine($"   -> Підписались на канал '{channelName}'!");

            string updateMessage = "TOUR:T123-PARIS-WEEKEND | New Price: 480.00 EUR";

            // публікація повідомлення
            long subscribersCount = await sub.PublishAsync(channelName, updateMessage);
            Console.WriteLine($"   -> [PUBLISHER] Опубліковано повідомлення. Отримали підписників: {subscribersCount}.");

            await Task.Delay(100);

            // відписка
            await sub.UnsubscribeAsync(channelName);
            Console.WriteLine("   -> Відписались від каналу.");
        }

        private static async Task InventoryManagementBooking()
        {
            string inventoryKey = "INVENTORY:TOUR-777:2026-06-15";
            int initialCapacity = 20;
            int seatsToBook = 3;
            int seatsToCancel = 1;
            Console.WriteLine($"\nКерування бронюванням.");

            await _db.StringSetAsync(inventoryKey, initialCapacity);
            Console.WriteLine($"   -> Ініціалізовано доступність: {initialCapacity} місць.");

            // бронювання
            long remainingSeats = await _db.StringIncrementAsync(inventoryKey, -seatsToBook);

            Console.WriteLine($"   -> Бронювання {seatsToBook} місць. Залишок: {remainingSeats}.");

            if (remainingSeats < 0)
            {
                // місць не вистачило
                Console.WriteLine($"   -> Бронювання неможливе. Відбувається скасування...");
                await _db.StringIncrementAsync(inventoryKey, seatsToBook);
                long rolledBackSeats = (long)await _db.StringGetAsync(inventoryKey);
                Console.WriteLine($"   -> Фактичний залишок: {rolledBackSeats}.");
            }
            else
                Console.WriteLine($"   -> Бронювання успішне.");

            // скасування бронювання
            long afterCancellation = await _db.StringIncrementAsync(inventoryKey, seatsToCancel);
            Console.WriteLine($"   -> Скасовано {seatsToCancel} місце. Залишок: {afterCancellation}.");
        }
    }
}
using System;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Collections.Generic;
using Microsoft.Data.SqlClient;
using Tourism.Repositories;
using Tourism.Interfaces;
using Tourism.Models;
using Tourism.Services;
using Tourism.Redis;

namespace Tourism
{
    public class Program
    {
        //private const string SqlConnectionString = "Server=DESKTOP-EMI94R6;Database=Tourism;Trusted_Connection=True;TrustServerCertificate=True;";
        private const string SqlConnectionString = "Server=.;Database=Tourism;Trusted_Connection=True;TrustServerCertificate=True;";
        private const string MongoConnectionString = "mongodb://localhost:27017/TourismLogs";
        private const string MongoDatabaseName = "TourismLogs";

        public static async Task Main(string[] args)
        {
            Console.OutputEncoding = System.Text.Encoding.UTF8;
            Console.WriteLine($"Підключення до SQL: {SqlConnectionString}");
            Console.WriteLine($"Підключення до MongoDB: {MongoConnectionString}");

            // виконання запитів Redis
            await RedisQueries.Run();

            // ініціалізація логерів
            var sqlLogger = new SqlLogRepository(SqlConnectionString);
            var mongoLogger = new MongoLogRepository(MongoConnectionString, MongoDatabaseName);

            // перевірка швидкості логерів
            //await RunPerformanceTestAsync(mongoLogger, sqlLogger);

            //створення нового бронювання та запис логу
            //await RunDemoAsync(SqlConnectionString, mongoLogger, sqlLogger);

            Console.WriteLine("Натисніть будь-яку клавішу для завершення...");
            Console.ReadKey();
        }

        // створення бронювання
        static async Task RunDemoAsync(string sqlConnString, MongoLogRepository mongoLogger, SqlLogRepository sqlLogger)
        {
            ReadBookings(sqlConnString, "Перед створенням нового бронювання:");

            Guid realScheduleId = Guid.Parse("45bb091e-d037-4504-a89c-312af1c8aa1a"); // айді розкладу туру з БД
            string travelerEmail = "traveler4@gmail.com"; // існуючий користувач
            Guid newBookingId = Guid.Empty;
            decimal calculatedPrice = 0m;

            using (var uow = new Repositories.UnitOfWork(sqlConnString))
            {
                try
                {
                    // створення бронювання в SQL
                    newBookingId = uow.Bookings.Create(travelerEmail, realScheduleId, null);
                    Console.WriteLine($"1. Бронювання ID: {newBookingId} створено у SQL (Pending).");

                    calculatedPrice = uow.Bookings.GetPriceForBooking(newBookingId);
                    Console.WriteLine($"   -> Ціна розрахована SQL: {calculatedPrice.ToString("N2")} (Отримано для логування).");

                    // дані для логування:
                    var logDetails = new
                    {
                        BookingId = newBookingId,
                        TravelerEmail = travelerEmail,
                        TourScheduleId = realScheduleId,
                        PriceCalculated = calculatedPrice,
                        Status = "PendingConfirmation",
                        Environment = "ConsoleApp"
                    };

                    // логування в MongoDB
                    await mongoLogger.LogAsync("BookingCreated_Pending", travelerEmail, logDetails);
                    Console.WriteLine("4. Лог успішно записано в Mongo.");

                    // логування в SQL JSON
                    sqlLogger.Log("BookingCreated_Pending", travelerEmail, logDetails);
                    Console.WriteLine("5. Лог записано в SqlAuditLog.");

                    // підтвердження бронювання
                    Console.Write("\nБажаєте відразу підтвердити бронювання? (Y/N): ");
                    string input = Console.ReadLine()?.ToUpper();

                    if (input == "Y" && newBookingId != Guid.Empty)
                    {
                        uow.Bookings.Confirm(newBookingId);
                        Console.WriteLine("   -> Бронювання підтверджено у SQL.");

                        // логування зміни статусу
                        await mongoLogger.LogAsync("BookingStatusChanged", travelerEmail, new { BookingId = newBookingId, NewStatus = "Confirmed" });
                        Console.WriteLine("   -> Лог зміни статусу відправлено в Mongo.");
                    }

                    // коміт в БД
                    uow.Commit();
                    Console.WriteLine("   -> Успішно! SQL-Транзакція (Commit) завершена.");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"\nПомилка! Відкат змін: {ex.Message}");
                    var errorDetails = new { Message = ex.Message, SQLConnection = sqlConnString };
                    // логування помилка в MongoDB
                    await mongoLogger.LogAsync("BookingError", travelerEmail, errorDetails);
                    Console.WriteLine("   -> Лог помилки відправлено в Mongo.");
                    uow.Rollback();
                    Console.WriteLine("   -> SQL-Транзакція (Rollback) завершена.");
                }
            }

            ReadBookings(sqlConnString, "\nПісля спроби створення нового бронювання:");
        }

        static void ReadBookings(string connectionString, string message)
        {
            Console.WriteLine($"\n{message}");
            try
            {
                using (var uowRead = new Repositories.UnitOfWork(connectionString)) // 💡 UnitOfWork замість MockUnitOfWork
                {
                    var bookings = uowRead.Bookings.GetAllBookings();
                    int count = 0;
                    foreach (var b in bookings)
                    {
                        count++;
                        Console.WriteLine($"   [{count}] {b.TourTitle} | Клієнт: {b.TravelerName} | {b.TotalPrice.ToString("N2")}");
                    }
                    if (count == 0)
                        Console.WriteLine("   (Список порожній.)");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Помилка читання: {ex.Message}");
            }
        }

        // тести швидкості логів
        public static async Task RunPerformanceTestAsync(MongoLogRepository mongoLogger, SqlLogRepository sqlLogger)
        {
            const int InsertCount = 1000;
            const int MongoParallelism = 50;
            var sw = new Stopwatch();

            Console.WriteLine("\nТест швидкості:");

            // дані для вставки
            var testDetails = new { EventId = Guid.NewGuid(), Timestamp = DateTime.UtcNow, Source = "PerfTest", Message = "Polyglot test record." };

            // NoSQL
            var mongoTasks = new List<Task>();
            sw.Start();
            for (int i = 0; i < InsertCount; i++)
            {
                mongoTasks.Add(mongoLogger.LogAsync("Test_Insert", $"test_{i}@mongo.com", testDetails));

                if (mongoTasks.Count >= MongoParallelism)
                {
                    await Task.WhenAll(mongoTasks);
                    mongoTasks.Clear();
                }
            }
            await Task.WhenAll(mongoTasks);
            sw.Stop();
            Console.WriteLine($"[NoSQL - MongoDB]       {InsertCount} вставок: {sw.ElapsedMilliseconds} мс.");

            double MongoTime = sw.ElapsedMilliseconds;

            // SQL - JSON log
            sw.Restart();
            for (int i = 0; i < InsertCount; i++)
                sqlLogger.Log("Test_Insert", $"test_{i}@sqljson.com", testDetails);
            sw.Stop();
            Console.WriteLine($"[SQL - JSON Log]        {InsertCount} вставок:    {sw.ElapsedMilliseconds} мс.");

            double SQLJSONTime = sw.ElapsedMilliseconds;

            // SQL - normalized log
            sw.Restart();
            for (int i = 0; i < InsertCount; i++)
                sqlLogger.LogNormalized("Test_Insert", $"test_{i}@sqlnorm.com", testDetails);
            sw.Stop();
            Console.WriteLine($"[SQL - Normalized]      {InsertCount} транзакцій: {sw.ElapsedMilliseconds} мс.");

            double SQLNormTime = sw.ElapsedMilliseconds;

            Console.WriteLine("Очікуваний результат: NoSQL < SQL JSON < SQL Normalized ");
            var results = new List<(string Name, double Time)>
            {
                ("NoSQL", MongoTime),
                ("SQL JSON", SQLJSONTime),
                ("SQL Normalized", SQLNormTime)
            };

            var sorted = results.OrderBy(r => r.Time).ToList();

            Console.WriteLine("Реальний результат: ");
            foreach (var r in sorted)
                Console.Write($"{r.Name} < ");
            Console.WriteLine();
        }
    }
}
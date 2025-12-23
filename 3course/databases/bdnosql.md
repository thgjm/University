Create table Currency (

&nbsp;   code char(3) primary key, --UAH, USD

&nbsp;   symbol nvarchar(10)  not null,

&nbsp;   exchange\_rate\_to\_usd decimal(10, 4)

);



Create table Country (

&nbsp;   iso\_code char(2) primary key, --UA, US

&nbsp;   name varchar(50) not null,

&nbsp;   currency\_code char(3) not null foreign key references Currency(code),

&nbsp;   region varchar(50),

);



Create table Language (

&nbsp;   code char(2) primary key, --ua, en

&nbsp;   name varchar(50) not null,

&nbsp;   native\_name nvarchar(50)

);



Create table City (

&nbsp;   city\_slug varchar(50) primary key, --kyiv-ua, rome-it

&nbsp;   name varchar(100) not null,

&nbsp;   country\_code char(2) not null foreign key references Country(iso\_code),

&nbsp;   timezone varchar(50)

);



Create table Category (

&nbsp;   slug varchar(50) primary key,

&nbsp;   display\_name varchar(100) not null

);



Create table Traveler (

&nbsp; email varchar(50) primary key,

&nbsp; first\_name varchar(50) not null,

&nbsp; last\_name varchar(50) not null,

&nbsp; age int not null,

&nbsp; phone varchar(15),

&nbsp; nationality\_code char(2) foreign key references country(iso\_code),

&nbsp; is\_deleted bit default 0,

&nbsp; created\_at datetime2 default getdate()

)



Create table TravelAgency (

&nbsp; tax\_id varchar(50) primary key,

&nbsp; company\_name varchar(100) not null,

&nbsp; website varchar(100),

&nbsp; contact\_email varchar(50) not null,

&nbsp; is\_verified bit,

&nbsp; rating decimal(2, 1)

)



Create table TourGuide (

&nbsp;   license\_number varchar(50) primary key,

&nbsp;   email varchar(100) not null unique,

&nbsp;   full\_name varchar(100) not null,

&nbsp;   hourly\_rate decimal(10, 2),

&nbsp;   currency\_code char(3) not null foreign key references Currency(code)

);



Create table GuideLanguages (

&nbsp;   guide\_license varchar(50) not null foreign key references TourGuide(license\_number),

&nbsp;   language\_code char(2) not null foreign key references Language(code),

&nbsp;   primary key (guide\_license, language\_code)

);



Create table Attraction (

&nbsp;   id uniqueidentifier default newid() primary key,

&nbsp;   name varchar(150) not null,

&nbsp;   city\_slug varchar(50) not null foreign key references City(city\_slug),

&nbsp;   category\_slug varchar(50) foreign key references Category(slug),

&nbsp;   description varchar(max),

&nbsp;   coordinates varchar(50)

);



Create table Hotel (

&nbsp;   id uniqueidentifier default newid() primary key,

&nbsp;   name varchar(150) not null,

&nbsp;   city\_slug varchar(50) not null foreign key references City(city\_slug),

&nbsp;   star\_rating int,

&nbsp;   address varchar(250),

&nbsp;   contact\_phone varchar(15),

&nbsp;   is\_deleted bit default 0

);



Create table RoomType (

&nbsp;   id uniqueidentifier default newid() primary key,

&nbsp;   hotel\_id uniqueidentifier not null foreign key references Hotel(id),

&nbsp;   name varchar(100) not null, -- Standard, Deluxe

&nbsp;   price\_per\_night decimal(10, 2) not null,

&nbsp;   capacity int,

&nbsp;   total\_rooms int

);



Create table TourPackage (

&nbsp;   id uniqueidentifier default newid() primary key,

&nbsp;   agency\_tax\_id varchar(50) not null foreign key references TravelAgency(tax\_id),

&nbsp;   title varchar(200) not null,

&nbsp;   description nvarchar(max),

&nbsp;   duration\_days int,

&nbsp;   base\_price decimal(10, 2) not null,

&nbsp;   currency\_code char(3) not null foreign key references Currency(code),

&nbsp;   updated\_at datetime2 default getdate(),

&nbsp;   updated\_by\_email varchar(50), -- Посилання на контактний email агенції

&nbsp;   is\_deleted bit default 0

)



create table TourSchedule (

&nbsp;   id uniqueidentifier default newid() primary key,

&nbsp;   tour\_id uniqueidentifier not null foreign key references TourPackage(id),

&nbsp;   start\_date date not null,

&nbsp;   end\_date date not null,

&nbsp;   guide\_license varchar(50) foreign key references TourGuide(license\_number),

&nbsp;   max\_capacity int,

&nbsp;   current\_bookings int default 0

);



Create table Booking (

&nbsp;   id uniqueidentifier default newid() primary key,

&nbsp;   traveler\_email varchar(50) not null foreign key references Traveler(email),

&nbsp;   tour\_schedule\_id uniqueidentifier foreign key references TourSchedule(id),

&nbsp;   room\_type\_id uniqueidentifier foreign key references RoomType(id),

&nbsp;   status varchar(50) not null,

&nbsp;   total\_price decimal(10, 2) not null,

&nbsp;   booking\_date datetime2 default getdate(),

&nbsp;   last\_modified\_at datetime2 default getdate()

);



Create table Payment (

&nbsp;   transaction\_id varchar(100) primary key,

&nbsp;   booking\_id uniqueidentifier not null foreign key references Booking(id),

&nbsp;   amount DECIMAL(10, 2) not null,

&nbsp;   currency\_code char(3) not null foreign key references Currency(code),

&nbsp;   payment\_method varchar(50),

&nbsp;   status varchar(50),

&nbsp;   payment\_date datetime2 default getdate()

);



Create table Review (

&nbsp;   id uniqueidentifier default newid() primary key,

&nbsp;   traveler\_email varchar(50) not null foreign key references Traveler(email),

&nbsp;   target\_id uniqueidentifier not null, -- ID туру або готелю

&nbsp;   target\_type varchar(10) not null, -- TOUR або HOTEL

&nbsp;   rating int,

&nbsp;   comment nvarchar(max),

&nbsp;   created\_at datetime2 default getdate(),

);



Create table SupportTicket (

&nbsp;   ticket\_id varchar(50) primary key,

&nbsp;   traveler\_email varchar(50) not null foreign key references Traveler(email),

&nbsp;   subject varchar(200) not null,

&nbsp;   status varchar(50),

&nbsp;   created\_at datetime2 default getdate(),

&nbsp;   closed\_at datetime2

);







##### &nbsp;				=====CONSTRAINTS====

Alter table TourSchedule

add constraint TourSchedule\_MaxCapacity

check (max\_capacity>=1),

constraint TourSchedule\_DateOrder

check (start\_date <= end\_date),

constraint TourSchedule\_BookingCapacity

check (current\_bookings <= max\_capacity)



Alter table SupportTicket

add constraint SupportTicket\_DateOrder

check (closed\_at is null or created\_at <= closed\_at)





Alter table Booking

add constraint Booking\_Status

check (status in ('Pending', 'Confirmed', 'Cancelled', 'Completed'))



Alter table Review

add constraint Review\_Rating

check (rating between 1 and 5)



alter table Payment

add constraint Payment\_Status check (status in ('Paid', 'Pending', 'Failed', 'Refunded'));







##### &nbsp;				=====TRIGERS====









-- Тригер для TourPackage: оновлення updated\_at при будь-якій зміні

CREATE TRIGGER trg\_TourPackage\_UpdateAudit

ON TourPackage

AFTER UPDATE

AS

BEGIN

&nbsp;   -- Оновлюємо updated\_at тільки якщо дані були змінені

&nbsp;   IF UPDATE(title) OR UPDATE(description) OR UPDATE(base\_price) 

&nbsp;   BEGIN

&nbsp;       UPDATE tp

&nbsp;       SET tp.updated\_at = GETDATE()

&nbsp;       FROM TourPackage tp

&nbsp;       INNER JOIN inserted i ON tp.id = i.id;

&nbsp;   END

END;

GO



-- Тригер для Booking: оновлення last\_modified\_at при зміні статусу

CREATE TRIGGER trg\_Booking\_UpdateAudit

ON Booking

AFTER UPDATE

AS

BEGIN

&nbsp;   -- Оновлюємо last\_modified\_at тільки якщо змінився статус або інші важливі поля

&nbsp;   IF UPDATE(status) OR UPDATE(total\_price)

&nbsp;   BEGIN

&nbsp;       UPDATE b

&nbsp;       SET b.last\_modified\_at = GETDATE()

&nbsp;       FROM Booking b

&nbsp;       INNER JOIN inserted i ON b.id = i.id;

&nbsp;   END

END;

GO



-- Тригер 1: Збільшення current\_bookings при створенні нового підтвердженого бронювання

CREATE TRIGGER trg\_Booking\_IncrementCapacity

ON Booking

AFTER INSERT

AS

BEGIN

&nbsp;   -- Перевіряємо, чи стосується бронювання туру (не готелю) і чи статус CONFIRMED

&nbsp;   IF EXISTS (SELECT 1 FROM inserted WHERE tour\_schedule\_id IS NOT NULL AND status = 'Confirmed')

&nbsp;   BEGIN

&nbsp;       UPDATE ts

&nbsp;       SET current\_bookings = ts.current\_bookings + 1

&nbsp;       FROM TourSchedule ts

&nbsp;       INNER JOIN inserted i ON ts.id = i.tour\_schedule\_id;

&nbsp;   END

END;

GO



-- Тригер 2: Зменшення current\_bookings при скасуванні бронювання

CREATE TRIGGER trg\_Booking\_DecrementCapacity

ON Booking

AFTER UPDATE

AS

BEGIN

&nbsp;   -- Перевіряємо, чи відбулася зміна статусу

&nbsp;   IF UPDATE(status)

&nbsp;   BEGIN

&nbsp;       -- Знаходимо бронювання, які були 'Confirmed' і стали 'Cancelled'

&nbsp;       UPDATE ts

&nbsp;       SET current\_bookings = ts.current\_bookings - 1

&nbsp;       FROM TourSchedule ts

&nbsp;       INNER JOIN inserted i ON ts.id = i.tour\_schedule\_id

&nbsp;       INNER JOIN deleted d ON i.id = d.id -- Використовуємо deleted для перевірки старого значення

&nbsp;       WHERE i.tour\_schedule\_id IS NOT NULL -- Перевіряємо, що це тур

&nbsp;         AND i.status = 'Cancelled' -- Новий статус - скасовано

&nbsp;         AND d.status <> 'Cancelled'; -- Старий статус НЕ був скасований (тобто це реальне скасування)



&nbsp;       -- Забезпечуємо, щоб current\_bookings не опустився нижче нуля (хоча це гарантується логікою)

&nbsp;       UPDATE ts

&nbsp;       SET current\_bookings = 0

&nbsp;       FROM TourSchedule ts

&nbsp;       WHERE current\_bookings < 0;

&nbsp;   END

END;

GO



CREATE TRIGGER trg\_Traveler\_SoftDelete

ON Traveler

INSTEAD OF DELETE

AS

BEGIN

&nbsp;   SET NOCOUNT ON;

&nbsp;   UPDATE t

&nbsp;   SET is\_deleted = 1

&nbsp;   FROM Traveler t

&nbsp;   INNER JOIN deleted d ON t.email = d.email;

&nbsp;   RAISERROR('Traveler was soft-deleted.', 10, 1) WITH NOWAIT;

END;

GO



CREATE TRIGGER trg\_TourPackage\_SoftDelete

ON TourPackage

INSTEAD OF DELETE

AS

BEGIN

&nbsp;   SET NOCOUNT ON;

&nbsp;   UPDATE tp

&nbsp;   SET is\_deleted = 1,

&nbsp;       updated\_at = GETDATE()

&nbsp;   FROM TourPackage tp

&nbsp;   INNER JOIN deleted d ON tp.id = d.id;

END;

GO



CREATE TRIGGER trg\_Hotel\_SoftDelete

ON Hotel

INSTEAD OF DELETE

AS

BEGIN

&nbsp;   SET NOCOUNT ON;

&nbsp;   UPDATE h

&nbsp;   SET is\_deleted = 1

&nbsp;   FROM Hotel h

&nbsp;   INNER JOIN deleted d ON h.id = d.id;

END;

GO



CREATE TRIGGER trg\_TourPackage\_CascadeSoftDelete

ON TourPackage

AFTER UPDATE

AS

BEGIN

&nbsp;   SET NOCOUNT ON;



&nbsp;   -- Перевіряємо, чи була змінена колонка is\_deleted і чи новий статус = 1

&nbsp;   IF UPDATE(is\_deleted) AND EXISTS (SELECT \* FROM inserted i JOIN deleted d ON i.id = d.id WHERE i.is\_deleted = 1 AND d.is\_deleted = 0)

&nbsp;   BEGIN

&nbsp;       -- М'яко видаляємо всі пов'язані розклади туру

&nbsp;       UPDATE ts

&nbsp;       SET is\_deleted = 1

&nbsp;       FROM TourSchedule ts

&nbsp;       INNER JOIN inserted i ON ts.tour\_id = i.id

&nbsp;       -- Ми оновлюємо лише ті розклади, які належать щойно "видаленому" пакету

&nbsp;       WHERE i.is\_deleted = 1;

&nbsp;   END

END;

GO



##### &nbsp;					=====INDEXES====

NON-CLUSTERED (для швидкого пошуку):

1\)

CREATE NONCLUSTERED INDEX IX\_Booking\_TravelerEmail 

ON Booking (traveler\_email);

GO

2\)

CREATE NONCLUSTERED INDEX IX\_Booking\_TourScheduleId

ON Booking (tour\_schedule\_id);

GO

3\)

CREATE NONCLUSTERED INDEX IX\_TourSchedule\_TourId

ON TourSchedule (tour\_id);

GO

4\)

CREATE NONCLUSTERED INDEX IX\_TourPackage\_AgencyTaxId

ON TourPackage (agency\_tax\_id);

GO

5\)

CREATE NONCLUSTERED INDEX IX\_Review\_TravelerEmail

ON Review (traveler\_email);

GO



UNIQUE:

1\)

CREATE UNIQUE NONCLUSTERED INDEX UQ\_TravelAgency\_ContactEmail 

ON TravelAgency (contact\_email);

GO



COMPOSITE:

1. Індекс для пошуку розкладу туру (Availability Search)

CREATE NONCLUSTERED INDEX IX\_TourSchedule\_TourDate 

ON TourSchedule (tour\_id, start\_date);

GO



2\) Індекс для історії бронювань мандрівника

CREATE NONCLUSTERED INDEX IX\_Booking\_TravelerStatus 

ON Booking (traveler\_email, status);

GO



3\) Індекс для пошуку типів номерів у готелі

CREATE NONCLUSTERED INDEX IX\_RoomType\_HotelName 

ON RoomType (hotel\_id, name);

GO



4\)

CREATE NONCLUSTERED INDEX IX\_Attraction\_CityCategory 

ON Attraction (city\_slug, category\_slug);

GO



##### &nbsp;				=====STORED PROCEDURES====



1. Реалізує "м'яке видалення" мандрівника, яке блокується тригером trg\_Traveler\_SoftDelete при використанні команди DELETE. 

CREATE PROCEDURE usp\_SoftDeleteTraveler

&nbsp;   @TravelerEmail NVARCHAR(100)

AS

BEGIN

&nbsp;   SET NOCOUNT ON;

&nbsp;   

&nbsp;   -- Оновлюємо прапор is\_deleted на 1 (м'яке видалення)

&nbsp;   UPDATE Traveler

&nbsp;   SET is\_deleted = 1

&nbsp;   WHERE email = @TravelerEmail;

&nbsp;   

&nbsp;   IF @@ROWCOUNT = 0

&nbsp;       THROW 50001, 'Traveler with this email was not found.', 1;

END;

GO



2\. Створює нове бронювання і фіксує його як Pending. Ця процедура запускає тригери аудиту.



CREATE PROCEDURE usp\_CreateNewBooking

&nbsp;   @TravelerEmail NVARCHAR(100),

&nbsp;   @ScheduleID UNIQUEIDENTIFIER,

&nbsp;   @RoomTypeID UNIQUEIDENTIFIER = NULL,

&nbsp;   @TotalPrice DECIMAL(10, 2)

AS

BEGIN

&nbsp;   SET NOCOUNT ON;

&nbsp;   

&nbsp;   -- Перевірка чи існує мандрівник

&nbsp;   IF NOT EXISTS (SELECT 1 FROM Traveler WHERE email = @TravelerEmail AND is\_deleted = 0)

&nbsp;   BEGIN

&nbsp;       THROW 50002, 'Traveler not found or is deleted.', 1;

&nbsp;       RETURN;

&nbsp;   END



&nbsp;   INSERT INTO Booking (traveler\_email, tour\_schedule\_id, room\_type\_id, status, total\_price, booking\_date)

&nbsp;   VALUES (@TravelerEmail, @ScheduleID, @RoomTypeID, 'Pending', @TotalPrice, GETDATE());

&nbsp;   

&nbsp;   -- Аудит: trg\_Booking\_UpdateAudit спрацює при подальшому оновленні статусу

END;

GO



3\. Підтверджує бронювання. Зміна статусу викликає тригер trg\_Booking\_IncrementCapacity та оновлює аудит.



CREATE PROCEDURE usp\_ConfirmBooking

&nbsp;   @BookingID UNIQUEIDENTIFIER

AS

BEGIN

&nbsp;   SET NOCOUNT ON;

&nbsp;   

&nbsp;   UPDATE Booking

&nbsp;   SET status = 'Confirmed'

&nbsp;   -- Тригер trg\_Booking\_UpdateAudit оновить last\_modified\_at

&nbsp;   -- Тригер trg\_Booking\_IncrementCapacity оновить TourSchedule.current\_bookings

&nbsp;   WHERE id = @BookingID AND status = 'Pending';

&nbsp;   

&nbsp;   IF @@ROWCOUNT = 0

&nbsp;       THROW 50003, 'Booking not found or already confirmed.', 1;

END;

GO



4\. Оновлює ціну та опис турпакету, а також зберігає інформацію про користувача, що вніс зміни.



CREATE PROCEDURE usp\_UpdateTourPackageDetails

&nbsp;   @TourID UNIQUEIDENTIFIER,

&nbsp;   @NewPrice DECIMAL(10, 2),

&nbsp;   @NewDescription NVARCHAR(MAX),

&nbsp;   @ModifiedByEmail NVARCHAR(100) -- Користувач, що вніс зміни

AS

BEGIN

&nbsp;   SET NOCOUNT ON;

&nbsp;   

&nbsp;   UPDATE TourPackage

&nbsp;   SET base\_price = @NewPrice,

&nbsp;       description = @NewDescription,

&nbsp;       -- Збереження користувача, що змінив дані

&nbsp;       updated\_by\_email = @ModifiedByEmail

&nbsp;   -- Тригер trg\_TourPackage\_UpdateAudit оновить updated\_at

&nbsp;   WHERE id = @TourID;

&nbsp;   

&nbsp;   IF @@ROWCOUNT = 0

&nbsp;       THROW 50004, 'Tour Package not found.', 1;

END;

GO



5\. Отримує список доступних турів на задану дату, фільтруючи видалені пакети.



CREATE PROCEDURE usp\_GetAvailableTours

&nbsp;   @TargetDate DATE

AS

BEGIN

&nbsp;   SET NOCOUNT ON;



&nbsp;   SELECT 

&nbsp;       tp.title,

&nbsp;       ts.start\_date,

&nbsp;       ts.end\_date,

&nbsp;       tp.base\_price,

&nbsp;       ts.max\_capacity - ts.current\_bookings AS AvailableSeats

&nbsp;   FROM TourSchedule ts

&nbsp;   INNER JOIN TourPackage tp ON ts.tour\_id = tp.id

&nbsp;   WHERE ts.start\_date <= @TargetDate

&nbsp;     AND ts.end\_date >= @TargetDate

&nbsp;     -- Використання Soft Delete батьківської сутності

&nbsp;     AND tp.is\_deleted = 0 

&nbsp;     AND ts.current\_bookings < ts.max\_capacity;

END;

GO





##### &nbsp;					=====USER-DEFINED FUNCTIONS====



1. Обчислює фінальну вартість туру з урахуванням податку.
2. 

CREATE FUNCTION fn\_CalculateFinalPrice

(

&nbsp;   @BasePrice DECIMAL(10, 2),

&nbsp;   @TaxRate DECIMAL(4, 2) -- наприклад, 0.15 для 15%

)

RETURNS DECIMAL(10, 2)

AS

BEGIN

&nbsp;   DECLARE @FinalPrice DECIMAL(10, 2);

&nbsp;   SELECT @FinalPrice = @BasePrice \* (1 + @TaxRate);

&nbsp;   RETURN @FinalPrice;

END;

GO



2\. Повертає повне ім'я мандрівника.



CREATE FUNCTION fn\_GetTravelerFullName

(

&nbsp;   @TravelerEmail NVARCHAR(100)

)

RETURNS NVARCHAR(101)

AS

BEGIN

&nbsp;   DECLARE @FullName NVARCHAR(101);

&nbsp;   

&nbsp;   SELECT @FullName = first\_name + ' ' + last\_name

&nbsp;   FROM Traveler

&nbsp;   WHERE email = @TravelerEmail;

&nbsp;   

&nbsp;   RETURN ISNULL(@FullName, 'N/A');

END;

GO



3\. Повертає таблицю всіх активних турів для конкретної турагенції.



CREATE FUNCTION fn\_GetAgencyTours

(

&nbsp;   @AgencyTaxID VARCHAR(50)

)

RETURNS TABLE

AS

RETURN

(

&nbsp;   SELECT 

&nbsp;       id, 

&nbsp;       title, 

&nbsp;       base\_price, 

&nbsp;       updated\_at, 

&nbsp;       updated\_by\_email

&nbsp;   FROM TourPackage

&nbsp;   WHERE agency\_tax\_id = @AgencyTaxID

&nbsp;     AND is\_deleted = 0

);

GO



##### &nbsp;						=====VIEWS====



1 Показує лише підтверджені, активні бронювання, виключаючи "м'яко видалені" тури.



CREATE VIEW v\_ActiveConfirmedBookings

AS

SELECT 

&nbsp;   b.id AS BookingID,

&nbsp;   b.booking\_date,

&nbsp;   b.total\_price,

&nbsp;   b.last\_modified\_at,

&nbsp;   dbo.fn\_GetTravelerFullName(b.traveler\_email) AS TravelerName,

&nbsp;   tp.title AS TourTitle,

&nbsp;   tp.is\_deleted AS TourSoftDeleted -- Для контролю

FROM Booking b

INNER JOIN TourSchedule ts ON b.tour\_schedule\_id = ts.id

INNER JOIN TourPackage tp ON ts.tour\_id = tp.id

WHERE b.status = 'Confirmed' 

&nbsp; AND tp.is\_deleted = 0; -- Фільтрація Soft Delete

GO



2\. Показує основні дані турпакету та інформацію про останній аудит (хто і коли змінив).



CREATE VIEW v\_AuditTrailTourPackages

AS

SELECT 

&nbsp;   id,

&nbsp;   title,

&nbsp;   base\_price,

&nbsp;   updated\_at,

&nbsp;   updated\_by\_email,

&nbsp;   agency\_tax\_id

FROM TourPackage

WHERE updated\_by\_email IS NOT NULL; -- Показує лише ті, що були хоча б раз змінені користувачем

GO


/*
1. создать базу данных, спроектированную в рамках
лабораторной работы №4, используя изученные в
лабораторных работах 5-10 средства SQL Server 2012:
• поддержания создания и физической организации базы данных; --ОК
• различных категорий целостности;                    -- ON ACTION и т.п.
• представления и индексы;                            -- есть из лабы 7
• хранимые процедуры, функции и триггеры;             -- лабы 8-9
2. создание объектов базы данных должно осуществляться
средствами DDL (CREATE/ALTER/DROP), в обязательном
порядке иллюстрирующих следующие аспекты:
• добавление и изменение полей;                                   --OK
• назначение типов данных;                                        -- OK
• назначение ограничений целостности (PRIMARY KEY, NULL/NOT
NULL/UNIQUE, CHECK и т.п.);                                        -- ОК
• определение значений по умолчанию;                               -- ОК (newid(), CREATE TABLE Occasion.Crime ... DEFAULT (0) NOT NULL)
3. в рассматриваемой базе данных должны быть тем или иным образом (в
рамках объектов базы данных или дополнительно) созданы запросы DML
для:
• выборки записей (команда SELECT);            --OK
• добавления новых записей (команда INSERT), как с помощью непосредственного -- OK
указания значений, так и с помощью команды SELECT;     -- ОК (gradeHistory)
• модификации записей (команда UPDATE); --OK
• удаления записей (команда DELETE); --OK
4. запросы, созданные в рамках пп.2,3 должны иллюстрировать следующие
возможности языка:
– удаление повторяющихся записей (DISTINCT); -- рядом с "Номер личного дела изменить нельзя"
– выбор, упорядочивание и именование полей (создание псевдонимов для полей и  --ОК
таблиц / представлений);
– соединение таблиц (INNER JOIN / LEFT JOIN / RIGHT JOIN / FULL OUTER JOIN); -- OK
– условия выбора записей (в том числе, условия NULL / LIKE / BETWEEN / IN / -- UPDATE Persons.viewPersonInvestigator SET name = N'John' WHERE personalNumber between 100 and 200;
EXISTS);
– сортировка записей (ORDER BY - ASC, DESC);
– группировка записей (GROUP BY + HAVING, использование функций агрегирования
– COUNT / AVG / SUM / MIN / MAX);
– объединение результатов нескольких запросов (UNION / UNION ALL / EXCEPT /
INTERSECT);
– вложенные запросы.
*/

USE master;
GO
 
--  Создать базу данных
IF DB_ID (N'CrimeAndPunishment') IS NOT NULL
    DROP DATABASE CrimeAndPunishment;
GO
 
CREATE DATABASE CrimeAndPunishment
    ON ( NAME = CrimeAndPunishment_data, FILENAME = 'C:\SQL\CrimeAndPunishment.mdf',
         SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
    LOG ON ( NAME = CrimeAndPunishment_log, FILENAME = 'C:\SQL\CrimeAndPunishment_log.ldf',
             SIZE = 5, MAXSIZE = UNLIMITED, FILEGROWTH = 5 );
GO

USE CrimeAndPunishment;

-- Добавить файловую группу и файл данных
IF OBJECT_ID('CrimeAndPunishment_filegroup') IS NOT NULL OR OBJECT_ID (N'CrimeAndPunishment_filegroup') IS NOT NULL
	ALTER DATABASE CrimeAndPunishment 
		REMOVE FILEGROUP CrimeAndPunishment_filegroup;
GO

ALTER DATABASE CrimeAndPunishment
	ADD FILEGROUP CrimeAndPunishment_filegroup;

ALTER DATABASE CrimeAndPunishment
	ADD FILE ( NAME = CrimeAndPunishment_filegroup_file, FILENAME = 'C:\SQL\CrimeAndPunishment_filegroup_file.ndf',
				SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
				TO FILEGROUP CrimeAndPunishment_filegroup

-- Сделать созданную файловую группу файловой группой по умолчанию.
ALTER DATABASE CrimeAndPunishment
	MODIFY FILEGROUP CrimeAndPunishment_filegroup DEFAULT;
GO

-- --------------------------------------------------

IF OBJECT_ID(N'Occasion.updateCrime', 'TR') IS NOT NULL DROP TRIGGER Occasion.updateCrime;
IF OBJECT_ID(N'Occasion.insertCrime', 'TR') IS NOT NULL DROP TRIGGER Occasion.insertCrime;
IF OBJECT_ID(N'Occasion.deleteCrime', 'TR') IS NOT NULL DROP TRIGGER Occasion.deleteCrime;
IF OBJECT_ID(N'Persons.insertVeiwPersonInvestigator', 'TR') IS NOT NULL DROP TRIGGER Persons.insertVeiwPersonInvestigator;
IF OBJECT_ID(N'Persons.deleteVeiwPersonInvestigator', 'TR') IS NOT NULL DROP TRIGGER Persons.deleteVeiwPersonInvestigator;
IF OBJECT_ID(N'Persons.updateVeiwPersonInvestigator', 'TR') IS NOT NULL DROP TRIGGER Persons.updateVeiwPersonInvestigator;
GO
IF OBJECT_ID(N'viewAttributeCompensation')  IS NOT NULL DROP VIEW viewAttributeCompensation;
IF OBJECT_ID(N'viewCrimeHarry')             IS NOT NULL DROP VIEW viewCrimeHarry;
IF OBJECT_ID(N'Persons.viewPersonInvestigator') IS NOT NULL DROP VIEW Persons.viewPersonInvestigator;
GO
IF OBJECT_ID (N'Occasion.SufferAttribute')   IS NOT NULL DROP TABLE Occasion.SufferAttribute;
IF OBJECT_ID (N'Occasion.CriminalAttribute') IS NOT NULL DROP TABLE Occasion.CriminalAttribute;
GO
IF OBJECT_ID (N'Persons.Сriminal')     IS NOT NULL DROP TABLE Persons.Criminal;
IF OBJECT_ID (N'Persons.Suffered')     IS NOT NULL DROP TABLE Persons.Suffered;
IF OBJECT_ID (N'Persons.Investigator') IS NOT NULL DROP TABLE Persons.Investigator;

GO
IF OBJECT_ID (N'Persons.Person')       IS NOT NULL DROP TABLE Persons.Person;
IF OBJECT_ID (N'Occasion.Crime')       IS NOT NULL DROP TABLE Occasion.Crime;
GO


IF OBJECT_ID(N'Occasion') IS NOT NULL
DROP SCHEMA Occasion
GO

CREATE SCHEMA Occasion
GO
CREATE SCHEMA Persons
GO

-- ******************************FINISH CREATING DATABASE******************************

CREATE TABLE Occasion.Crime
(
	caseNumber int IDENTITY(1,1) PRIMARY KEY,
	condition  bit
		CONSTRAINT DF_Crime_condition DEFAULT (0) NOT NULL,
	dateHappened datetime NULL, 
	caseDescription nvarchar(256) NOT NULL,

	CONSTRAINT CHCK_Crime_date
	CHECK (YEAR(dateHappened) > 2000)
);
GO
 
 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'Карл у Клары украл кораллы.',
		 DATETIMEFROMPARTS ( 2001, 12, 31, 23, 59, 59, 0 ))
		 --DATETIMEFROMPARTS ( 1998, 12, 31, 23, 59, 59, 0 ))
 GO

 
 -- 3. Создать таблицу с первичным ключом на основе глобального уникального идентификатора.
       
CREATE TABLE Persons.Person
(
	--personID UNIQUEIDENTIFIER DEFAULT NEWID(),
	personID UNIQUEIDENTIFIER ROWGUIDCOL PRIMARY KEY NOT NULL
		CONSTRAINT DF_PersonID_rowguid DEFAULT (newid()),
	surname    nvarchar(50) NOT NULL,
	name       text NULL
);
GO

ALTER TABLE Persons.Person ALTER COLUMN name nvarchar(50) NOT NULL
GO
ALTER TABLE Persons.Person ADD patronymic nvarchar(50)
GO
ALTER TABLE Persons.Person ALTER COLUMN patronymic nvarchar(50) NULL
GO

CREATE TABLE Persons.Investigator (

FK_personID UNIQUEIDENTIFIER FOREIGN KEY
	REFERENCES Persons.Person (personID)
	ON DELETE CASCADE,
personalNumber int PRIMARY KEY NOT NULL,
grade nvarchar(50)
);
GO

CREATE TABLE Persons.Criminal (

FK_personID UNIQUEIDENTIFIER UNIQUE FOREIGN KEY
	REFERENCES Persons.Person (personID)
	ON DELETE CASCADE
);
GO

CREATE TABLE Persons.Suffered (

FK_personID UNIQUEIDENTIFIER UNIQUE FOREIGN KEY
	REFERENCES Persons.Person (personID)
	ON DELETE CASCADE
);
GO

-- Создать таблицу с первичным ключом на основе последовательности.
CREATE TABLE Occasion.SufferAttribute
(
	AttrID int PRIMARY KEY,
	suffer nvarchar(50) NOT NULL,
	compensation text NULL,
	caseNumberPtr int NULL,
	CONSTRAINT FK_SufferAttribute_Crime FOREIGN KEY (caseNumberPtr) REFERENCES Occasion.Crime (caseNumber)     
		ON DELETE SET NULL
		ON UPDATE CASCADE,
		
	FK_sufferedID uniqueidentifier NULL FOREIGN KEY REFERENCES Persons.Suffered (FK_personID)
		ON UPDATE NO ACTION
		ON DELETE CASCADE
);
GO

CREATE TABLE Occasion.CriminalAttribute
(
	AttrID int PRIMARY KEY,
	suffer nvarchar(50) NOT NULL,
	compensation text NULL,
	caseNumberPTR int NULL,
	CONSTRAINT FK_CriminalAttribute_Crime FOREIGN KEY (caseNumberPtr) REFERENCES Occasion.Crime (caseNumber)  
		ON UPDATE NO ACTION   
		ON DELETE SET NULL,

	FK_criminalID uniqueidentifier NULL FOREIGN KEY REFERENCES Persons.Criminal (FK_personID)
		ON UPDATE NO ACTION
		ON DELETE SET NULL
);
GO

CREATE SEQUENCE Occasion.AttributesCounter
	START     WITH 1
	INCREMENT BY   1;
GO

INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Украден коралл', SCOPE_IDENTITY())
GO

INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'Клара у Карла украла кларнет', CURRENT_TIMESTAMP)
 GO

DELETE FROM Occasion.Crime WHERE caseNumber = 1;  
GO

-- ********************************* VIEWS AND INDECES *********************************

SET DATEFORMAT dmy;
GO

INSERT INTO Occasion.Crime (caseDescription, dateHappened, condition)
 VALUES (N'Котёнок Гав спрятал все котлеты в надежное место.', N'31/12/2008', 1)
 GO

INSERT Occasion.SufferAttribute (AttrID, suffer, compensation, caseNumberPTR)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Котлет не осталось', N'Компенсация моральная.', SCOPE_IDENTITY())
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'Тот-Кого-Нельзя-Называть убил родителей Гарри Поттера.', CURRENT_TIMESTAMP)
 GO

INSERT Occasion.SufferAttribute (AttrID, suffer,compensation, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Гарри остался сиротой.', N'Гарри отомстил.',SCOPE_IDENTITY())
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened, condition)
 values (N'Кум Тыква строил из кирпичей домик в неположенном месте.', CURRENT_TIMESTAMP, 1)
 GO

Declare @last int = SCOPE_IDENTITY()
INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Сеньор Помидор не получил от кума Тыквы налог.', @last)

INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Кум Тыква пострадал от сеньора Помидора.', @last)
GO

SELECT * FROM Occasion.Crime
GO
SELECT * FROM Occasion.SufferAttribute
GO

CREATE VIEW viewAttributeCompensationRIGHT -- Недостающие слева заполняются NULL
AS
SELECT a.suffer AS sufferRIGHT, a.compensation AS compensation,
b.caseNumber as number, b.dateHappened as date
FROM Occasion.SufferAttribute a RIGHT JOIN Occasion.Crime b ON a.caseNumberPTR = b.caseNumber
GO

CREATE VIEW viewAttributeCompensationLEFT -- Недостающие справа заполняются NULL
AS
SELECT a.suffer AS sufferLEFT, a.compensation AS compensation,
b.caseNumber as number, b.dateHappened as date
FROM Occasion.SufferAttribute a LEFT JOIN Occasion.Crime b ON a.caseNumberPTR = b.caseNumber
GO

CREATE VIEW viewAttributeCompensationINNER -- Пересечение записей
AS
SELECT a.suffer AS sufferINNER, a.compensation AS compensation,
b.caseNumber as number, b.dateHappened as date
FROM Occasion.SufferAttribute a INNER JOIN Occasion.Crime b ON a.caseNumberPTR = b.caseNumber
GO

CREATE VIEW viewAttributeCompensationOUTER -- Объединение записей
AS
SELECT a.suffer AS sufferOUTER, a.compensation AS compensation,
b.caseNumber as number, b.dateHappened as date
FROM Occasion.SufferAttribute a FULL OUTER JOIN Occasion.Crime b ON a.caseNumberPTR = b.caseNumber
GO

SELECT * FROM viewAttributeCompensationRIGHT
SELECT * FROM viewAttributeCompensationLEFT
SELECT * FROM viewAttributeCompensationINNER
SELECT * FROM viewAttributeCompensationOUTER
GO


-- 3.Создать индекс для одной из таблиц задания 6, включив в него дополнительные неключевые поля.
IF OBJECT_ID (N'CandP_Indeces_filegroup') IS NOT NULL
	ALTER DATABASE CrimeAndPunishment 
		REMOVE FILEGROUP CandP_Indeces_filegroup;
GO

ALTER DATABASE CrimeAndPunishment
	ADD FILEGROUP CandP_Indeces_filegroup;

ALTER DATABASE CrimeAndPunishment
	ADD FILE ( NAME = CandP_Indeces_filegroup_file, FILENAME = 'C:\SQL\CandP_Indeces_filegroup_file.ndf',
				SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
				TO FILEGROUP CandP_Indeces_filegroup
GO

IF OBJECT_ID(N'indexCrimeDates') IS NOT NULL DROP INDEX indexCrimeDates ON Occasion.Crime
GO

CREATE UNIQUE NONCLUSTERED INDEX indexCrimeDates -- UNIQUE = не может быть 2-х одинаковых значений ключа
        ON Occasion.Crime (dateHappened, caseNumber DESC) -- ключевые --
		INCLUDE (caseDescription) -- неключевые --
ON CandP_Indeces_filegroup
GO

-- 4.Создать индексированное представление.
set ansi_nulls on
GO
CREATE VIEW viewClosedCrimesIndexed
WITH SCHEMABINDING
AS
SELECT crime.caseNumber AS caseNumber, crime.condition AS condition
FROM Occasion.Crime crime
GO

CREATE UNIQUE CLUSTERED INDEX indexOfClosedCrimes_clustered -- UNIQUE = не может быть 2-х одинаковых значений ключа
        ON viewClosedCrimesIndexed (caseNumber) -- ключевые --
ON CandP_Indeces_filegroup
GO

CREATE INDEX indexOfClosedCrimes_nonclustered
ON viewClosedCrimesIndexed (condition) -- ключевые --
ON CandP_Indeces_filegroup
GO

SELECT * FROM viewClosedCrimesIndexed
GO

-- *********************************** ПРОЦЕДУРЫ И КУРСОРЫ ***********************************

-- 8.2.Модифицировать хранимую процедуру п.1. таким образом, чтобы выборка осуществлялась с формированием столбца, значение которого формируется пользовательской функцией.

--periodOfPrescription -- истечение срока давности

IF OBJECT_ID (N'Occasion.periodOfPrescriptionExpiry', N'FN') IS NOT NULL 
	DROP FUNCTION Occasion.periodOfPrescriptionExpiry; 
GO

CREATE FUNCTION Occasion.periodOfPrescriptionExpiry (@crimeDate datetime)
RETURNS datetime
AS BEGIN
DECLARE @elapse int = 2
RETURN DATEADD(year, @elapse, @crimeDate);
END;
GO

-- 8.4.Модифицировать хранимую процедуру п.2. таким образом, чтобы выборка формировалась с помощью табличной функции.

IF OBJECT_ID (N'Occasion.formTableExpiry', N'FN') IS NOT NULL 
	DROP FUNCTION Occasion.formTableExpiry; 
GO

CREATE FUNCTION Occasion.formTableExpiry (@oldest datetime)
RETURNS TABLE
	AS 
		RETURN (
			SELECT *, expiry = Occasion.periodOfPrescriptionExpiry(src.dateHappened)
			FROM Occasion.Crime src
			WHERE src.dateHappened > @oldest
		)
GO

IF OBJECT_ID (N'getCrimesByDateAfter', 'P') IS NOT NULL 
	DROP PROCEDURE getCrimesByDateAfter; 
GO

CREATE PROCEDURE getCrimesByDateAfter
	@oldest datetime
AS

	SELECT * FROM  Occasion.formTableExpiry(@oldest) ORDER BY dateHappened
GO


-- Лаба 7:
-- 1.Создать представление на основе одной из таблиц задания 6.
-- 2.Создать представление на основе полей обеих связанных таблиц задания 6.
-- Лаба 9:
-- 1.Для одной из таблиц пункта 2 задания 7 создать триггеры на вставку, удаление и добавление,
-- при выполнении заданных условий один из триггеров должен инициировать возникновение ошибки (RAISERROR / THROW).
-- 2.Для представления пункта 2 задания 7создать триггеры на вставку, удаление и добавление,
-- обеспечивающие возможность выполнения операций с данными непосредственно через представление.

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
GO
IF OBJECT_ID(N'viewAttributeCompensation')  IS NOT NULL DROP VIEW viewAttributeCompensation;
IF OBJECT_ID(N'viewCrimeHarry')             IS NOT NULL DROP VIEW viewCrimeHarry;
IF OBJECT_ID(N'Persons.viewPersonInvestigator') IS NOT NULL DROP VIEW Persons.viewPersonInvestigator;
GO
IF OBJECT_ID (N'Occasion.SufferAttribute') IS NOT NULL DROP TABLE Occasion.SufferAttribute;
GO
IF OBJECT_ID (N'Occasion.Crime')       IS NOT NULL DROP TABLE Occasion.Crime;
IF OBJECT_ID (N'Persons.Investigator') IS NOT NULL DROP TABLE Persons.Investigator;
IF OBJECT_ID (N'Persons.Person')       IS NOT NULL DROP TABLE Persons.Person;
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
	name       nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL,
	surname    nvarchar(50) NOT NULL
);
GO

-- Создать таблицу с первичным ключом на основе последовательности.
CREATE TABLE Occasion.SufferAttribute
(
	AttrID int PRIMARY KEY,
	suffer nvarchar(50) NOT NULL,
	compensation text NULL,

	--caseNumberPtr int NULL,
	--caseNumberPtr int NOT NULL,
	caseNumberPTR int NOT NULL,

--	CONSTRAINT FK_SufferAttribute_Crime FOREIGN KEY (caseNumberPtr)     
--		REFERENCES Occasion.Crime (caseNumber)     
		--ON DELETE CASCADE    
		--ON UPDATE CASCADE

		--ON DELETE SET NULL    --конфликт, если caseNumberPtr int NOT NULL,
		--ON UPDATE SET NULL

--		ON DELETE SET DEFAULT   --конфликт, если caseNumberPtr int NOT NULL и не установлено по умолчанию значение
--		ON UPDATE SET DEFAULT

		--ON DELETE NO ACTION    
		--ON UPDATE NO ACTION
		
);
GO

CREATE SEQUENCE Occasion.AttributesCounter
	START WITH 1
	INCREMENT BY 1;
GO

INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Украден коралл', SCOPE_IDENTITY())
--IDENT_CURRENT(N'Occasion.Crime'), SCOPE_IDENTITY(), @@IDENTITY
GO

INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'Клара у Карла украла кларнет', CURRENT_TIMESTAMP)
 GO

DELETE FROM Occasion.Crime WHERE caseNumber = 1;  
GO  

-- 5.Создать две связанные таблицы, и протестировать на них различные варианты
-- действий для ограничений ссылочной целостности (NO ACTION| CASCADE | SET NULL | SET DEFAULT).


-- ********************************* VIEWS AND INDECES *********************************



INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 VALUES (N'Котёнок Гав спрятал все котлеты в надежное место.', CURRENT_TIMESTAMP)
 GO

INSERT Occasion.SufferAttribute (AttrID, suffer, compensation, caseNumberPTR)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Котлет не осталось', N'Компенсация моральная.', SCOPE_IDENTITY())
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'Тот-Кого-Нельзя-Называть убил родителей Гарри Поттера.', CURRENT_TIMESTAMP)
 GO

INSERT Occasion.SufferAttribute (AttrID, suffer,compensation, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Гарри остался сиротой.', N'Гарри отомстил.', SCOPE_IDENTITY())
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'Кум Тыква строил из кирпичей домик в неположенном месте.', CURRENT_TIMESTAMP)
 GO

Declare @last int = SCOPE_IDENTITY()
INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Сеньор Помидор не получил от кума Тыквы налог.', @last)

INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'Кум Тыква пострадал от сеньора Помидора.', @last)
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'*One more test entry*', CURRENT_TIMESTAMP)
 GO

SELECT * FROM Occasion.Crime
GO
SELECT * FROM Occasion.SufferAttribute
GO

-- *************************** TRIGGERS ***************************
-- 1.Для одной из таблиц пункта 2 задания 7 создать триггеры на вставку, удаление и добавление,
-- при выполнении заданных условий один из триггеров должен инициировать возникновение ошибки (RAISERROR / THROW).

CREATE TRIGGER Occasion.deleteCrime
	ON  Occasion.Crime
	INSTEAD OF DELETE
	AS
	-- Номера пользовательских сообщений об ошибках должны быть больше 50 000
	THROW 50000, N'Что написано пером - не вырубить топором', 0; -- 0 - state Если одна и та же пользовательская ошибка возникает в нескольких местах, то при помощи уникального номера состояния для каждого местоположения можно определить, в каком месте кода появилась ошибка.
GO

BEGIN TRY
	DELETE FROM Occasion.Crime WHERE dateHappened IS NULL;
	PRINT N'TRY: DELETE FROM Occasion.Crime WHERE dateHappened IS NULL; Done.'
END TRY
BEGIN CATCH
	PRINT N'TRY: DELETE FROM Occasion.Crime WHERE dateHappened IS NULL; ERROR!'
	SELECT * FROM Occasion.Crime
END CATCH
GO

 -- ----------------------------------------


CREATE TRIGGER Occasion.insertCrime
	ON  Occasion.Crime
	AFTER INSERT
	AS
	
	DECLARE @num_rows int;
	--SET @num_rows = (SELECT COUNT(*) FROM inserted)
	SET @num_rows = @@ROWCOUNT
	PRINT N'В таблицу Occasion.insertCrime вставлено ' + CONVERT(nvarchar(50), @num_rows) + N' значений';
GO

INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'Пара-пам-пам-пам', CURRENT_TIMESTAMP), (N'Курение убивает', CURRENT_TIMESTAMP)
 GO

 SELECT * FROM Occasion.Crime
 GO


 -- ----------------------------------------
CREATE TRIGGER Occasion.updateCrime
	ON  Occasion.Crime
	INSTEAD OF UPDATE
	--AFTER UPDATE 
	AS

	SELECT * FROM deleted
	SELECT * FROM inserted
	
GO


-- Если триггер INSTEAD OF UPDATE - никакие значения не вставляются,
-- но и ошибки из-за caseDescription NOT NULL не происходит
-- Если триггер AFTER - просиходит ошибка
UPDATE Occasion.Crime SET caseDescription = NULL WHERE caseDescription LIKE N'K%'
GO

SELECT * FROM Occasion.Crime
GO

--****************************************************************************

CREATE TABLE Persons.Investigator (

FK_personID UNIQUEIDENTIFIER FOREIGN KEY
	REFERENCES Persons.Person (personID)
	ON DELETE CASCADE,
personalNumber int,
grade nvarchar(50)
);
GO

CREATE VIEW Persons.viewPersonInvestigator
AS
--SELECT investigator.personalNumber, investigator.grade,
--       person.name, person.patronymic, person.surname
SELECT investigator.*, person.*
FROM
Persons.Investigator investigator, Persons.Person person
WHERE investigator.FK_personID = person.personID
GO

-- Для представления допустимы только триггеры INSTEAD OF

CREATE TRIGGER Persons.insertVeiwPersonInvestigator
ON Persons.viewPersonInvestigator
INSTEAD OF INSERT
AS
BEGIN
DECLARE @temp TABLE (
    personID UNIQUEIDENTIFIER ROWGUIDCOL PRIMARY KEY NOT NULL DEFAULT (newid()),
	name       nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL,
	surname    nvarchar(50) NOT NULL,
	personalNumber int,
	grade nvarchar(50) );

--INSERT INTO Persons.viewPersonInvestigator SELECT * FROM inserted;
INSERT INTO @temp(name, patronymic, surname, personalNumber, grade)
          SELECT name, patronymic, surname, personalNumber, grade
		  FROM inserted;

INSERT INTO Persons.Person(personID, name, surname, patronymic)
		            SELECT personID, name, surname, patronymic
					FROM @temp;
INSERT INTO Persons.Investigator(FK_personID,   personalNumber,  grade)
		                 SELECT     personID,   personalNumber,  grade
					FROM @temp;
--INSERT INTO Persons.viewPersonInvestigator(name, patronymic, surname, personalNumber, grade, personID, FK_personID)
--								    SELECT name, patronymic, surname, personalNumber, grade, personID, personID
--									FROM @temp;
END
GO

/*
CREATE TRIGGER Persons.insertVeiwPersonInvestigator
ON Persons.viewPersonInvestigator
INSTEAD OF INSERT
AS
BEGIN

DECLARE @personalNumber int,
        @grade      nvarchar(50),
        @name       nvarchar(50),
		@surname    nvarchar(50),
		@patronymic nvarchar(50);
DECLARE @newPersonID uniqueidentifier;

DECLARE @dataCursor CURSOR
SET @dataCursor = CURSOR STATIC FOR
		SELECT * FROM inserted
OPEN @dataCursor

FETCH NEXT FROM @dataCursor INTO @personalNumber, @grade, @name, @surname, @patronymic;
While @@FETCH_STATUS = 0
BEGIN
	
	SET @newPersonID = newid();
	INSERT INTO Persons.Person (personID,     name,  surname,  patronymic)
		               VALUES (@newPersonID, @name, @surname, @patronymic);

	INSERT INTO Persons.Investigator(FK_personID,   personalNumber,  grade)
		                     VALUES (@newPersonID, @personalNumber, @grade)
	FETCH NEXT FROM @dataCursor INTO @personalNumber, @grade, @name, @surname, @patronymic;
END

CLOSE @dataCursor; 
DEALLOCATE @dataCursor;

END
GO

*/

INSERT INTO Persons.viewPersonInvestigator(name, patronymic, surname, personalNumber, grade)
			VALUES(N'Василий', N'Петрович', N'Иванов', 333, N'Майор'),
				  (N'Иван', N'Иванович', N'Петров', 123, N'Подполковник')
GO

SELECT * FROM Persons.Investigator;
SELECT * FROM Persons.Person;
SELECT * FROM Persons.viewPersonInvestigator;
GO

-- --------------------
CREATE TRIGGER  Persons.deleteVeiwPersonInvestigator
ON Persons.viewPersonInvestigator
INSTEAD OF DELETE
AS
 DELETE FROM Persons.Person WHERE Persons.Person.personID IN (SELECT personID FROM deleted)
/*
DELETE FROM Persons.Person WHERE Persons.Person.name       IN (SELECT name       FROM deleted)
                             AND Persons.Person.patronymic IN (SELECT patronymic FROM deleted)
							 AND Persons.Person.surname    IN (SELECT surname    FROM deleted);
							 */
-- Соответствующие строки в Persons.Investigator удаляются автоматически благодаря DELETE CASCADE
GO 

DELETE Persons.viewPersonInvestigator WHERE name LIKE N'Василий'
GO

SELECT * FROM Persons.Investigator;
SELECT * FROM Persons.Person;
SELECT * FROM Persons.viewPersonInvestigator;
GO
-- ---------------------------

CREATE TRIGGER  Persons.updateVeiwPersonInvestigator
ON Persons.viewPersonInvestigator
INSTEAD OF UPDATE
AS
IF UPDATE(personalNumber)
	THROW 50001, N'Номер личного дела изменить нельзя', 0
/*UPDATE Persons.Person SET name = inserted.name,
                       surname = inserted.surname, 
				    patronymic = inserted.patronymic
		              FROM inserted
					  WHERE Persons.Person.personID IN (SELECT FK_personID FROM Persons.Investigator
											WHERE inserted.personalNumber = personalNumber) */
UPDATE  Persons.Person SET name = inserted.name,
                       surname = inserted.surname, 
				    patronymic = inserted.patronymic
		              FROM inserted
					  WHERE Persons.Person.personID = inserted.personalNumber;

UPDATE Persons.Investigator SET grade = inserted.grade FROM inserted
	WHERE Persons.Investigator.personalNumber =  inserted.personalNumber;
GO

-- Если update не переопределен, подполковник Петров получает личный номер 0
INSERT INTO Persons.viewPersonInvestigator(name, patronymic, surname, personalNumber, grade)
			VALUES(N'A', N'Б', N'Иванов', 345, N'Майор'),
				  (N'Иван', N'Иванович', N'Петров', 9, N'Подполковник')
GO
UPDATE Persons.viewPersonInvestigator SET name = N'John' WHERE personalNumber < 100;
-- UPDATE Persons.viewPersonInvestigator SET personalNumber = 0 WHERE NOT personalNumber = 0


GO
SELECT * FROM Persons.Investigator;
SELECT * FROM Persons.Person;
SELECT * FROM Persons.viewPersonInvestigator;
GO
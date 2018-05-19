/*
Задание 15. Создание распределенных баз данных со связанными таблицами средствами СУБД SQL Server 2012
1.Создать в базах данных пункта 1 задания 13 связанные таблицы.
(
   Задание 13.Создание распределенных баз данных на основе секционированных представлений
   1.Создать две базы данных на одном экземпляре СУБД SQL Server 2012.
)
2.Создать необходимые элементы базы данных (представления, триггеры), обеспечивающие работу с данными
связанных таблиц (выборку, вставку, изменение, удаление).
*/

USE master;
GO
 
-- 1. Создать базу данных
IF DB_ID (N'CrimeAndPunishment_1') IS NOT NULL
DROP DATABASE CrimeAndPunishment_1;
GO

IF DB_ID (N'CrimeAndPunishment_2') IS NOT NULL
DROP DATABASE CrimeAndPunishment_2;
GO
 
CREATE DATABASE CrimeAndPunishment_1
    ON ( NAME = CrimeAndPunishment_1_data, FILENAME = 'C:\SQL\CrimeAndPunishment_1.mdf',
         SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
    LOG ON ( NAME = CrimeAndPunishment_1_log, FILENAME = 'C:\SQL\CrimeAndPunishment_1_log.ldf',
             SIZE = 5, MAXSIZE = UNLIMITED, FILEGROWTH = 5 );
GO

CREATE DATABASE CrimeAndPunishment_2
    ON ( NAME = CrimeAndPunishment_2_data, FILENAME = 'C:\SQL\CrimeAndPunishment_2.mdf',
         SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
    LOG ON ( NAME = CrimeAndPunishment_1_log, FILENAME = 'C:\SQL\CrimeAndPunishment_2_log.ldf',
             SIZE = 5, MAXSIZE = UNLIMITED, FILEGROWTH = 5 );
GO

USE CrimeAndPunishment_1;
GO
ALTER DATABASE CrimeAndPunishment_1
	ADD FILEGROUP CrimeAndPunishment_1_filegroup;

ALTER DATABASE CrimeAndPunishment_1
	ADD FILE ( NAME = CrimeAndPunishment_1_filegroup_file,
	FILENAME = 'C:\SQL\CrimeAndPunishment_1_filegroup_file.ndf',
				SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
				TO FILEGROUP CrimeAndPunishment_1_filegroup
GO

-- Сделать созданную файловую группу файловой группой по умолчанию.
ALTER DATABASE CrimeAndPunishment_1
	MODIFY FILEGROUP CrimeAndPunishment_1_filegroup DEFAULT;
GO

USE CrimeAndPunishment_2;
ALTER DATABASE CrimeAndPunishment_2
	ADD FILEGROUP CrimeAndPunishment_2_filegroup;

ALTER DATABASE CrimeAndPunishment_2
	ADD FILE ( NAME = CrimeAndPunishment_2_filegroup_file,
	FILENAME = 'C:\SQL\CrimeAndPunishment_2_filegroup_file.ndf',
				SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
				TO FILEGROUP CrimeAndPunishment_2_filegroup
GO

-- Сделать созданную файловую группу файловой группой по умолчанию.
ALTER DATABASE CrimeAndPunishment_2
	MODIFY FILEGROUP CrimeAndPunishment_2_filegroup DEFAULT;
GO

-- ********************************** Первая БД - создание таблицы **********************************
 
USE CrimeAndPunishment_1;
GO

CREATE SCHEMA Persons
GO
 
IF OBJECT_ID (N'Persons.Person') IS NOT NULL
    DROP TABLE Persons.Person;
GO
 
CREATE TABLE Persons.Person
(
	personID   uniqueidentifier ROWGUIDCOL PRIMARY KEY DEFAULT (newid()),
	name       nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL,
	surname    nvarchar(50) NOT NULL
);
GO

IF OBJECT_ID (N'Persons.insertPerson', 'TR') IS NOT NULL DROP TRIGGER Persons.insertPerson;
GO

-- ********************************** Вторая БД - создание таблицы **********************************

USE CrimeAndPunishment_2;
GO

CREATE SCHEMA Persons
GO
 
IF OBJECT_ID (N'Persons.Investigator') IS NOT NULL
    DROP TABLE Persons.Investigator;
GO
CREATE TABLE Persons.Investigator (
/*FK_personID UNIQUEIDENTIFIER FOREIGN KEY
	REFERENCES Persons.Person (personID)
	ON DELETE CASCADE, <- это надо вручную реализовать*/
FK_personID    uniqueidentifier    NULL,
personalNumber int PRIMARY KEY NOT NULL,
grade nvarchar(50)
);
GO

-- ***************** Триггеры: 1 БД *****************
USE CrimeAndPunishment_1;
GO

IF OBJECT_ID (N'Persons.deletePerson', 'TR') IS NOT NULL DROP TRIGGER Persons.deletePerson;
GO

CREATE TRIGGER Persons.deletePerson
ON Persons.Person
AFTER DELETE
AS
BEGIN
-- реализация каскадного удаления
DELETE CrimeAndPunishment_2.Persons.Investigator WHERE FK_personID IN (SELECT personID FROM deleted)
END
GO

IF OBJECT_ID (N'Persons.updatePerson', 'TR') IS NOT NULL DROP TRIGGER Persons.updatePerson;
GO
CREATE TRIGGER Persons.updatePerson
ON Persons.Person
INSTEAD OF UPDATE
AS
IF (UPDATE(personID)) THROW 50001, N'Суррогатный ключ изменить нельзя', 1
ELSE
BEGIN
UPDATE CrimeAndPunishment_1.Persons.Person SET name = inserted.name,
                       surname = inserted.surname, 
				    patronymic = inserted.patronymic
		              FROM inserted
					  WHERE CrimeAndPunishment_1.Persons.Person.personID = inserted.personID;
END
GO

-- ***************** СЕКЦИОНИРОВАННОЕ ПРЕДСТАВЛЕНИЕ: 1 БД *****************
USE CrimeAndPunishment_1;
GO

IF OBJECT_ID (N'Persons.viewPersonInvestigator') IS NOT NULL DROP VIEW Persons.viewPersonInvestigator;
GO

CREATE VIEW Persons.viewPersonInvestigator
AS
SELECT investigator.*, person.*
FROM
CrimeAndPunishment_2.Persons.Investigator investigator, CrimeAndPunishment_1.Persons.Person person
WHERE investigator.FK_personID = person.personID
GO

--  ---------------- Секционированное представление: триггеры ----------------

IF OBJECT_ID (N'Persons.insertVeiwPersonInvestigator', 'TR') IS NOT NULL
DROP TRIGGER Persons.insertVeiwPersonInvestigator;
GO

CREATE TRIGGER Persons.insertVeiwPersonInvestigator
ON Persons.viewPersonInvestigator
INSTEAD OF INSERT
AS
BEGIN
DECLARE @temp TABLE (
    auto_personID UNIQUEIDENTIFIER ROWGUIDCOL PRIMARY KEY NOT NULL DEFAULT (newid()),
	name       nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL,
	surname    nvarchar(50) NOT NULL,
	personalNumber int,
	grade nvarchar(50) );

--INSERT INTO Persons.viewPersonInvestigator SELECT * FROM inserted;
INSERT INTO @temp(name, patronymic, surname, personalNumber, grade)
          SELECT name, patronymic, surname, personalNumber, grade
		  FROM inserted;

INSERT INTO CrimeAndPunishment_1.Persons.Person(personID, name, surname, patronymic)
		            SELECT auto_personID, name, surname, patronymic
					FROM @temp;
INSERT INTO CrimeAndPunishment_2.Persons.Investigator(FK_personID,   personalNumber,  grade)
		            SELECT auto_personID,   personalNumber,  grade
					FROM @temp;
END
GO
-- -------------------
IF OBJECT_ID (N'Persons.deleteVeiwPersonInvestigator', 'TR') IS NOT NULL
DROP TRIGGER Persons.deleteVeiwPersonInvestigator;
GO

CREATE TRIGGER  Persons.deleteVeiwPersonInvestigator
ON Persons.viewPersonInvestigator
INSTEAD OF DELETE
AS
DELETE FROM Persons.Person WHERE Persons.Person.personID IN (SELECT personID FROM deleted)
-- Каскадное удаление из второй таблицы уже реализовано выше
GO

IF OBJECT_ID (N'Persons.updateVeiwPersonInvestigator', 'TR') IS NOT NULL
DROP TRIGGER Persons.updateVeiwPersonInvestigator;
GO

CREATE TRIGGER  Persons.updateVeiwPersonInvestigator
ON Persons.viewPersonInvestigator
INSTEAD OF UPDATE
AS
IF UPDATE(personalNumber)
	THROW 50002, N'Номер личного дела изменить нельзя', 2
UPDATE CrimeAndPunishment_1.Persons.Person SET name = inserted.name,
                       surname = inserted.surname, 
				    patronymic = inserted.patronymic
		              FROM inserted
					  WHERE CrimeAndPunishment_1.Persons.Person.personID = inserted.personID;

UPDATE CrimeAndPunishment_2.Persons.Investigator SET grade = inserted.grade FROM inserted
	WHERE CrimeAndPunishment_2.Persons.Investigator.personalNumber =  inserted.personalNumber;
GO
GO


-- -------------

USE master
GO

INSERT INTO CrimeAndPunishment_1.Persons.viewPersonInvestigator(name, patronymic, surname, personalNumber, grade)
			VALUES(N'Василий', N'Петрович', N'Иванов', 333, N'Майор'),
				  (N'Иван', N'Иванович', N'Петров', 123, N'Подполковник')
GO

SELECT * FROM CrimeAndPunishment_1.Persons.Person
SELECT * FROM CrimeAndPunishment_2.Persons.Investigator
GO
DELETE CrimeAndPunishment_1.Persons.Person WHERE name LIKE N'Василий'
GO
SELECT * FROM CrimeAndPunishment_1.Persons.Person
SELECT * FROM CrimeAndPunishment_2.Persons.Investigator
GO
UPDATE CrimeAndPunishment_1.Persons.viewPersonInvestigator SET name = N'MODIFIED' WHERE name LIKE N'Иван'

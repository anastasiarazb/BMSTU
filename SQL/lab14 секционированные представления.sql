/*
Задание 14. Создание вертикально фрагментированных таблиц средствами СУБД SQL Server 2012
1.Создать в базах данных пункта 1 задания 13 таблицы, содержащие вертикально фрагментированные данные.
2.Создать необходимые элементы базы данных (представления, триггеры), обеспечивающие работу с данными
  вертикально фрагментированных таблиц (выборку, вставку, изменение, удаление).
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
	ADD FILE ( NAME = CrimeAndPunishment_1_filegroup_file, FILENAME = 'C:\SQL\CrimeAndPunishment_1_filegroup_file.ndf',
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
	ADD FILE ( NAME = CrimeAndPunishment_2_filegroup_file, FILENAME = 'C:\SQL\CrimeAndPunishment_2_filegroup_file.ndf',
				SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
				TO FILEGROUP CrimeAndPunishment_2_filegroup
GO

-- Сделать созданную файловую группу файловой группой по умолчанию.
ALTER DATABASE CrimeAndPunishment_2
	MODIFY FILEGROUP CrimeAndPunishment_2_filegroup DEFAULT;
GO

-- 2. Создать таблицу
 
USE CrimeAndPunishment_1;
GO

CREATE SCHEMA Persons
GO
 
IF OBJECT_ID (N'Persons.Person_1') IS NOT NULL
    DROP TABLE Persons.Person_1;
GO
 
CREATE TABLE Persons.Person_1
(
	personID int PRIMARY KEY NOT NULL,
	--name       nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL,
	surname    nvarchar(50) NOT NULL
);
GO

USE CrimeAndPunishment_2;
GO

CREATE SCHEMA Persons
GO
 
IF OBJECT_ID (N'Persons.Person_2') IS NOT NULL
    DROP TABLE Persons.Person_2;
GO
 
CREATE TABLE Persons.Person_2
(
	personID int PRIMARY KEY NOT NULL,
	name       nvarchar(50) NOT NULL,
	--patronymic nvarchar(50) NULL,
	--surname    nvarchar(50) NOT NULL
);
GO
 
 USE master;
 GO

 --EXEC sp_serveroption 'OtherServer', 'lazy schema validation', 'true' --Параметр сервера "lazy schema validation" недоступен в данном выпуске SQL Server.
 --GO


IF OBJECT_ID (N'insertViewPersonSection', 'TR') IS NOT NULL
   DROP TRIGGER insertViewPersonSection;
IF OBJECT_ID (N'updateViewPersonSection', 'TR') IS NOT NULL
   DROP TRIGGER updateViewPersonSection;
IF OBJECT_ID (N'deleteViewPersonSection', 'TR') IS NOT NULL
   DROP TRIGGER deleteViewPersonSection;
GO
 IF OBJECT_ID (N'viewPersonSection') IS NOT NULL
 BEGIN
    DROP VIEW viewPersonSection;
END
GO


CREATE VIEW viewPersonSection AS
SELECT p2.*, p1.surname, p1.patronymic
 FROM CrimeAndPunishment_1.Persons.Person_1 p1, CrimeAndPunishment_2.Persons.Person_2 p2
 WHERE p1.personID = p2.personID
GO

CREATE TRIGGER insertViewPersonSection
ON viewPersonSection
INSTEAD OF INSERT
AS
BEGIN
INSERT INTO CrimeAndPunishment_1.Persons.Person_1(personID, patronymic, surname)
										SELECT personID, patronymic, surname
										FROM inserted
INSERT INTO CrimeAndPunishment_2.Persons.Person_2(personID, name)
										SELECT personID, name
										FROM inserted
END
GO

CREATE TRIGGER deleteViewPersonSection
ON viewPersonSection
INSTEAD OF DELETE
AS
BEGIN
DELETE CrimeAndPunishment_1.Persons.Person_1 WHERE personID IN (SELECT personID FROM deleted)
DELETE CrimeAndPunishment_2.Persons.Person_2 WHERE personID IN (SELECT personID FROM deleted)
END
GO

CREATE TRIGGER updateViewPersonSection
ON viewPersonSection
INSTEAD OF UPDATE
AS
BEGIN
IF UPDATE(personID) THROW 55555, N'Изменение значения personID запрещено', 0;

UPDATE CrimeAndPunishment_1.Persons.Person_1
	SET surname = inserted.surname,
	 patronymic = inserted.patronymic FROM inserted
	WHERE CrimeAndPunishment_1.Persons.Person_1.personID = inserted.personID

UPDATE CrimeAndPunishment_2.Persons.Person_2
	SET name = inserted.name FROM inserted
	WHERE CrimeAndPunishment_2.Persons.Person_2.personID = inserted.personID
END
GO



INSERT INTO viewPersonSection (	personID,  name, patronymic, surname )
                  VALUES (1, N'A1', NULL, 'B1'), (2, N'A2', NULL, 'B2'), (3, N'A3', NULL, 'B3'),
				         (4, N'A4', NULL, 'B4'), (5, N'A5', NULL, 'B5'), (6, N'A6', NULL, 'B6')
GO

-- update viewPersonSection set personID = 25 where personID = 5

SELECT * FROM viewPersonSection;
GO

UPDATE viewPersonSection SET name = N'MODIFIED',  patronymic = N'MODIFIED' WHERE personID = 3
GO

SELECT * FROM CrimeAndPunishment_1.Persons.Person_1
SELECT * FROM CrimeAndPunishment_2.Persons.Person_2
GO

DELETE FROM viewPersonSection WHERE name = N'MODIFIED'
GO

SELECT * FROM CrimeAndPunishment_1.Persons.Person_1
SELECT * FROM CrimeAndPunishment_2.Persons.Person_2
GO

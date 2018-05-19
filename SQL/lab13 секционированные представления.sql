/*
Задание 13.Создание распределенных баз данных на основе секционированных представлений
1.Создать две базы данных на одном экземпляре СУБД SQL Server 2012.
2.Создать в базах данных п.1. горизонтально фрагментированные таблицы.
3.Создать секционированные представления, обеспечивающие работу с данными таблиц
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
 
IF OBJECT_ID (N'Persons.Person') IS NOT NULL
    DROP TABLE Persons.Person;
GO
 
CREATE TABLE Persons.Person_1
(
	--personID UNIQUEIDENTIFIER DEFAULT NEWID(),
	personID int PRIMARY KEY NOT NULL
		CONSTRAINT DF_PersonID_check CHECK (personID < 4 or  personID > 6),
	name       nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL,
	surname    nvarchar(50) NOT NULL
);
GO

USE CrimeAndPunishment_2;
GO

CREATE SCHEMA Persons
GO
 
IF OBJECT_ID (N'Persons.Person') IS NOT NULL
    DROP TABLE Persons.Person;
GO
 
CREATE TABLE Persons.Person_2
(
	--personID UNIQUEIDENTIFIER DEFAULT NEWID(),
	personID int PRIMARY KEY NOT NULL
		CONSTRAINT DF_PersonID_check CHECK (personID between 4 and 6),
	name       nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL,
	surname    nvarchar(50) NOT NULL
);
GO
 
 USE master;
 GO

 --EXEC sp_serveroption 'OtherServer', 'lazy schema validation', 'true' --Параметр сервера "lazy schema validation" недоступен в данном выпуске SQL Server.
 --GO

 IF OBJECT_ID (N'viewPersonSection') IS NOT NULL
    DROP VIEW viewPersonSection;
GO

CREATE VIEW viewPersonSection AS
SELECT * FROM CrimeAndPunishment_1.Persons.Person_1
UNION ALL
SELECT * FROM CrimeAndPunishment_2.Persons.Person_2
GO


INSERT INTO viewPersonSection (	personID,  name, patronymic, surname )
                  VALUES (1, N'A1', NULL, 'B1'), (2, N'A2', NULL, 'B2'), (3, N'A3', NULL, 'B3'),
				         (4, N'A4', NULL, 'B4'), (5, N'A5', NULL, 'B5'), (6, N'A6', NULL, 'B6')
GO

update viewPersonSection set personID = 25 where personID = 5
SELECT * FROM viewPersonSection;
GO

SELECT * FROM CrimeAndPunishment_1.Persons.Person_1
SELECT * FROM CrimeAndPunishment_2.Persons.Person_2
GO
USE master;
GO
 
-- 1. Создать базу данных
IF DB_ID (N'lab1_database') IS NOT NULL
    DROP DATABASE lab1_database;
GO
 
CREATE DATABASE lab1_database
    ON ( NAME = lab1_database_data, FILENAME = 'C:\SQL\lab1_database.mdf',
         SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
    LOG ON ( NAME = lab1_database_log, FILENAME = 'C:\SQL\lab1_database_log.ldf',
             SIZE = 5, MAXSIZE = UNLIMITED, FILEGROWTH = 5 );
GO

-- 2. Создать таблицу
 
USE lab1_database;
 
IF OBJECT_ID (N'lab1_table1') IS NOT NULL
    DROP TABLE lab1_table1;
GO
 
CREATE TABLE lab1_table1
(
	ID int NOT NULL PRIMARY KEY,
	value  char(20) NOT NULL,
	value2 char(20) NOT NULL,
	value3 char(20) NULL
);
GO
 
-- 3. Добавить файловую группу и файл данных
IF OBJECT_ID('lab1_filegroup') IS NOT NULL OR OBJECT_ID (N'lab1_filegroup') IS NOT NULL
	ALTER DATABASE lab1_database 
		REMOVE FILEGROUP lab1_filegroup;
GO
 
ALTER DATABASE lab1_database
	ADD FILEGROUP lab1_filegroup;
ALTER DATABASE lab1_database
	ADD FILE ( NAME = lab1_filegroup_file, FILENAME = 'C:\SQL\lab1_filegroup_file.ndf',
				SIZE = 5, MAXSIZE = 15, FILEGROWTH = 5 )
				TO FILEGROUP lab1_filegroup

-- 4. Сделать созданную файловую группу файловой группой по умолчанию.
ALTER DATABASE lab1_database
	MODIFY FILEGROUP lab1_filegroup DEFAULT;
GO

 
 -- 5.(*) Создать еще одну произвольную таблицу
IF OBJECT_ID (N'lab1_table2') IS NOT NULL
    DROP TABLE lab1_table2;
GO
       
CREATE TABLE lab1_table2
(
	ID int NOT NULL PRIMARY KEY,
	value1 char(20) NOT NULL,
	value2 money NULL
);
GO
 
 -- 6.(*) Удалить созданную вручную файловую группу.
ALTER DATABASE lab1_database 
	MODIFY FILEGROUP [primary] DEFAULT;

ALTER DATABASE lab1_database 
	REMOVE FILE lab1_filegroup_file;
GO

DROP TABLE lab1_table2;
ALTER DATABASE lab1_database
    REMOVE FILEGROUP lab1_filegroup;
GO
 
-- 7.Создать схему, переместить в нее одну из таблиц, удалить схему.
CREATE SCHEMA lab1_schema
GO
 
ALTER SCHEMA lab1_schema 
	TRANSFER lab1_table1
GO
 
DROP TABLE lab1_schema.lab1_table1;
DROP SCHEMA lab1_schema;
GO
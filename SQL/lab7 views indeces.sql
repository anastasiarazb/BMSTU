
-- 1.������� ������������� �� ������ ����� �� ������ ������� 6.
-- 2.������� ������������� �� ������ ����� ����� ��������� ������ ������� 6.
-- 3.������� ������ ��� ����� �� ������ ������� 6, ������� � ���� �������������� ���������� ����.
-- 4.������� ��������������� �������������.

USE master;
GO
 
--  ������� ���� ������
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

-- �������� �������� ������ � ���� ������
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

-- ������� ��������� �������� ������ �������� ������� �� ���������.
ALTER DATABASE CrimeAndPunishment
	MODIFY FILEGROUP CrimeAndPunishment_filegroup DEFAULT;
GO

CREATE SCHEMA Occasion
GO
CREATE SCHEMA Persons
GO

-- ******************************FINISH CREATING DATABASE******************************
 
IF OBJECT_ID (N'Occasion.Crime') IS NOT NULL
    DROP TABLE Occasion.Crime;
GO

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
 values (N'���� � ����� ����� �������.',
		 DATETIMEFROMPARTS ( 2001, 12, 31, 23, 59, 59, 0 ))
		 --DATETIMEFROMPARTS ( 1998, 12, 31, 23, 59, 59, 0 ))
 GO

 
 -- 3. ������� ������� � ��������� ������ �� ������ ����������� ����������� ��������������.
IF OBJECT_ID (N'Persons.Person') IS NOT NULL
    DROP TABLE Persons.Person;
GO
       
CREATE TABLE Persons.Person
(
	--personID UNIQUEIDENTIFIER DEFAULT NEWID(),
	personID UNIQUEIDENTIFIER ROWGUIDCOL NOT NULL
		CONSTRAINT DF_PersonID_rowguid DEFAULT (newid()),
	name nvarchar(50) NOT NULL,
	surname nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL
);
GO

-- 4. ������� ������� � ��������� ������ �� ������ ������������������.
CREATE TABLE Occasion.SufferAttribute
(
	AttrID int PRIMARY KEY,
	suffer nvarchar(50) NOT NULL,
	compensation text NULL,

	--caseNumberPtr int NULL,
	--caseNumberPtr int NOT NULL,
	caseNumberPTR int DEFAULT ( SCOPE_IDENTITY()) NOT NULL,

	CONSTRAINT FK_SufferAttribute_Crime FOREIGN KEY (caseNumberPtr)     
		REFERENCES Occasion.Crime (caseNumber)     
		--ON DELETE CASCADE    
		--ON UPDATE CASCADE

		--ON DELETE SET NULL    --��������, ���� caseNumberPtr int NOT NULL,
		--ON UPDATE SET NULL

		ON DELETE SET DEFAULT   --��������, ���� caseNumberPtr int NOT NULL � �� ����������� �� ��������� ��������
		ON UPDATE SET DEFAULT

		--ON DELETE NO ACTION    
		--ON UPDATE NO ACTION
		
);
GO

CREATE SEQUENCE Occasion.AttributesCounter
	START WITH 1
	INCREMENT BY 1;
GO

INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'������� ������', SCOPE_IDENTITY())
--IDENT_CURRENT(N'Occasion.Crime'), SCOPE_IDENTITY(), @@IDENTITY
GO

INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'����� � ����� ������ �������', CURRENT_TIMESTAMP)
 GO

DELETE FROM Occasion.Crime WHERE caseNumber = 1;  
GO  

-- 5.������� ��� ��������� �������, � �������������� �� ��� ��������� ��������
-- �������� ��� ����������� ��������� ����������� (NO ACTION| CASCADE | SET NULL | SET DEFAULT).


-- ********************************* VIEWS AND INDECES *********************************



INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 VALUES (N'������ ��� ������� ��� ������� � �������� �����.', CURRENT_TIMESTAMP)
 GO

INSERT Occasion.SufferAttribute (AttrID, suffer, compensation, caseNumberPTR)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'������ �� ��������', N'����������� ���������.', SCOPE_IDENTITY())
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'���-����-������-�������� ���� ��������� ����� �������.', CURRENT_TIMESTAMP)
 GO

INSERT Occasion.SufferAttribute (AttrID, suffer,compensation, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'����� ������� �������.', N'����� ��������.',SCOPE_IDENTITY())
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'��� ����� ������ �� �������� ����� � ������������ �����.', CURRENT_TIMESTAMP)
 GO

INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'������ ������� �� ������� �� ���� ����� �����.', SCOPE_IDENTITY())
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'*One more test entry*', CURRENT_TIMESTAMP)
 GO

SELECT * FROM Occasion.Crime
GO
SELECT * FROM Occasion.SufferAttribute
GO

-- 1.������� ������������� �� ������ ����� �� ������ ������� 6.
IF OBJECT_ID(N'viewCrimeHarry') IS NOT NULL DROP VIEW viewCrimeHarry;
GO

CREATE VIEW viewCrimeHarry
AS
SELECT crime.caseNumber AS number, crime.caseDescription AS [description] FROM
Occasion.Crime crime
WHERE [caseDescription] like N'%�����%'
GO

SELECT * FROM viewCrimeHarry
GO

-- 2.������� ������������� �� ������ ����� ����� ��������� ������ ������� 6.

IF OBJECT_ID(N'viewAttributeCompensation') IS NOT NULL DROP VIEW viewAttributeCompensation;
GO

set ansi_nulls off
GO

--CREATE VIEW viewAttributeCompensation
--AS
--SELECT a.suffer AS suffer, a.compensation AS compensation,
--b.caseNumber as number, b.dateHappened as date
--FROM Occasion.SufferAttribute a, Occasion.Crime b
--WHERE a.compensation <> NULL AND a.AttrID = b.caseNumber
--GO

CREATE VIEW viewAttributeCompensation
AS
SELECT a.suffer AS suffer, a.compensation AS compensation,
b.caseNumber as number, b.dateHappened as date
FROM Occasion.SufferAttribute a RIGHT JOIN Occasion.Crime b ON a.caseNumberPTR = b.caseNumber
--WHERE a.compensation <> NULL
GO

SELECT * FROM viewAttributeCompensation
GO


-- 3.������� ������ ��� ����� �� ������ ������� 6, ������� � ���� �������������� ���������� ����.
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

CREATE UNIQUE NONCLUSTERED INDEX indexCrimeDates -- UNIQUE = �� ����� ���� 2-� ���������� �������� �����
        ON Occasion.Crime (dateHappened, caseNumber DESC) -- �������� --
		INCLUDE (caseDescription) -- ���������� --
ON CandP_Indeces_filegroup
GO

-- 4.������� ��������������� �������������.
set ansi_nulls on
GO
CREATE VIEW viewClosedCrimesIndexed
WITH SCHEMABINDING
AS
SELECT crime.caseNumber AS caseNumber, crime.condition AS condition
FROM Occasion.Crime crime
GO

CREATE UNIQUE CLUSTERED INDEX indexOfClosedCrimes_clustered -- UNIQUE = �� ����� ���� 2-� ���������� �������� �����
        ON viewClosedCrimesIndexed (caseNumber) -- �������� --
ON CandP_Indeces_filegroup
GO

CREATE INDEX indexOfClosedCrimes_nonclustered
ON viewClosedCrimesIndexed (condition) -- �������� --
ON CandP_Indeces_filegroup
GO

SELECT * FROM viewClosedCrimesIndexed
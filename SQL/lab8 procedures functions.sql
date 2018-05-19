
-- 1.������� �������� ���������, ������������ ������� �� ��������� ������� � ������������ ��������� ������� � ���� �������.
-- 2.�������������� �������� ��������� �.1. ����� �������, ����� ������� �������������� � ������������� �������, �������� �������� ����������� ���������������� ��������.
-- 3.������� �������� ���������, ���������� ��������� �.1., �������������� ��������� ������������� ������� � ��������� ���������, �������������� �� ������� ��� ���������� �������, ��������� ��� ����� ���������������� ��������.
-- 4.�������������� �������� ��������� �.2. ����� �������, ����� ������� ������������� � ������� ��������� �������.

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
	caseNumberPTR int NOT NULL,

--	CONSTRAINT FK_SufferAttribute_Crime FOREIGN KEY (caseNumberPtr)     
--		REFERENCES Occasion.Crime (caseNumber)     
		--ON DELETE CASCADE    
		--ON UPDATE CASCADE

		--ON DELETE SET NULL    --��������, ���� caseNumberPtr int NOT NULL,
		--ON UPDATE SET NULL

--		ON DELETE SET DEFAULT   --��������, ���� caseNumberPtr int NOT NULL � �� ����������� �� ��������� ��������
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

SET DATEFORMAT dmy;
GO

INSERT INTO Occasion.Crime (caseDescription, dateHappened, condition)
 VALUES (N'������ ��� ������� ��� ������� � �������� �����.', N'31/12/2008', 1)
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

 INSERT INTO Occasion.Crime (caseDescription, dateHappened, condition)
 values (N'��� ����� ������ �� �������� ����� � ������������ �����.', CURRENT_TIMESTAMP, 1)
 GO

Declare @last int = SCOPE_IDENTITY()
INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'������ ������� �� ������� �� ���� ����� �����.', @last)

INSERT Occasion.SufferAttribute (AttrID, suffer, caseNumberPtr)
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'��� ����� ��������� �� ������� ��������.', @last)
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'*One more test entry*', CURRENT_TIMESTAMP)
 GO

SELECT * FROM Occasion.Crime
GO
SELECT * FROM Occasion.SufferAttribute
GO

-- *********************************** ��������� � ������� ***********************************

-- 1.������� �������� ���������, ������������ ������� �� ��������� ������� � ������������ ��������� ������� � ���� �������.
IF OBJECT_ID (N'getCrimesByKeyWord', 'P') IS NOT NULL 
	DROP PROCEDURE getCrimesByKeyWord; 
GO

CREATE PROCEDURE getCrimesByKeyWord
	@dat nvarchar(50),
	@dataCursor CURSOR VARYING OUTPUT
AS
	SET @dataCursor = CURSOR STATIC FOR
		SELECT * 
		FROM Occasion.Crime src
		WHERE src.caseDescription LIKE @dat;
	OPEN @dataCursor;
GO

DECLARE @cursor_CrimesByKeyWord CURSOR;
EXEC getCrimesByKeyWord @dat = N'�%', @dataCursor = @cursor_CrimesByKeyWord OUTPUT;

PRINT N'PROCEDURE 1: getCrimesByKeyWord'
-- OPEN @cursor_CrimesByKeyWord -- �� �����, ������ ��� ������
FETCH NEXT FROM @cursor_CrimesByKeyWord; -- Perform the first fetch. ����� - ��������� ������� �� 1-� ������
WHILE (@@FETCH_STATUS = 0) 
BEGIN 
	FETCH NEXT FROM @cursor_CrimesByKeyWord
END

CLOSE @cursor_CrimesByKeyWord; 
DEALLOCATE @cursor_CrimesByKeyWord; 
GO 

-- 2.�������������� �������� ��������� �.1. ����� �������, ����� ������� �������������� � ������������� �������, �������� �������� ����������� ���������������� ��������.

--periodOfPrescription -- ��������� ����� ��������

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

IF OBJECT_ID (N'getCrimesByDateAfter', 'P') IS NOT NULL 
	DROP PROCEDURE getCrimesByDateAfter; 
GO

CREATE PROCEDURE getCrimesByDateAfter
	@oldest datetime,
	@outputCursor CURSOR VARYING OUTPUT
AS
	SET @outputCursor = CURSOR STATIC FOR
		SELECT *, expiry = Occasion.periodOfPrescriptionExpiry(src.dateHappened)
		FROM Occasion.Crime src
		WHERE src.dateHappened > @oldest;
	OPEN @outputCursor;
GO

-- ��-�� @cursor_CrimesByKeyWord ���� ������ ������ ����������, �� ��� ��������� ����������; 
DECLARE @cursor_CrimesByDate CURSOR;
EXEC getCrimesByDateAfter @oldest = N'31/12/2008', @outputCursor = @cursor_CrimesByDate OUTPUT;
PRINT N'PROCEDURE 2: getCrimesByDateAfter'
FETCH NEXT FROM @cursor_CrimesByDate; -- Perform the first fetch. ����� - ��������� ������� �� 1-� ������
WHILE (@@FETCH_STATUS = 0) 
BEGIN
	FETCH NEXT FROM @cursor_CrimesByDate
END

CLOSE @cursor_CrimesByDate; 
DEALLOCATE @cursor_CrimesByDate; 
GO

--3.������� �������� ���������, ���������� ��������� �.1.,
--�������������� ��������� ������������� ������� � ��������� ���������,
--�������������� �� ������� ��� ���������� �������, ��������� ��� ����� ���������������� ��������.

IF OBJECT_ID (N'Occasion.hasAttribs', N'FN') IS NOT NULL 
	DROP FUNCTION Occasion.isDetected; 
GO

CREATE FUNCTION Occasion.hasAttribs(@crimeID int)
	RETURNS int
	BEGIN
		DECLARE @res int;

		WITH Conjugate (AttrID)
		AS (SELECT a.AttrID
		FROM Occasion.SufferAttribute a
		WHERE a.caseNumberPTR = @crimeID)

		SELECT  @res = COUNT(AttrID) FROM Conjugate  
		RETURN @res
	END;
GO

IF OBJECT_ID (N'getChildrenCrimesByKeyWord', 'P') IS NOT NULL 
	DROP PROCEDURE getChildrenCrimesByKeyWord; 
GO

CREATE PROCEDURE getChildrenCrimesByKeyWord
	@dat nvarchar(250)
AS
	DECLARE @dataCursor cursor
	SET @dataCursor = CURSOR STATIC FOR
		SELECT 	caseNumber, caseDescription 
		FROM Occasion.Crime
		WHERE caseDescription LIKE @dat;
	OPEN @dataCursor;

	DECLARE @caseNum int
	DECLARE @description nvarchar(250)

	FETCH NEXT FROM @dataCursor INTO @caseNum, @description
	WHILE (@@FETCH_STATUS = 0) 
		BEGIN
		DECLARE @childrenNum int = Occasion.hasAttribs(@caseNum)
		    IF (@childrenNum > 1)
				PRINT N'caseNum = ' + CAST(@caseNum AS NVARCHAR(5)) + N' ' + @description +
				N' childrenNumber = ' + CAST(@childrenNum AS NVARCHAR(2))
			FETCH NEXT FROM @dataCursor INTO @caseNum, @description
		END

	CLOSE @dataCursor;
	DEALLOCATE @dataCursor;
GO

PRINT N'PROCEDURE 3: getChildrenCrimesByKeyWord'
EXEC getChildrenCrimesByKeyWord @dat = N'�%';
GO

-- 4.�������������� �������� ��������� �.2. ����� �������, ����� ������� ������������� � ������� ��������� �������.

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

IF OBJECT_ID (N'getCrimesByDateAfter_v2', 'P') IS NOT NULL 
	DROP PROCEDURE getCrimesByDateAfter_v2; 
GO

CREATE PROCEDURE getCrimesByDateAfter_v2
	@oldest datetime,
	@outputCursor CURSOR VARYING OUTPUT
AS
	SET @outputCursor = CURSOR STATIC FOR
		SELECT *
		FROM  Occasion.formTableExpiry(@oldest)
	OPEN @outputCursor;
GO

-- ��-�� @cursor_CrimesByKeyWord ���� ������ ������ ����������, �� ��� ��������� ����������; 
DECLARE @cursor_CrimesByDate CURSOR;
EXEC getCrimesByDateAfter @oldest = N'31/12/2008', @outputCursor = @cursor_CrimesByDate OUTPUT;
PRINT N'PROCEDURE 4: getCrimesByDateAfter'
FETCH NEXT FROM @cursor_CrimesByDate; -- Perform the first fetch. ����� - ��������� ������� �� 1-� ������
WHILE (@@FETCH_STATUS = 0) 
BEGIN
	FETCH NEXT FROM @cursor_CrimesByDate
END

CLOSE @cursor_CrimesByDate; 
DEALLOCATE @cursor_CrimesByDate; 
GO
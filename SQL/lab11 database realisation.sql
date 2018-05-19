/*
1. ������� ���� ������, ���������������� � ������
������������ ������ �4, ��������� ��������� �
������������ ������� 5-10 �������� SQL Server 2012:
� ����������� �������� � ���������� ����������� ���� ������; --��
� ��������� ��������� �����������;                    -- ON ACTION � �.�.
� ������������� � �������;                            -- ���� �� ���� 7
� �������� ���������, ������� � ��������;             -- ���� 8-9
2. �������� �������� ���� ������ ������ ��������������
���������� DDL (CREATE/ALTER/DROP), � ������������
������� �������������� ��������� �������:
� ���������� � ��������� �����;                                   --OK
� ���������� ����� ������;                                        -- OK
� ���������� ����������� ����������� (PRIMARY KEY, NULL/NOT
NULL/UNIQUE, CHECK � �.�.);                                        -- ��
� ����������� �������� �� ���������;                               -- �� (newid(), CREATE TABLE Occasion.Crime ... DEFAULT (0) NOT NULL)
3. � ��������������� ���� ������ ������ ���� ��� ��� ���� ������� (�
������ �������� ���� ������ ��� �������������) ������� ������� DML
���:
� ������� ������� (������� SELECT);            --OK
� ���������� ����� ������� (������� INSERT), ��� � ������� ����������������� -- OK
�������� ��������, ��� � � ������� ������� SELECT;     -- �� (gradeHistory)
� ����������� ������� (������� UPDATE); --OK
� �������� ������� (������� DELETE); --OK
4. �������, ��������� � ������ ��.2,3 ������ �������������� ���������
����������� �����:
� �������� ������������� ������� (DISTINCT); -- ����� � "����� ������� ���� �������� ������"
� �����, �������������� � ���������� ����� (�������� ����������� ��� ����� �  --��
������ / �������������);
� ���������� ������ (INNER JOIN / LEFT JOIN / RIGHT JOIN / FULL OUTER JOIN); -- OK
� ������� ������ ������� (� ��� �����, ������� NULL / LIKE / BETWEEN / IN / -- UPDATE Persons.viewPersonInvestigator SET name = N'John' WHERE personalNumber between 100 and 200;
EXISTS);
� ���������� ������� (ORDER BY - ASC, DESC);
� ����������� ������� (GROUP BY + HAVING, ������������� ������� �������������
� COUNT / AVG / SUM / MIN / MAX);
� ����������� ����������� ���������� �������� (UNION / UNION ALL / EXCEPT /
INTERSECT);
� ��������� �������.
*/

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
IF OBJECT_ID (N'Persons.�riminal')     IS NOT NULL DROP TABLE Persons.Criminal;
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
 values (N'���� � ����� ����� �������.',
		 DATETIMEFROMPARTS ( 2001, 12, 31, 23, 59, 59, 0 ))
		 --DATETIMEFROMPARTS ( 1998, 12, 31, 23, 59, 59, 0 ))
 GO

 
 -- 3. ������� ������� � ��������� ������ �� ������ ����������� ����������� ��������������.
       
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

-- ������� ������� � ��������� ������ �� ������ ������������������.
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
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'������� ������', SCOPE_IDENTITY())
GO

INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'����� � ����� ������ �������', CURRENT_TIMESTAMP)
 GO

DELETE FROM Occasion.Crime WHERE caseNumber = 1;  
GO

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

CREATE VIEW viewAttributeCompensationRIGHT -- ����������� ����� ����������� NULL
AS
SELECT a.suffer AS sufferRIGHT, a.compensation AS compensation,
b.caseNumber as number, b.dateHappened as date
FROM Occasion.SufferAttribute a RIGHT JOIN Occasion.Crime b ON a.caseNumberPTR = b.caseNumber
GO

CREATE VIEW viewAttributeCompensationLEFT -- ����������� ������ ����������� NULL
AS
SELECT a.suffer AS sufferLEFT, a.compensation AS compensation,
b.caseNumber as number, b.dateHappened as date
FROM Occasion.SufferAttribute a LEFT JOIN Occasion.Crime b ON a.caseNumberPTR = b.caseNumber
GO

CREATE VIEW viewAttributeCompensationINNER -- ����������� �������
AS
SELECT a.suffer AS sufferINNER, a.compensation AS compensation,
b.caseNumber as number, b.dateHappened as date
FROM Occasion.SufferAttribute a INNER JOIN Occasion.Crime b ON a.caseNumberPTR = b.caseNumber
GO

CREATE VIEW viewAttributeCompensationOUTER -- ����������� �������
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
GO

-- *************************** TRIGGERS ***************************
-- 1.��� ����� �� ������ ������ 2 ������� 7 ������� �������� �� �������, �������� � ����������,
-- ��� ���������� �������� ������� ���� �� ��������� ������ ������������ ������������� ������ (RAISERROR / THROW).

CREATE TRIGGER Occasion.deleteCrime
	ON  Occasion.Crime
	INSTEAD OF DELETE
	AS
	-- ������ ���������������� ��������� �� ������� ������ ���� ������ 50 000
	THROW 50000, N'��� �������� ����� - �� �������� �������', 0; -- 0 - state ���� ���� � �� �� ���������������� ������ ��������� � ���������� ������, �� ��� ������ ����������� ������ ��������� ��� ������� �������������� ����� ����������, � ����� ����� ���� ��������� ������.
GO

BEGIN TRY
	DELETE FROM Occasion.Crime WHERE dateHappened IS NULL;
	PRINT N'TRY: DELETE FROM Occasion.Crime WHERE dateHappened IS NULL;\nDone.'
END TRY
BEGIN CATCH
	PRINT N'TRY: DELETE FROM Occasion.Crime WHERE dateHappened IS NULL;\nERROR!'
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
	PRINT N'� ������� Occasion.insertCrime ��������� ' + CONVERT(nvarchar(50), @num_rows) + N' ��������';
GO

INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'����-���-���-���', CURRENT_TIMESTAMP), (N'������� �������', CURRENT_TIMESTAMP)
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


-- ���� ������� INSTEAD OF UPDATE - ������� �������� �� �����������,
-- �� � ������ ��-�� caseDescription NOT NULL �� ����������
-- ���� ������� AFTER - ���������� ������
UPDATE Occasion.Crime SET caseDescription = NULL
GO

SELECT * FROM Occasion.Crime
GO

--****************************************************************************

CREATE VIEW Persons.viewPersonInvestigator
AS
SELECT investigator.personalNumber, investigator.grade,
       person.name, person.patronymic, person.surname
FROM
Persons.Investigator investigator, Persons.Person person
WHERE investigator.FK_personID = person.personID
GO

-- ��� ������������� ��������� ������ �������� INSTEAD OF
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

INSERT INTO Persons.viewPersonInvestigator(name, patronymic, surname, personalNumber, grade)
			VALUES(N'�������', N'��������', N'������', 333, N'�����'),
				  (N'����', N'��������', N'������', 123, N'������������')
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

DELETE FROM Persons.Person WHERE Persons.Person.name       IN (SELECT name       FROM deleted)
                             AND Persons.Person.patronymic IN (SELECT patronymic FROM deleted)
							 AND Persons.Person.surname    IN (SELECT surname    FROM deleted);
-- ��������������� ������ � Persons.Investigator ��������� �������������, ��������� DELETE CASCADE
GO 

DELETE Persons.viewPersonInvestigator WHERE name LIKE N'�������'
GO

SELECT * FROM Persons.Investigator;
SELECT * FROM Persons.Person;
SELECT * FROM Persons.viewPersonInvestigator  ORDER BY personalNumber ASC;
GO
-- ---------------------------

IF OBJECT_ID('Persons.gradeHistory') IS NOT NULL DROP TABLE Persons.gradeHistory
GO

CREATE TABLE Persons.gradeHistory ( --������� ����������
new_grade nvarchar(50) NULL,
change_time datetime DEFAULT(CURRENT_TIMESTAMP),
FK_person int NULL FOREIGN KEY REFERENCES Persons.Investigator (personalNumber)
ON DELETE CASCADE
)
GO

CREATE TRIGGER  Persons.updateVeiwPersonInvestigator
ON Persons.viewPersonInvestigator
INSTEAD OF UPDATE
AS
IF UPDATE(personalNumber)
	THROW 50001, N'����� ������� ���� �������� ������', 0
UPDATE Persons.Person SET name = inserted.name,
                       surname = inserted.surname, 
				    patronymic = inserted.patronymic
		              FROM inserted
					  WHERE personID IN (SELECT DISTINCT FK_personID FROM Persons.Investigator
											WHERE inserted.personalNumber = personalNumber)
UPDATE Persons.Investigator SET grade = inserted.grade FROM inserted
	WHERE Persons.Investigator.personalNumber =  inserted.personalNumber;

INSERT INTO Persons.gradeHistory (new_grade, FK_person) SELECT grade, personalNumber FROM inserted;
GO

-- ���� update �� �������������, ������������ ������ �������� ������ ����� 0
INSERT INTO Persons.viewPersonInvestigator(name, patronymic, surname, personalNumber, grade)
			VALUES(N'A', N'�', N'������', 345, N'�����'),
				  (N'����', N'��������', N'������', 9, N'������������'),
				  (N'����', N'��������', N'�������', 56, N'������������')
GO
UPDATE Persons.viewPersonInvestigator SET name = N'John' WHERE personalNumber between 100 and 200;
-- UPDATE Persons.viewPersonInvestigator SET personalNumber = 0 WHERE NOT personalNumber = 0


GO
SELECT * FROM Persons.Investigator;
SELECT * FROM Persons.Person;
SELECT * FROM Persons.viewPersonInvestigator;
GO


-- *********************************** ��������� � ������� ***********************************

-- 8.2.�������������� �������� ��������� �.1. ����� �������, ����� ������� �������������� � ������������� �������, �������� �������� ����������� ���������������� ��������.

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

-- 8.4.�������������� �������� ��������� �.2. ����� �������, ����� ������� ������������� � ������� ��������� �������.

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



--***************��������� ����� 4**********************
/*
4. �������, ��������� � ������ ��.2,3 ������ �������������� ���������
����������� �����:
...
� ������� ������ ������� (� ��� �����, ������� NULL / LIKE / BETWEEN / IN / EXISTS);
   (-- UPDATE Persons.viewPersonInvestigator SET name = N'John' WHERE personalNumber between 100 and 200;
    --  WHERE Persons.Person.name       IN (SELECT name       FROM deleted)
	)

� ���������� ������� (ORDER BY - ASC, DESC); -- OK
� ����������� ������� (GROUP BY + HAVING, ������������� ������� �������������
� COUNT / AVG / SUM / MIN / MAX); -- OK
� ����������� ����������� ���������� �������� (UNION / UNION ALL / EXCEPT /
INTERSECT); -- OK
� ��������� �������. --OK
*/

SELECT * FROM Occasion.SufferAttribute WHERE compensation IS NULL ORDER BY AttrID DESC
SELECT grade FROM Persons.Investigator GROUP BY grade HAVING SUM(personalNumber) < 1000 --���� ������ 1000 - ������ ������

SELECT COUNT(*) as [rows num], AVG(CONVERT (float, condition)) as [avg condition], SUM(caseNumber) as [case num sum],
       MIN(dateHappened) as [min date], MAX(dateHappened) as [max date]
		 FROM Occasion.Crime
GO

/*
DELETE Persons.viewPersonInvestigator WHERE name LIKE N'�������'
...
SELECT * FROM Persons.viewPersonInvestigator  ORDER BY personalNumber ASC; */

--HAVING ��������� �������� ������� � ��������� ������������� -������ ������� ����������� ����� ������ ������� �� ������� WHERE;

IF EXISTS (SELECT * FROM Occasion.SufferAttribute)
DECLARE @num int = (SELECT COUNT(*) FROM Occasion.SufferAttribute)
PRINT N'Occasion.SufferAttribute has ' +  CONVERT(nvarchar(50), @num) + ' rows'
GO

SELECT caseDescription as [string] FROM Occasion.Crime
UNION ALL
SELECT grade as [string] FROM Persons.Investigator
GO

SELECT grade as [string] FROM Persons.Investigator
UNION --������� �������������
SELECT grade as [string] FROM Persons.Investigator
GO

SELECT * FROM Persons.Investigator
GO

SELECT grade as [string] FROM Persons.Investigator
EXCEPT
SELECT grade as [string] FROM Persons.Investigator WHERE personalNumber > 300
GO

SELECT grade as [string] FROM Persons.Investigator
INTERSECT
SELECT grade as [string] FROM Persons.Investigator
GO

SELECT * FROM Occasion.Crime WHERE caseNumber IN (SELECT AttrID FROM Occasion.SufferAttribute)
GO
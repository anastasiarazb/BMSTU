
-- ���� 7:
-- 1.������� ������������� �� ������ ����� �� ������ ������� 6.
-- 2.������� ������������� �� ������ ����� ����� ��������� ������ ������� 6.
-- ���� 9:
-- 1.��� ����� �� ������ ������ 2 ������� 7 ������� �������� �� �������, �������� � ����������,
-- ��� ���������� �������� ������� ���� �� ��������� ������ ������������ ������������� ������ (RAISERROR / THROW).
-- 2.��� ������������� ������ 2 ������� 7������� �������� �� �������, �������� � ����������,
-- �������������� ����������� ���������� �������� � ������� ��������������� ����� �������������.

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
	name       nvarchar(50) NOT NULL,
	patronymic nvarchar(50) NULL,
	surname    nvarchar(50) NOT NULL
);
GO

-- ������� ������� � ��������� ������ �� ������ ������������������.
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
VALUES (NEXT VALUE FOR Occasion.AttributesCounter, N'����� ������� �������.', N'����� ��������.', SCOPE_IDENTITY())
GO

 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'��� ����� ������ �� �������� ����� � ������������ �����.', CURRENT_TIMESTAMP)
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

-- ��� ������������� ��������� ������ �������� INSTEAD OF

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
 DELETE FROM Persons.Person WHERE Persons.Person.personID IN (SELECT personID FROM deleted)
/*
DELETE FROM Persons.Person WHERE Persons.Person.name       IN (SELECT name       FROM deleted)
                             AND Persons.Person.patronymic IN (SELECT patronymic FROM deleted)
							 AND Persons.Person.surname    IN (SELECT surname    FROM deleted);
							 */
-- ��������������� ������ � Persons.Investigator ��������� ������������� ��������� DELETE CASCADE
GO 

DELETE Persons.viewPersonInvestigator WHERE name LIKE N'�������'
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
	THROW 50001, N'����� ������� ���� �������� ������', 0
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

-- ���� update �� �������������, ������������ ������ �������� ������ ����� 0
INSERT INTO Persons.viewPersonInvestigator(name, patronymic, surname, personalNumber, grade)
			VALUES(N'A', N'�', N'������', 345, N'�����'),
				  (N'����', N'��������', N'������', 9, N'������������')
GO
UPDATE Persons.viewPersonInvestigator SET name = N'John' WHERE personalNumber < 100;
-- UPDATE Persons.viewPersonInvestigator SET personalNumber = 0 WHERE NOT personalNumber = 0


GO
SELECT * FROM Persons.Investigator;
SELECT * FROM Persons.Person;
SELECT * FROM Persons.viewPersonInvestigator;
GO
USE CrimeAndPunishment;

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

INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'���� � ����� ����� �������.',DATETIMEFROMPARTS ( 2001, 12, 31, 23, 59, 59, 0 ))
 GO
INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'����� � ����� ������ �������', CURRENT_TIMESTAMP)
 GO
INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 VALUES (N'������ ��� ������� ��� ������� � �������� �����.', CURRENT_TIMESTAMP)
 GO
 INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 values (N'���-����-������-�������� ���� ��������� ����� �������.', CURRENT_TIMESTAMP)
 GO


-- ************ 1) ��������: ������� ������; ������� ��������: Read Uncommited ************

-- WITH (NOLOCK) ��������� ��� update, insert, delete
 /*
BEGIN TRANSACTION;
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
SELECT * FROM Occasion.Crime
GO
UPDATE Occasion.Crime SET caseDescription = N'*MODIFIED*' WHERE caseNumber > 3;
SELECT * FROM Occasion.Crime;
GO
WAITFOR DELAY '00:00:10';
ROLLBACK;
GO

SELECT * FROM Occasion.Crime
GO
 */



-- �� ������ �����:
-- SELECT * FROM Occasion.Crime WITH (NOLOCK)
-- GO


-- ************ 2) ��������: ����������������� ������; ������� ��������: Read Commited ************
-- �� ��������� ����� ���� ������� ��������

-- ���� ���� ������, � ������ - "������"
/*
BEGIN TRANSACTION
--SET TRANSACTION ISOLATION LEVEL READ COMMITTED -- �� ��������� ����� ���� ������� ��������, ������ ����������
--SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; -- ������ �� ����������
SELECT * FROM Occasion.Crime 
GO
WAITFOR DELAY '00:00:10';
GO
SELECT * FROM Occasion.Crime
GO
COMMIT
*/
-- �� ������ ����� ("��������")
-- BEGIN TRANSACTION;
-- UPDATE Occasion.Crime SET caseDescription = N'*MODIFIED*' WHERE caseNumber > 3;
-- COMMIT;
-- GO

-- ******** 3) ��������: ��������� ������; ������� ��������: Repeatable Read / Serializable / Snapshot *********


-- ���� ���� ������, � ������ - "������"

--  ��� �������� � ����������� ��������� ���������� ������ ����� �� ��������� ���� ������ (tempdb). 
 /*
ALTER DATABASE CrimeAndPunishment
 SET ALLOW_SNAPSHOT_ISOLATION ON
 GO
 */
 /*
BEGIN TRANSACTION
--SET TRANSACTION ISOLATION LEVEL READ COMMITTED
-- SET TRANSACTION ISOLATION LEVEL REPEATABLE READ; -- �������� �����
 -- SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;   -- �������� ��������, ����� ����������,
                                                  -- �� �������� ��������, ��� ��� ������ ����
									-- ���� ���������� ���� ���������� (2 ���� ����� ������, ����������� �����)
 SET TRANSACTION ISOLATION LEVEL SNAPSHOT; --������ ���� ������������ ������, ��������� � ������ ���� ����� �� ��������
SELECT * FROM Occasion.Crime 
GO
WAITFOR DELAY '00:00:10';
GO
SELECT * FROM Occasion.Crime
GO
INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 VALUES (N'*NEW TEST ENTRY SERIALIZABLE*', CURRENT_TIMESTAMP)
 GO
COMMIT

*/
-- �� ������ ����� ("��������")
-- BEGIN TRANSACTION;
-- UPDATE Occasion.Crime SET caseDescription = N'*MODIFIED*' WHERE caseNumber > 3;
-- COMMIT;
-- GO

-- ***************************** 4) Deadlock *****************************

BEGIN TRANSACTION;
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
UPDATE Occasion.Crime SET caseDescription = N'*MODIFIED 1*' WHERE caseNumber > 3;
GO
WAITFOR DELAY '00:00:10';
INSERT Persons.Person(name, surname) VALUES (N'A1', N'B1') 
GO
SELECT * FROM Occasion.Crime
SELECT * FROM Persons.Person
COMMIT

--********************************************************************************************
/*
The following query will display lock information.
The value for <dbid> should be replaced with the database_id from sys.databases.
*/
/*
SELECT resource_type, resource_associated_entity_id,  
    request_status,
	request_mode, --��� ����������, � ���� ������� ������� ����� ���������� �������� �� ������ � S, U, X � ��.
	request_session_id, 
	resource_type, --��� �������. �������� ����� ���� ����� �� ���������: DATABASE, FILE, OBJECT, PAGE, KEY, EXTENT, RID, APPLICATION, METADATA, HOBT ��� ALLOCATION_UNIT. �������� �� ��� ��� � DATABASE � ���� ���� ������; OBJECT � ������� ��� ����� ������ ������; PAGE � �������� ����� ��; KEY � ��������� �� ���� �������; EXTENT � ������� ����� ��; RID � ���������� ������ � ����. 
    resource_description   
    FROM sys.dm_tran_locks  
    WHERE resource_database_id = DB_ID(N'CrimeAndPunishment')
*/


SELECT 
    CASE locks.resource_type
		WHEN N'OBJECT' THEN OBJECT_NAME(locks.resource_associated_entity_id)
		WHEN N'KEY'THEN (SELECT OBJECT_NAME(object_id) FROM sys.partitions WHERE hobt_id = locks.resource_associated_entity_id)
		WHEN N'PAGE' THEN (SELECT OBJECT_NAME(object_id) FROM sys.partitions WHERE hobt_id = locks.resource_associated_entity_id)
		WHEN N'HOBT' THEN (SELECT OBJECT_NAME(object_id) FROM sys.partitions WHERE hobt_id = locks.resource_associated_entity_id)
		WHEN N'RID' THEN (SELECT OBJECT_NAME(object_id) FROM sys.partitions WHERE hobt_id = locks.resource_associated_entity_id)
		ELSE N'Unknown'
    END AS objectName,
    CASE locks.resource_type
		WHEN N'KEY' THEN (SELECT indexes.name 
							FROM sys.partitions JOIN sys.indexes 
								ON partitions.object_id = indexes.object_id AND partitions.index_id = indexes.index_id
							WHERE partitions.hobt_id = locks.resource_associated_entity_id)
		ELSE N'Unknown'
    END AS IndexName,
    locks.resource_type,
	DB_NAME(locks.resource_database_id) AS database_name,
	locks.resource_description,
	locks.resource_associated_entity_id,
	locks.request_mode,
	locks.request_type
   -- locks.*
FROM sys.dm_tran_locks AS locks
	WHERE locks.resource_database_id = DB_ID(N'CrimeAndPunishment')
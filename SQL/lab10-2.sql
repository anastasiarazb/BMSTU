USE CrimeAndPunishment;

-- ************ 1) ��������: ������� ������; ������� ��������: Read uncommited ************
-- � ������ ����� (������ ���� "������", � ���� ������).
-- BEGIN TRANSACTION;
-- SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
-- UPDATE Occasion.Crime SET caseDescription = N'*MODIFIED*' WHERE caseNumber > 3;
-- SELECT * FROM Occasion.Crime;
-- GO
-- WAITFOR DELAY '00:00:10';
-- ROLLBACK;
-- GO

/*
--SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
SELECT * FROM Occasion.Crime WITH (NOLOCK)
GO
*/

-- ************ 2)  ��������: ����������������� ������; ������� ��������: Read commited ************
-- � ������ ����� (���� ���� "������", � ������ - ������).
 /*
SELECT * FROM Occasion.Crime
GO
BEGIN TRANSACTION;
UPDATE Occasion.Crime SET caseDescription = N'*MODIFIED*' WHERE caseNumber < 3;
COMMIT;
GO
SELECT * FROM Occasion.Crime
GO
 */

-- � ������ ����� (��������)
-- SELECT * FROM Occasion.Crime
-- GO
-- WAITFOR DELAY '00:00:10';
-- GO
-- SELECT * FROM Occasion.Crime
-- GO

-- ************ 3) ��������: ��������� ������; ������� ��������: Repeatable Read ************
/*
SELECT * FROM Occasion.Crime
GO
BEGIN TRANSACTION;
INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 VALUES (N'*NEW TEST ENTRY 1*', CURRENT_TIMESTAMP)
 GO
INSERT INTO Occasion.Crime (caseDescription, dateHappened)
 VALUES (N'*NEW TEST ENTRY 2*', CURRENT_TIMESTAMP)
 GO
COMMIT;
GO
SELECT * FROM Occasion.Crime
GO
*/

-- � ������ ����� (��������)
-- SELECT * FROM Occasion.Crime
-- GO
-- WAITFOR DELAY '00:00:10';
-- GO
-- SELECT * FROM Occasion.Crime
-- GO
--INSERT INTO Occasion.Crime (caseDescription, dateHappened)
--VALUES (N'*NEW TEST ENTRY SERIALIZABLE*', CURRENT_TIMESTAMP)
--GO


-- ***************************** 4) Deadlock *****************************

BEGIN TRANSACTION
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE
INSERT Persons.Person(name, surname) VALUES (N'A2', N'B2') 
GO
WAITFOR DELAY '00:00:10';
UPDATE Occasion.Crime SET caseDescription = N'*MODIFIED 2*' WHERE caseNumber > 3;
GO
SELECT * FROM Occasion.Crime
SELECT * FROM Persons.Person
COMMIT
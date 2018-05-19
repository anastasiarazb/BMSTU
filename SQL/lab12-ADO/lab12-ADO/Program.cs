using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Threading.Tasks;
using System.Data.SqlClient; //SqlConnection, SqlCommand 
using System.Data;           //определение общих типов, не связанных с конкретным источником данных -DataSet, DataTable, DataRow, IDataAdapter, IDataReader, IDataParameter, IDbTransaction etc. SqlDbType 
using System.Configuration; //ConfigurationManager

namespace lab12_ADO
{
    class CrimeAndPunishment
    {
        //Просмотр содержимого таблицы
        static void ShowDBTable(SqlConnection connection, string tablename)
        {
            try
            {
                Console.WriteLine(" ");
                Console.WriteLine("ShowDBTable():");
                SqlCommand command = new SqlCommand();
                command.Connection = connection;
                // Связный уровень
                // вызов метода ExecuteReader() объекта DataReader для получения результатов запроса;
                command.CommandText = "SELECT * FROM " + tablename;
                SqlDataReader resultReader = command.ExecuteReader();

                while (resultReader.Read())
                {
                    Console.WriteLine("{");
                    for (int i = 0; i < resultReader.FieldCount; ++i)
                    {
                        Console.Write("    " + resultReader.GetName(i) + " = " + resultReader.GetValue(i) + "\n");
                    }
                    Console.WriteLine("}");
                }
                resultReader.Close();
            }
            catch (Exception exp)
            {
                Console.WriteLine(exp.Message);
                Console.ReadKey();
                throw exp;
            }
        }

        //Удаление
        static void deleteCrimeLike(SqlConnection connection, String like)
        {
            try
            {// Связный уровень
                SqlCommand command = new SqlCommand();
                command.CommandText = "DELETE Occasion.Crime WHERE caseDescription LIKE @likeCondition";
                command.Connection = connection;
                SqlParameter param = new SqlParameter
                {
                    ParameterName = "@likeCondition",
                    Value = like,
                    SqlDbType = SqlDbType.NVarChar,
                    Size = 50
                };
                command.Parameters.Add(param);
                command.ExecuteNonQuery(); //Запрос не возвращает результат
            }
            catch (Exception exp)
            {
                Console.WriteLine(exp.Message);
                Console.ReadKey();
                throw exp;
            }
        }

        //Вставка
        static void insertCrime(SqlConnection connection, DateTime dateHappened, String caseDescription, bool condition = false)
        {
            try
            {// Связный уровень
                SqlCommand command = new SqlCommand();
                command.CommandText = "INSERT INTO Occasion.Crime(caseDescription, dateHappened, condition) VALUES (@caseDescription, @dateHappened, @condition)";
                command.Connection = connection;
                SqlParameter[] p = new SqlParameter[3];
                //SqlParameter param = new SqlParameter(); ... command.Parameters.Add(param);

                p[0] = new SqlParameter();
                p[0].ParameterName = "@dateHappened";
                p[0].Value = dateHappened;
                p[0].SqlDbType = SqlDbType.DateTime;

                p[1] = new SqlParameter();
                p[1].ParameterName = "@caseDescription";
                p[1].Value = caseDescription;
                p[1].SqlDbType = SqlDbType.NVarChar;
                p[1].Size = 50;

                p[2] = new SqlParameter();
                p[2].ParameterName = "@condition";
                p[2].Value = condition;
                p[2].SqlDbType = SqlDbType.Bit;

                command.Parameters.AddRange(p);
                command.ExecuteNonQuery(); //Запрос не возвращает результат
            }
            catch (Exception exp)
            {
                Console.WriteLine(exp.Message);
                Console.ReadKey();
                throw exp;
            }
        }

        //Обновление
        static void updateCrime(SqlConnection connection, DateTime dateHappened, String caseDescription, bool newCondition = false)
        {
            try
            {// Связный уровень

                String commandText = "UPDATE Occasion.Crime SET condition = @newCondition " +
                                     "WHERE caseDescription LIKE @caseDescription AND dateHappened = @dateHappened";
                SqlCommand command = new SqlCommand(commandText, connection);
                command.Parameters.Add("@newCondition",    SqlDbType.Bit);
                command.Parameters.Add("@caseDescription", SqlDbType.NVarChar, 50);
                command.Parameters.Add("@dateHappened",    SqlDbType.DateTime);
                command.Parameters["@newCondition"].SqlValue    = newCondition;
                command.Parameters["@caseDescription"].SqlValue = caseDescription;
                command.Parameters["@dateHappened"].SqlValue    = dateHappened;
                command.ExecuteNonQuery(); //Запрос не возвращает результат
            }
            catch (Exception exp)
            {
                Console.WriteLine(exp.Message);
                Console.ReadKey();
                throw exp;
            }
        }

        static public void showDifference(SqlConnection connection)
        {
            Console.WriteLine("Связный уровень - таблица");
            ShowDBTable(connection, "Occasion.Crime");
            Console.WriteLine("Несвязный уровень - таблица");

            SqlDataAdapter adapterCrime = new SqlDataAdapter("SELECT * FROM Occasion.Crime", connection);

            SqlCommand insert_command = new SqlCommand();
            insert_command.Connection = connection;
            insert_command.CommandText = "INSERT INTO Occasion.Crime(caseDescription, dateHappened, condition) "
                                        + "VALUES (@caseDescription, @dateHappened, @condition)";
            //Параметр, тип, максимальное кол-во памяти, которое можно выделить (важно для динамических типов вроде строк), столбец
            insert_command.Parameters.Add("@caseDescription", SqlDbType.NVarChar, 50, "caseDescription");
            insert_command.Parameters.Add("@condition", SqlDbType.Bit, 0, "condition");
            adapterCrime.InsertCommand = insert_command;

            SqlCommand delete_command = new SqlCommand();
            delete_command.Connection = connection;
            delete_command.CommandText = "DELETE Occasion.Crime WHERE caseNumber = @caseNumber";
            delete_command.Parameters.Add("@caseNumber", SqlDbType.Int, 0, "caseNumber");
            adapterCrime.DeleteCommand = delete_command;

            SqlCommand update_command = new SqlCommand();
            update_command.CommandText = "UPDATE Occasion.Crime SET condition = @newCondition " +
                                 "WHERE caseDescription LIKE @caseDescription AND dateHappened = @dateHappened";
            update_command.Connection = connection;
            update_command.Parameters.Add("@newCondition", SqlDbType.Bit, 0, "condition");
            update_command.Parameters.Add("@dateHappened", SqlDbType.DateTime, 0, "dateHappened");
            update_command.Parameters.Add("@caseDescription", SqlDbType.NVarChar, 50, "caseDescription");
            update_command.Parameters["@dateHappened"].SourceVersion = DataRowVersion.Original;
            update_command.Parameters["@caseDescription"].SourceVersion = DataRowVersion.Original;
            adapterCrime.UpdateCommand = update_command;

            DataSet data_set = new DataSet();
            //Загрузка локальной копии данных в DataSet согласно SelectCommand, указанной при открытии адаптера
            adapterCrime.Fill(data_set, "Crimes");
            //Задание новой строки
            DataRow newRow = data_set.Tables["Crimes"].NewRow();
            newRow["condition"] = false;
            newRow["dateHappened"] = DateTime.Now;
            newRow["caseDescription"] = "*Вставлено с помощью ADO - несвязный уровень 1*";
            data_set.Tables["Crimes"].Rows.Add(newRow);
            adapterCrime.Update(data_set, "Crimes"); //Фиксация изменений
            ShowDBTable(connection, "Occasion.Crime");
            //RowState = Modified
            newRow["condition"] = true;
            adapterCrime.Update(data_set, "Crimes"); //Фиксация изменений
            ShowDBTable(connection, "Occasion.Crime");

            //Удаление строки в локальной копии, а затем и в базе данных
            Console.WriteLine("Первая запись была:\n" + data_set.Tables["Crimes"].Rows[0]["caseDescription"]);
            data_set.Tables["Crimes"].Rows[0].Delete();
            adapterCrime.Update(data_set, "Crimes");

            Console.WriteLine("Первая запись стала:\n" + data_set.Tables["Crimes"].Rows[0]["caseDescription"]);


        }

        //Вызов хранимой процедуры
        static public void getCrimesByDateAfter(SqlConnection connection, DateTime date)
        {
             try
            {
                SqlCommand command = new SqlCommand("getCrimesByDateAfter", connection);
                //Указать, как интерпретировать строку. По умолчанию - текст
                command.CommandType = System.Data.CommandType.StoredProcedure;
                // Связный уровень
                SqlParameter param = new SqlParameter
                {
                    ParameterName = "@oldest",
                    Value = date,
                    Direction = ParameterDirection.Input //Для Output result = (type)cmd.Parameters[<Parametername>].Value;
                   // SqlDbType = SqlDbType.DateTime
                };
                command.Parameters.Add(param);
                //Можно обращаться к уже добавленному параметру по имени
                command.Parameters["@oldest"].SqlDbType = SqlDbType.DateTime;

                SqlDataReader resultReader = command.ExecuteReader();

                while (resultReader.Read())
                {
                    Console.WriteLine("getCrimesByDateAfter()\n{");
                    //Можно получать значение по индексу или по имени столбца
                    Console.WriteLine("     caseDescription: " + resultReader["caseDescription"]);
                    Console.WriteLine("     dateHappened =   " + resultReader["dateHappened"]);
                    Console.WriteLine("     expiry       =   " + resultReader["expiry"]);
                    Console.WriteLine("}");
                }
                resultReader.Close();
            }
            catch (Exception exp)
            {
                Console.WriteLine(exp.Message);
                Console.ReadKey();
                throw exp;
            }
        }

        static public void testQuery()
        {
            try
            {
                //<add name="ADO_query" из App.config
                string connectionString = ConfigurationManager.ConnectionStrings["ADO_query"].ConnectionString;
                using (SqlConnection connection = new SqlConnection(connectionString))
                {
                    connection.Open();
                    Console.WriteLine("Connection opened");

                   insertCrime(connection, new DateTime(2008, 12, 31), "*Вставлено с помощью ADO 1*", true);
                   insertCrime(connection, new DateTime(2012, 6, 13), "*Вставлено с помощью ADO 2*", false);
                   insertCrime(connection, new DateTime(2012, 6, 13), "*Вставлено с помощью ADO 3*"); //false

                   updateCrime(connection, new DateTime(2012, 6, 13), "*Вставлено с помощью ADO 2*", true);
                   ShowDBTable(connection, "Occasion.Crime");

                   Console.WriteLine("Отобразить все записи, позднее заданного года.\nВведите год:");
                   int year = Int32.Parse(Console.ReadLine());
                   getCrimesByDateAfter(connection, new DateTime(year, 01, 01));

                   Console.WriteLine("Удалить записи, в которых описание соответствует следующей форматной строке.\n" +
                                     "Введите строку (без кавычек, например, К% или %1%):");
                   String deleteCondition = Console.ReadLine();
                   deleteCrimeLike(connection, deleteCondition);

                   ShowDBTable(connection, "Occasion.Crime");

                   showDifference(connection);
                   deleteCrimeLike(connection, "*Вставлено с помощью ADO %");
                    connection.Close();
                }
            }
            catch (Exception exp)
            {
                Console.WriteLine(exp.Message);
                Console.ReadKey();
                throw exp;
            }
        }
    }
    class Program {
        static void Main(string[] args) {
            CrimeAndPunishment.testQuery();
            Console.ReadKey();
        }
    }
}

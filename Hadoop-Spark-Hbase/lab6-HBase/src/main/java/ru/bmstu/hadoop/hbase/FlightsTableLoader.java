package ru.bmstu.hadoop.hbase;

import ru.bmstu.hadoop.hbase.model.*;
import java.io.FileReader;
import java.io.IOException;
import java.io.BufferedReader;

import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.TableName;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.util.Bytes;

public class FlightsTableLoader {
    public static final String PATH_TO_FILE = "/home/nastasia/5sem/Hadoop/Labs/lab6/664600583_T_ONTIME_sample.csv";
    public static final String TABLE_NAME   = "flights2";
    public static final String COLUMN_FAMILY_NAME = "info";

    public static void main(String[] args) throws IOException {
        Configuration config = HBaseConfiguration.create();
        config.set("hbase.zookeeper.quorum", "localhost");

        Connection connection = ConnectionFactory.createConnection();

        Table flights = connection.getTable(TableName.valueOf(TABLE_NAME));

        BufferedReader reader = new BufferedReader(new FileReader(PATH_TO_FILE));
        String[] header = reader.readLine().replace("\"", "").split(",");

        String nextLine;

        int entityNumber = 0;
        int dateColumn      = FlightTable.columnNum(header, FlightTable.FL_DATE);
        int airlineIDColumn = FlightTable.columnNum(header, FlightTable.AIRLINE_ID);
        while ((nextLine = reader.readLine()) != null) {
            String[] row = nextLine.split(",");
            Put put = new Put(Bytes.toBytes(
                    row[dateColumn] + " "+ row[airlineIDColumn] + " " + String.valueOf(entityNumber)
            ));
            for (int j = 0; j < header.length; ++j) {
                put.addColumn(Bytes.toBytes(COLUMN_FAMILY_NAME), Bytes.toBytes(header[j]), Bytes.toBytes(row[j]));
            }
            put.addColumn(Bytes.toBytes(COLUMN_FAMILY_NAME),
                          Bytes.toBytes("index"),
                          Bytes.toBytes(String.valueOf(entityNumber)));
            flights.put(put);
            ++entityNumber;
        }

        flights.close();
        reader.close();
        connection.close();
    }
}

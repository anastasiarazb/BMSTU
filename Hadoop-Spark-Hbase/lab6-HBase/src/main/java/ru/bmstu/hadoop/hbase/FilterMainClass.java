package ru.bmstu.hadoop.hbase;

import ru.bmstu.hadoop.hbase.model.*;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.util.Bytes;

import java.io.IOException;

public class FilterMainClass {

    private static final byte[] START_ROW = Bytes.toBytes("2015-01-05");
    private static final byte[] STOP_ROW  = Bytes.toBytes("2015-01-10s");

    private static final float CUSTOM_DELAY_TIME = 10.0f;

    public static void main(String[] args) throws IOException {
        Configuration config = HBaseConfiguration.create();
        config.set("hbase.zookeper.quorum","localhost");

        Connection connection = ConnectionFactory.createConnection();
        Table flights = connection.getTable(TableName.valueOf(FlightsTableLoader.TABLE_NAME));

        Scan scan = new Scan();
        scan.addFamily(Bytes.toBytes(FlightsTableLoader.COLUMN_FAMILY_NAME));
        scan.setFilter(new CancelDelayFlightFilter(CUSTOM_DELAY_TIME));
        scan.setStartRow(START_ROW);
        scan.setStopRow(STOP_ROW);

        byte[] colFamily    = Bytes.toBytes(FlightsTableLoader.COLUMN_FAMILY_NAME);
        byte[] delayColumn  = Bytes.toBytes(FlightTable.ARR_DELAY_NEW);
        byte[] cancelColumn = Bytes.toBytes(FlightTable.CANCELLED);

        ResultScanner scanner = flights.getScanner(scan);
        for (Result res: scanner) {
            String key       = Bytes.toString(res.getRow());
            String delay     = Bytes.toString(res.getValue(colFamily, delayColumn));
            String cancelled = Bytes.toString(res.getValue(colFamily, cancelColumn));

            System.out.println("Key:" + key + "; Delay:" + delay + "; Cancelled:" + cancelled);
        }

        scanner.close();
        flights.close();
        connection.close();
    }
}

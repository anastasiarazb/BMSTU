package ru.bmstu.labs.hadoop.lab2;

import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import java.io.IOException;
import java.lang.String;

//KEYIN,VALUEIN,KEYOUT,VALUEOUT
public class FlightMapper extends Mapper<LongWritable, Text, FlightKey, Text> {

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        String src = value.toString();
        String[] lines = src.split("\n");
//        FlightKey.initColumns(lines[0]);

        for (String record: lines) {
            if (record.startsWith("\"")) { //не строка с данными, а шапка
                return;
            }
            FlightKey k = new FlightKey(record);
            if (!k.arrivedInTime()) {
                context.write(k, new Text(record));
            }
        }
    }
}

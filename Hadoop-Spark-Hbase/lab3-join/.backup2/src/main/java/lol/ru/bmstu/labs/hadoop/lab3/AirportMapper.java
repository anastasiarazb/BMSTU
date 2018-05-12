package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;

public class AirportMapper extends Mapper<LongWritable, Text, AirportWritableComparable, Text> {
    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException{
        String line = value.toString();
        if (line.contains("Code")) {
            return;
        }

        AirportWritable writable = new AirportWritable(line);
        AirportWritableComparable comparable = new AirportWritableComparable(writable, 0);

        context.write(comparable, new Text(writable.getAirportName()));
    }
}

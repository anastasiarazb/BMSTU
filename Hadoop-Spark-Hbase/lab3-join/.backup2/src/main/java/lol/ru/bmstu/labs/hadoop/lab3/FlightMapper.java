package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;

public class FlightMapper extends Mapper<LongWritable, Text, AirportWritableComparable, Text> {
    @Override
    protected void map(LongWritable key, Text value, Context context)throws IOException, InterruptedException{
        String[] lines = value.toString().split("\n");

        for (String line: lines) {

            if (line.contains("YEAR")) {
                return;
            }

            FlightWritable writable = new FlightWritable(line);
            AirportWritableComparable comparable = new AirportWritableComparable(writable, 1);

            if (writable.getDelay() <= 0) {
                return;
            }

            context.write(comparable, new Text(Float.toString(writable.getDelay())));
        }
    }
}

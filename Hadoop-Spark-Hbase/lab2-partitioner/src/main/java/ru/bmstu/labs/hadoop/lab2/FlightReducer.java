package ru.bmstu.labs.hadoop.lab2;

import org.apache.hadoop.mapreduce.Reducer;
import java.io.IOException;
import org.apache.hadoop.io.Text;

//KEYIN,VALUEIN,KEYOUT,VALUEOUT
public class FlightReducer extends Reducer<FlightKey, Text, FlightKey, Text> {
    @Override
    protected void reduce(FlightKey key, Iterable<Text> values, Context context)
            throws IOException, InterruptedException {
        for (Text val: values) {
            context.write(key, val);
        }
    }
}

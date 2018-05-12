package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import ru.bmstu.labs.hadoop.lab3.model.AirportsTableRec;

import java.io.IOException;

//KEYIN,VALUEIN,KEYOUT,VALUEOUT
public class AirportIDMapper extends Mapper<LongWritable, Text, JoinedAirportFlightKey, Text> {

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        Debug.set(false);
        Debug.out("[AirportIDMapper]: receiving lines...");
        String src = value.toString();
        String[] lines = src.split("\n");

        Debug.out("[AirportIDMapper]: lines are got");

        for (String line: lines) {
            AirportsTableRec record = new AirportsTableRec(line);
            if (record.isValidRecord()) {
                //JoinedAirportFlightKey(airportID, Type=Airport), Text(airportName)
                context.write(record.key(), record.value());
            }
        }
    }
}


/**
 * Created by nastasia on 30.10.16.
 */

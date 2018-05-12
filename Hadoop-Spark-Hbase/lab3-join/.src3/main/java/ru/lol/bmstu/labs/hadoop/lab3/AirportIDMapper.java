package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import java.io.IOException;

public class AirportIDMapper extends Mapper<LongWritable, Text, JoinedAirportFlightKey, Text> {
    private class AirportTableRec {
        private int airportID;
        private String airportName;

        public AirportTableRec() {}

        AirportTableRec(String str){
            str = str.replace("\"", "");
            String[] arr = str.split(",");
            airportID = Integer.parseInt(arr[0]);
            airportName = arr[1];
        }

        public int getAirportID(){
            return airportID;
        }

        //Writable
        Text getAirportName() {
            return new Text(airportName);
        }
    }
    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException{
        String[] lines = value.toString().split("\n");
        for (String line: lines) {

            if (line.contains("Code")) {
                return;
            }

            AirportTableRec record = new AirportTableRec(line);
            JoinedAirportFlightKey joinedKey = new JoinedAirportFlightKey(record.getAirportID(), JoinedAirportFlightKey.AIRPORT);

            context.write(joinedKey, record.getAirportName());
        }
    }
}

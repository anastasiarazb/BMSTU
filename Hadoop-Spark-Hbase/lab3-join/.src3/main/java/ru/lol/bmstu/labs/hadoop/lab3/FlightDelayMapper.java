package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import java.io.IOException;

public class FlightDelayMapper extends Mapper<LongWritable, Text, JoinedAirportFlightKey, Text> {

    private class AirportsTableRec{
        private float delay;
        private int airportID;

        public AirportsTableRec() {

        }

        AirportsTableRec(String str){
            str = str.replace("\"", "");
            String[] arr = str.split(",");
            if (arr[17].isEmpty()) {
                delay = 0;
            } else {
                delay = Float.parseFloat(arr[17]);
            }
            airportID = Integer.parseInt(arr[14]);
        }

        public int getAirportID() {
            return airportID;
        }

        float getDelay() {
            return delay;
        }

        //Writable
        Text getDelayWritable() {
            return new Text(String.valueOf(delay));
        }
    }

    @Override
    protected void map(LongWritable key, Text value, Context context)throws IOException, InterruptedException{
        String[] lines = value.toString().split("\n");

        for (String line: lines) {

            if (line.contains("YEAR")) {
                return;
            }

            AirportsTableRec record = new AirportsTableRec(line);
            JoinedAirportFlightKey joinedKey = new JoinedAirportFlightKey(record.getAirportID(), JoinedAirportFlightKey.FLIGHT);

            if (record.getDelay() <= 0) {
                return;
            }

            context.write(joinedKey, record.getDelayWritable());
        }
    }
}

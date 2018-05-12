package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;

import java.io.IOException;
import java.lang.String;

//KEYIN,VALUEIN,KEYOUT,VALUEOUT
public class FlightDelayMapper extends Mapper<LongWritable, Text, JoinedAirportFlightKey, Text> {

    private class FlightsTableRec {

        private boolean validRec;
        private boolean isDelayed;
        private int destAirportID; //Часть Key
        private String arrivalDelayNew; //Value

        //Номера колонок, отвечающих извлекаемым значениям
        private static final int AIRPORT_ID_COLUMN = 14;
        private static final int ARR_DELAY_NEW_COLUMN = 18;

        private void getDelay(String src)
        {
            if (src.isEmpty()) {
                isDelayed = false; //Рейс был отменен, а не задержан
                arrivalDelayNew = null;
            }
            else {
                arrivalDelayNew = src;
                float delayTime = Float.valueOf(arrivalDelayNew);
                isDelayed = delayTime > 0.0;
            }
        }

        private void getAirportID(String line) {
            destAirportID = Integer.valueOf(line);
        }

        FlightsTableRec(String line) {
            String[] columns = line.split(",");
            validRec = !line.startsWith("\"YEAR\"");
            if (validRec) {
                getDelay(columns[ARR_DELAY_NEW_COLUMN]);
                getAirportID(columns[AIRPORT_ID_COLUMN]);
            }
        }

        boolean isDelayed() {
            return validRec && isDelayed;
        }

        Text value() {
            return new Text(arrivalDelayNew);
        }

        JoinedAirportFlightKey key() {
            return new JoinedAirportFlightKey(destAirportID, JoinedAirportFlightKey.FLIGHT);
        }
    }

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        Debug.set(false);
        Debug.out("[FlightDelayMapper]: receiving lines...");

        String src = value.toString();
        String[] lines = src.split("\n");

        Debug.out("[FlightDelayMapper]: lines are got");

        for (String line: lines) {
            FlightsTableRec record = new FlightsTableRec(line);
            if (record.isDelayed()) {
                Debug.out("[FlightDelayMapper]: key = " + record.key().toString() +
                        "; value = " + record.value().toString());
                //JoinedAirportFlightKey(destAirportID, FLIGHT), Text(delayTime);
                context.write(record.key(), record.value());
            }
        }
    }
}

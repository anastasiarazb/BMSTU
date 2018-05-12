package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.mapreduce.Reducer;
import java.io.IOException;
import java.util.Iterator;

import org.apache.hadoop.io.Text;

//KEYIN,VALUEIN,KEYOUT,VALUEOUT
public class FlightReducer extends Reducer<JoinedAirportFlightKey, Text, Text, Text> {
    @Override
    protected void reduce(JoinedAirportFlightKey key, Iterable<Text> values, Context context)
            throws IOException, InterruptedException {

        Iterator<Text> iterator = values.iterator();
        if (!iterator.hasNext()) {
            return;
        }

        String airportName = iterator.next().toString();
//        Text airportName = iterator.next();

        long counter = 0;
        float sum = 0.0f;
        float valueMin = Float.MAX_VALUE;
        float valueMax = 0.0f;

        while(iterator.hasNext()) {
            Text elem = iterator.next();
            Float value = Float.parseFloat(elem.toString());

            sum += value;
            valueMax = Math.max(valueMax, value);
            valueMin = Math.min(valueMin, value);

            counter++;
        }

        if (counter == 0) {
            return;
        }

        float average = sum / (float)counter;
        String outValue = "Average: " + String.valueOf(average) + " max: " + String.valueOf(valueMax) +
                " min: " + String.valueOf(valueMin);

        context.write(new Text(airportName), new Text(outValue));
//        context.write(airportName, new Text(outValue));
    }
}

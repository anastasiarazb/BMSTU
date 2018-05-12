package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;
import java.lang.Math;
import java.io.IOException;
import java.util.Iterator;

public class AirportReducer extends Reducer<AirportWritableComparable, Text, Text, Text>{
    @Override
    protected void reduce(AirportWritableComparable key, Iterable<Text> values, Context context) throws IOException, InterruptedException{
        Iterator<Text> iterator = values.iterator();
        if (!iterator.hasNext()) {
            return;
        }

        String outKey = iterator.next().toString();

        long counter = 0;
        Float sum = 0.0f;
        Float valueMin = Float.MAX_VALUE;
        Float valueMax = Float.MIN_VALUE;

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

        Float average = sum / counter;
        String outValue = "Average: " + average.toString() + " max: " + valueMax.toString() +
                " min: " + valueMin.toString();

        context.write(new Text(outKey), new Text(outValue));
    }
}

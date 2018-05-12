/**
 * Created by nastasia on 02.10.16.
 */
package ru.bmstu.labs.hadoop.lab2;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class FlightPartitioner extends Partitioner<FlightKey, Text> {
    public int getPartition(FlightKey key, Text value, int numPartitions) {
        return (key.firstPartHashCode() & Integer.MAX_VALUE) % numPartitions;
    }
}

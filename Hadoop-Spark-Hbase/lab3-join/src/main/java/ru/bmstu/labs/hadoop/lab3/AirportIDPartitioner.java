package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class AirportIDPartitioner extends Partitioner<JoinedAirportFlightKey, Text> {
    public int getPartition(JoinedAirportFlightKey key, Text value, int numPartitions) {
//        return (key.firstPartHashCode() & Integer.MAX_VALUE) % numPartitions;
        return key.airportID() % numPartitions;
    }
}

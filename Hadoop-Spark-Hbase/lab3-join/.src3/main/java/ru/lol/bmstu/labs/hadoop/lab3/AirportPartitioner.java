package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class AirportPartitioner extends Partitioner<JoinedAirportFlightKey, Text> {
    @Override
    public int getPartition(JoinedAirportFlightKey comparable, Text text, int numPartitions) {
        return comparable.getAirportID() % numPartitions;
    }
}

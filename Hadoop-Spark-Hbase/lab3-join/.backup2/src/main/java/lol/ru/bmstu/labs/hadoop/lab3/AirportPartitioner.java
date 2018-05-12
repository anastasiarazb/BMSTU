package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Partitioner;

public class AirportPartitioner extends Partitioner<AirportWritableComparable, Text> {
    @Override
    public int getPartition(AirportWritableComparable comparable, Text text, int numPartitions) {
        return comparable.getAirportID() % numPartitions;
    }
}

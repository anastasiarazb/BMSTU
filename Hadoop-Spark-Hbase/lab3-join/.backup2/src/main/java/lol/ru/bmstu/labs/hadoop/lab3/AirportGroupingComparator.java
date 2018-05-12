package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;

public class AirportGroupingComparator extends WritableComparator {
    protected AirportGroupingComparator(){
        super(AirportWritableComparable.class, true);
    }

    @Override
    public int compare(WritableComparable a1, WritableComparable b1){
        AirportWritableComparable a = (AirportWritableComparable)a1;
        AirportWritableComparable b = (AirportWritableComparable)b1;
        return (a.getAirportID() - b.getAirportID());
    }
}

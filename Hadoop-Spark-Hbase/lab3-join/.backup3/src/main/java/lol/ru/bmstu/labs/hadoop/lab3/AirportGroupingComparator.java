package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;

public class AirportGroupingComparator extends WritableComparator {
    protected AirportGroupingComparator(){
        super(JoinedAirportFlightKey.class, true);
    }

    @Override
    public int compare(WritableComparable a1, WritableComparable b1){
        JoinedAirportFlightKey a = (JoinedAirportFlightKey)a1;
        JoinedAirportFlightKey b = (JoinedAirportFlightKey)b1;
        return (a.getAirportID() - b.getAirportID());
    }
}

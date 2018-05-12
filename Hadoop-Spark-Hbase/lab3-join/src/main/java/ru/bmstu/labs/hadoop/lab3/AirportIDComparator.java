package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.WritableComparator;
import org.apache.hadoop.io.WritableComparable;

/* Разрабатываем GroupingComparatorClass, который учитывает только код
аэропорта */

public class AirportIDComparator extends WritableComparator {

    protected AirportIDComparator() {

        super(JoinedAirportFlightKey.class, true);

    }

    @Override
    public int compare(WritableComparable a, WritableComparable b) {

        JoinedAirportFlightKey keyA = (JoinedAirportFlightKey) a;

        JoinedAirportFlightKey keyB = (JoinedAirportFlightKey) b;

        return keyA.airportID() - keyB.airportID();

    }

}
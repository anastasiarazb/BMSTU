package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.WritableComparable;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class JoinedAirportFlightKey implements WritableComparable<JoinedAirportFlightKey> {

    final static int AIRPORT = 0;
    final static int FLIGHT = 1;


    private int airportID;
    private int dataType;

    public JoinedAirportFlightKey(){}

    public JoinedAirportFlightKey(int airportID, int dataType){
        this.dataType = dataType;
        this.airportID = airportID;
    }

    @Override
    public int compareTo(JoinedAirportFlightKey b) {
        if (airportID != b.airportID) {
            return airportID - b.airportID;
        }
        return dataType - b.dataType;
    }

    @Override
    public void write(DataOutput dataOutput) throws IOException {
        dataOutput.writeInt(airportID);
        dataOutput.writeInt(dataType);
    }

    @Override
    public void readFields(DataInput dataInput) throws IOException {
        airportID = dataInput.readInt();
        dataType = dataInput.readInt();
    }

    public int getAirportID() {
        return airportID;
    }

    private String dataTypeToString() {
        return (dataType == AIRPORT) ? "AIRPORT" : "FLIGHT";
    }

    @Override
    public String toString() {
        return "<AirportID = " + String.valueOf(airportID) + "; RecordType = " + dataTypeToString() + ">";
    }
}

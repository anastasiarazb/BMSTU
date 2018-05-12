package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.WritableComparable;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AirportWritableComparable implements WritableComparable<AirportWritableComparable> {
    private int m_airportID;
    private int m_dataID;

    public AirportWritableComparable(){}

    public AirportWritableComparable(FlightWritable writable, int dataID){
        m_dataID = dataID;
        m_airportID = writable.getAirportID();
    }

    public AirportWritableComparable(AirportWritable writable, int dataID){
        m_dataID = dataID;
        m_airportID = writable.getAirportID();
    }

    @Override
    public int compareTo(AirportWritableComparable b) {
        if (m_airportID != b.m_airportID) {
            return m_airportID - b.m_airportID;
        }
        return m_dataID - b.m_dataID;
    }

    @Override
    public void write(DataOutput dataOutput) throws IOException {
        dataOutput.writeInt(m_airportID);
        dataOutput.writeInt(m_dataID);
    }

    @Override
    public void readFields(DataInput dataInput) throws IOException {
        m_airportID = dataInput.readInt();
        m_dataID = dataInput.readInt();
    }

    public int getAirportID() {
        return m_airportID;
    }
}

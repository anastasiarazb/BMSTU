package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.Writable;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AirportWritable implements Writable {
    int m_airportID;
    String m_airportName;

    public AirportWritable(){}

    public AirportWritable(String str){
        str = str.replace("\"", "");
        String[] arr = str.split(",");
        m_airportID = Integer.parseInt(arr[0]);
        m_airportName = arr[1];
    }

    @Override
    public void write(DataOutput dataOutput) throws IOException {
        dataOutput.writeInt(m_airportID);
        dataOutput.writeChars(m_airportName);
    }

    @Override
    public void readFields(DataInput dataInput) throws IOException {
        m_airportID = dataInput.readInt();
        m_airportName = dataInput.readLine();
    }

    public int getAirportID(){
        return m_airportID;
    }

    public String getAirportName(){
        return m_airportName;
    }
}

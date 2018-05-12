package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.io.Writable;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class FlightWritable implements Writable {
    private float m_delay;
    private int m_airportID;

    public FlightWritable(){}

    public FlightWritable(String str){
        str = str.replace("\"", "");
        String[] arr = str.split(",");
        if (arr[17].isEmpty()){
            m_delay = 0;
        }else {
            m_delay = Float.parseFloat(arr[17]);
        }
        m_airportID = Integer.parseInt(arr[14]);
    }

    @Override
    public void write(DataOutput dataOutput) throws IOException {
        dataOutput.writeFloat(m_delay);
        dataOutput.writeInt(m_airportID);
    }

    @Override
    public void readFields(DataInput dataInput) throws IOException {
        m_delay = dataInput.readFloat();
        m_airportID = dataInput.readInt();
    }

    public int getAirportID(){
        return m_airportID;
    }

    public float getDelay(){
        return m_delay;
    }
}

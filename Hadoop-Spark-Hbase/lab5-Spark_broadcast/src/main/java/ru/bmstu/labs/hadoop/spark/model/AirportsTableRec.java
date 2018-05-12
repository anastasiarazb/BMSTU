package ru.bmstu.labs.hadoop.spark.model;

import ru.bmstu.labs.hadoop.spark.Debug;

/**
 * Created by nastasia on 12.11.16.
 */
public class AirportsTableRec {

    private boolean validRec;
    private int     airportID; //Часть Key
    private String  airportName; //Value

    //Номера колонок, отвечающих извлекаемым значениям
     private static final int NAME_COLUMN = 1;
     private static final int ID_COLUMN = 0;

    private void getAirportName(String src)
    {
        airportName = src;
        Debug.out("[AirportsTableRec]: " + airportName);
    }

    private void getAirportID(String line) {
        airportID = Integer.valueOf(line); //line.replaceAll("\"", "")
    }

    public AirportsTableRec() {}

    public AirportsTableRec(String line) {
        String[] columns = line.replaceAll("\"", "").split(",");
        validRec = !line.startsWith("Code");
        if (validRec) {
            getAirportID(columns[ID_COLUMN]);
            getAirportName(columns[NAME_COLUMN]);
        }
    }

    public boolean isValidRecord() {
        return validRec;
    }

    public int getAirportID() {
        return airportID;
    }

    public String getAirportName() {
        return airportName;
    }
}
package ru.bmstu.labs.hadoop.lab3.model;

import org.apache.hadoop.io.Text;
import ru.bmstu.labs.hadoop.lab3.Debug;
import ru.bmstu.labs.hadoop.lab3.JoinedAirportFlightKey;

/**
 * Created by nastasia on 12.11.16.
 */
public class AirportsTableRec {

    private boolean validRec;
    private int airportID; //Часть Key
    private String airportName; //Value

    //Номера колонок, отвечающих извлекаемым значениям
     private static final int NAME_COLUMN = 1;
     private static final int ID_COLUMN = 0;

    private static String trimQuotes(String src) {
        int start = 0;
        int end = src.length();
        if (src.startsWith("\"")) {
            start += 1;
        }
        if (src.endsWith("\"")) {
            end -= 1;
        }
        return src.substring(start, end);
    }

    private void getAirportName(String src)
    {
        airportName = src;
        Debug.out("[AirportsTableRec]: " + airportName);
    }

    private void getAirportID(String line) {
        airportID = Integer.valueOf(line); //line.replaceAll("\"", "")
    }

    public AirportsTableRec(String line) {
        String[] columns = trimQuotes(line).split("\",\"");
        validRec = !line.startsWith("Code");
        if (validRec) {
            getAirportID(columns[ID_COLUMN]);
            getAirportName(columns[NAME_COLUMN]);
        }
    }

    public boolean isValidRecord() {
        return validRec;
    }

    public JoinedAirportFlightKey key() {
        return new JoinedAirportFlightKey(airportID, JoinedAirportFlightKey.AIRPORT);
    }

    public Text value() {
        return new Text(airportName);
    }
}
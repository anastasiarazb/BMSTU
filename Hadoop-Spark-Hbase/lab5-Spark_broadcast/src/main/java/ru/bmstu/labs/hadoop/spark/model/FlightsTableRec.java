package ru.bmstu.labs.hadoop.spark.model;

public class FlightsTableRec {

    private boolean validRec;
    private boolean isDelayed;
    private boolean isCancelled;
    private int     destAirportID; //Часть Key
    private int     origAirportID;
    private float   arrivalDelayNew; //Value

    //Номера колонок, отвечающих извлекаемым значениям
    private static final int ORIGIN_AIRPORT_ID_COLUMN = 11;
    private static final int DEST_AIRPORT_ID_COLUMN = 14;
    private static final int ARR_DELAY_NEW_COLUMN = 18;
    private static final int CANCELLED_COLUMN = 19;

    private void extractDelay(String src)
    {
        if (src.isEmpty()) {
            isDelayed = false; //Рейс был отменен, а не задержан
            arrivalDelayNew = 0.0f;
        }
        else {
            arrivalDelayNew = Float.valueOf(src);
            isDelayed = arrivalDelayNew > 0.0;
        }
    }


    public FlightsTableRec() {}

    public FlightsTableRec(String line) {
        String[] columns = line.split(",");
        validRec = !line.startsWith("\"YEAR\"");
        if (validRec) {
            extractDelay(columns[ARR_DELAY_NEW_COLUMN]);
            destAirportID = Integer.valueOf(columns[DEST_AIRPORT_ID_COLUMN]);
            origAirportID = Integer.valueOf(columns[ORIGIN_AIRPORT_ID_COLUMN]);
            isCancelled   = Float.valueOf(columns[CANCELLED_COLUMN]) == 1.00;
        }
    }

    public boolean getDelayed() {
        return validRec && isDelayed;
    }

    public boolean getCancelled() {
        return validRec && isCancelled;
    }

    public float getArrivalDelayNew() {
        return arrivalDelayNew;
    }

    public int getDestAirportID() {
        return destAirportID;
    }

    public int getOriginAirportID() {
        return origAirportID;
    }
}
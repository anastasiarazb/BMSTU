package ru.bmstu.hadoop.hbase.model;

public class FlightTable {
    public static final String YEAR    = "YEAR";
    public static final String QUARTER = "QUARTER";
    public static final String MONTH   = "MONTH";
    public static final String DAY_OF_MONTH = "DAY_OF_MONTH";
    public static final String DAY_OF_WEEK  = "DAY_OF_WEEK";
    public static final String FL_DATE = "FL_DATE";
    public static final String UNIQUE_CARRIER = "UNIQUE_CARRIER";
    public static final String AIRLINE_ID = "AIRLINE_ID";
    public static final String CARRIER    = "CARRIER";
    public static final String TAIL_NUM   = "TAIL_NUM";
    public static final String FL_NUM     = "FL_NUM";
    public static final String ORIGIN_AIRPORT_ID     = "ORIGIN_AIRPORT_ID";
    public static final String ORIGIN_AIRPORT_SEQ_ID = "ORIGIN_AIRPORT_SEQ_ID";
    public static final String ORIGIN_CITY_MARKET_ID = "ORIGIN_CITY_MARKET_ID";
    public static final String DEST_AIRPORT_ID       = "DEST_AIRPORT_ID";
    public static final String WHEELS_ON = "WHEELS_ON";
    public static final String ARR_TIME  = "ARR_TIME";
    public static final String ARR_DELAY = "ARR_DELAY";
    public static final String ARR_DELAY_NEW = "ARR_DELAY_NEW";
    public static final String CANCELLED     = "CANCELLED";
    public static final String CANCELLATION_CODE ="CANCELLATION_CODE";
    public static final String AIR_TIME = "AIR_TIME";
    public static final String DISTANCE = "DISTANCE";

    public static int columnNum(String[] columns, String columnName) {
        for (int i = 0; i < columns.length; ++i) {
            if (columnName.equals(columns[i])) {
                return i;
            }
        }
        return -1;
    }

}

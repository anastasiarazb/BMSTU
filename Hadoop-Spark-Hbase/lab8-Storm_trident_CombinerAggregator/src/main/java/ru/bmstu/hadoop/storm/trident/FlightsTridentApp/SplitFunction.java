package ru.bmstu.hadoop.storm.trident.FlightsTridentApp;


import org.apache.storm.trident.operation.BaseFunction;
import org.apache.storm.trident.operation.TridentCollector;
import org.apache.storm.trident.tuple.TridentTuple;
import org.apache.storm.tuple.Values;

public class SplitFunction extends BaseFunction {

    /*
    б. Разрабатываем Function, которая будет принимать строку и разбивать ее на tuple, состоящий из следующих данных
    664600583_T_ONTIME_sample.csv :
    день недели, время опоздания, признак совершенного рейса
    */
    /*
     "YEAR","QUARTER","MONTH","DAY_OF_MONTH","DAY_OF_WEEK","FL_DATE","UNIQUE_CARRIER","AIRLINE_ID","CARRIER",
     "TAIL_NUM","FL_NUM","ORIGIN_AIRPORT_ID","ORIGIN_AIRPORT_SEQ_ID","ORIGIN_CITY_MARKET_ID","DEST_AIRPORT_ID",
     "WHEELS_ON","ARR_TIME","ARR_DELAY","ARR_DELAY_NEW","CANCELLED","CANCELLATION_CODE","AIR_TIME","DISTANCE"
    */
    private static final int DAY_OF_WEEK   = 4;
    private static final int ARR_DELAY_NEW = 18;
    private static final int IS_CANCELLED  = 19;

    //each(new Fields("row"), new SplitFunction(), new Fields("day_of_week", "array_delay", "is_cancelled"))
    @Override
    public void execute(TridentTuple tridentTuple, TridentCollector tridentCollector) {
        String[] columns = tridentTuple.getString(0).split(",");
        tridentCollector.emit(new Values(columns[DAY_OF_WEEK],
                                         columns[ARR_DELAY_NEW],
                                         columns[IS_CANCELLED])); //emit tuple
    }
}

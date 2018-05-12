package ru.bmstu.hadoop.storm.trident.FlightsTridentApp;


import org.apache.storm.trident.operation.BaseFilter;
import org.apache.storm.trident.tuple.TridentTuple;


//в. Разработываем фильтр отбрасывающий записи рейсов которые прилетели вовремя
public class DelayedCancelledFilter extends BaseFilter {

    private static final String CANCEL_MARK = "1.00";
    private static final String ZERO_DELAY  = "0.00";

    //each(new Fields("array_delay", "is_cancelled"), new DelayedCancelledFilter())
    @Override
    public boolean isKeep(TridentTuple tridentTuple) {
        String  delayVal  = tridentTuple.getString(0);
        String  cancelVal = tridentTuple.getString(1);
        boolean cancelled = !cancelVal.equals("") && cancelVal.equals(CANCEL_MARK);
        boolean delayed   = !delayVal.equals("")  && !delayVal.equals(ZERO_DELAY);
        return cancelled || delayed;
    }
}

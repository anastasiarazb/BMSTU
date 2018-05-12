package ru.bmstu.hadoop.storm.trident.FlightsTridentApp;


import org.apache.storm.trident.operation.BaseFilter;
import org.apache.storm.trident.tuple.TridentTuple;


//д. Разрабатываем фильтр который будет принимать результат с данными CombinerAggregator и печатать их в консоль
public class AggregatedPrintFilter extends BaseFilter {
    public AggregatedPrintFilter() {
    }

    @Override
    public boolean isKeep(TridentTuple tridentTuple) {
        System.out.println(tridentTuple);
        return false;
    }
}

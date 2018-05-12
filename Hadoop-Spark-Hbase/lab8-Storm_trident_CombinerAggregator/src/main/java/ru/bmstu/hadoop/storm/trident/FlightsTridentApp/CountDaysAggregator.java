package ru.bmstu.hadoop.storm.trident.FlightsTridentApp;


import org.apache.storm.trident.operation.CombinerAggregator;
import org.apache.storm.trident.tuple.TridentTuple;

/*
г. Разрабатываем CombinerAggregator, который принимает tuple с данными совершенного рейса и подсчитывает
   агрегированные данные <день недели>:<количесво рейсов>
*/

//Пересчитывает число записей в группе после группировки по полю "day_of_week"
//.aggregate(new Fields("day_of_week"), new CountDaysAggregator(), new Fields("count"))

public class CountDaysAggregator implements CombinerAggregator<Long> {

    //Каждая запись маркируется единицей
    @Override
    public Long init(TridentTuple tridentTuple) {
        return 1L;
    }

    //Суммарное количество всех записей в группе
    @Override
    public Long combine(Long a, Long b) {
        return a + b;
    }

    //Нейтральный элемент по сложению
    @Override
    public Long zero() {
        return 0L;
    }
}

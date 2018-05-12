package ru.bmstu.labs.hadoop.spark;

import ru.bmstu.labs.hadoop.spark.model.*;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.broadcast.Broadcast;
import scala.Tuple2;

import java.util.Map;


public class AirSort {

    public static String resultStatistics(DelayStatistics statistics) {
        return " max arr_delay: "
                + Float.toString(statistics.getMaxDelay())
                + "; cancelled and delayed: "
                + Float.toString(DelayStatistics.percentDelayedOrCancelled(statistics)) + "%";
    }

    public static void main(String[] args) throws Exception {

//        а. Инициализация Spark
        SparkConf conf = new SparkConf().setAppName("AirFlightSort");
        JavaSparkContext sparkContext = new JavaSparkContext(conf);

//        б. Загрузка исходных наборов данных в RDD
        JavaRDD<String> flightTable = sparkContext.textFile("664600583_T_ONTIME_sample.csv");

//       в. Преобразуем RDD в RDD пару ключ значение  с помощью метода mapToPair (ключ - (origin, destination))
        JavaPairRDD<Tuple2<Integer, Integer>, FlightsTableRec> flightAirportsRecPair =
                flightTable.filter(
                        s -> !s.startsWith("\"YEAR\"")
                ).mapToPair(
                        s -> {
                            FlightsTableRec FlightsTableRec = new FlightsTableRec(s);
                            return new Tuple2<>(new Tuple2<>(FlightsTableRec.getOriginAirportID(),
                                                             FlightsTableRec.getDestAirportID()),
                                                FlightsTableRec
                                                );
                        });

//        г. Создаем Java объекты для хранения данных — простые объекты
//        реализующие интерфейс Serializable
//        д. В качестве ключа для пары аэропортов используем класс Tuple2
//        c помощью функции reduce или аналогичных расчитываем максимальное
//        время опоздания, процент опоздавших+отмененных рейсов

//       Ключ (origin, destination) -> DelayStatistics Serializable,
//                                где для каждой пары накапливаются кол-во оподзавших, отмененных, максимальное время опоздания
        JavaPairRDD<Tuple2<Integer,Integer>, DelayStatistics> st_flights =
                flightAirportsRecPair.combineByKey(

                        //Статистика для одной единственной записи FlightsTableRec -> DelayStatistics
                        flightsTableRec -> DelayStatistics.createNewDelayStatistics( //(float delay, boolean delayed, boolean cancelled)
                                flightsTableRec.getArrivalDelayNew(),
                                flightsTableRec.getDelayed(),
                                flightsTableRec.getCancelled()
                        ),

                        //Обновление статистики с помощью очередной записи (DelayStatistics, FlightsTableRec) -> DelayStatistics
                        (statistics, flightsTableRec) -> DelayStatistics.updateByNewRec( //(statistics, newDelay, delayed, cancelled)
                                statistics,
                                flightsTableRec.getArrivalDelayNew(),
                                flightsTableRec.getDelayed(),
                                flightsTableRec.getCancelled()
                        ),

                        //Слияние двух статистических аккумуляторов (DelayStatistics, DelayStatistics) -> DelayStatistics
                        DelayStatistics::combineTwoStatistics);

        JavaRDD<String> airportTable = sparkContext.textFile("L_AIRPORT_ID.csv");

        JavaPairRDD<Integer, String> airportIdNamePair =
                airportTable
                        .filter(s -> !s.startsWith("Code"))
                        .mapToPair(
                                s -> {
                                    AirportsTableRec data = new AirportsTableRec(s);
                                    return new Tuple2<>(data.getAirportID(), data.getAirportName());

                                });

//        е. для связывания с таблицей аэропортов — предварительно
//        выкачиваем список аэропортов в главную функцию с помощью метода collectAsMap
//        (структура записи осталась той же, только теперь номер аэропорта - ключ в map)

        Map<Integer, String> stringAirportsTableRecMap = airportIdNamePair.collectAsMap();


//        ё. создаем в основном методе main переменную broadcast
//        final Broadcast<Map<String, AirportData>> airportsBroadcasted =
//                sc.broadcast(stringAirportDataMap);
        final Broadcast<Map<Integer, String>> airportsBroadcasted =
                sparkContext.broadcast(stringAirportsTableRecMap);

//        ж. в методе map преобразуем итоговый RDD содержащий статистические
//        данные — обогащаем его именами аэропортов, обращаясь внутри
//        функций к объекту airportsBroadcasted.value()
// JavaPairRDD<Tuple2<Integer,Integer>, DelayStatistics> st_flights :
// flightsTableRec._1() - ключ (origin, destination)
// flightsTableRec._2() - statistics - накопленная с помощью combineByKey статистика для данной пары
        JavaPairRDD<String, String> joined = st_flights.mapToPair(
                flightsTableRec -> new Tuple2<>(
                        airportsBroadcasted.value().get(flightsTableRec._1()._1()) +
                        " TO " +
                        airportsBroadcasted.value().get(flightsTableRec._1()._2()),
                        resultStatistics(flightsTableRec._2())
                )

        );

        joined.saveAsTextFile("output");

    }

}

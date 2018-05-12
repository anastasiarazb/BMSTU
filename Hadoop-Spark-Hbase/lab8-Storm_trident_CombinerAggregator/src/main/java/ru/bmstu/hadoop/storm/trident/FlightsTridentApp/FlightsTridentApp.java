package ru.bmstu.hadoop.storm.trident.FlightsTridentApp;


import org.apache.storm.Config;
import org.apache.storm.LocalCluster;
import org.apache.storm.trident.TridentTopology;
import org.apache.storm.tuple.Fields;

/*
    public Stream aggregate(Aggregator agg, Fields functionFields)
    public Stream aggregate(Fields inputFields, Aggregator agg, Fields functionFields)
    public Stream aggregate(CombinerAggregator agg, Fields functionFields)
    public Stream aggregate(Fields inputFields, CombinerAggregator agg, Fields functionFields) <- используется этот
    public Stream aggregate(ReducerAggregator agg, Fields functionFields)
    public Stream aggregate(Fields inputFields, ReducerAggregator agg, Fields functionFields)
    */

/*  Применяет функцию к потоку данных и генерирует дополнительные поля
    public Stream each(Fields inputFields, Function function, Fields functionFields) {
        this.projectionValidation(inputFields);
        return this._topology.addSourcedNode(this, new ProcessorNode(this._topology.getUniqueStreamId(), this._name, TridentUtils.fieldsConcat(new Fields[]{this.getOutputFields(), functionFields}), functionFields, new EachProcessor(inputFields, function)));
    }

    Фильтрует поток данных
    public Stream each(Fields inputFields, Filter filter) {
        return this.each(inputFields, new FilterExecutor(filter), new Fields(new String[0]));
    }
    */

public class FlightsTridentApp {

    public static String inputStreamDir    = "/home/nastasia/5sem/Hadoop/Labs/lab8/input_files";
    public static String processedFilesDir = "/home/nastasia/5sem/Hadoop/Labs/lab8/output_files";

    public static void main(String[] args) {
        TridentTopology topology = new TridentTopology();

        topology.newStream("generator",new BatchSpout())
                .parallelismHint(1).shuffle()
                .each(new Fields("row"), new SplitFunction(),                              //Генерирует дополнительные поля
                      new Fields("day_of_week", "array_delay", "is_cancelled")).shuffle()
                .each(new Fields("array_delay", "is_cancelled"), new DelayedCancelledFilter())      //Фильтрует по 2-м полям
                .groupBy(new Fields("day_of_week"))                                       //Группирует по дню недели
                //(Функция агрегирования будет вызываться не для всех сообщений в partition, а для каждого ключа отдельно)
                //А иначе она применяется ко всему потоку данных в рамках одного пакета данных - batch
                .aggregate(new Fields("day_of_week"), new CountDaysAggregator(),          //Пересчитывает число записей в группе
                           new Fields("count"))                                           //и генерирует новое поле
                .parallelismHint(7)
                .each(new Fields("day_of_week", "count"), new AggregatedPrintFilter());  //Распечатывает пары (поле группировки, счетчик)

        Config config = new Config();
        config.setDebug(false);
        config.put(BatchSpout.POLL_DIR,      inputStreamDir);
        config.put(BatchSpout.PROCESSED_DIR, processedFilesDir);

        LocalCluster cluster = new LocalCluster();
        cluster.submitTopology("Distribution topology",config,topology.build());
    }
}


/*
2. Задача.
Требуется разработать топологию для анализа данных перелетов — получить распределение количества опоздавших или
отмененных рейсов по дням недели.

а. Требуется разработать Spout который будет опрашивать директорию и в случае обнаружения в ней новых файлов читать ее
   построчно и генерировать batch cостоящий из всех строк файла. После окончания файла требуется перенести файл в папку
   для обработанных файлов
б. Разрабатываем Function которая будет принимать строку и разбивать ее на tuple состоящий из следующих данных
   664600583_T_ONTIME_sample.csv : день недели, время опоздания, признак совершенного рейса
в. Разработываем фильтр отбрасывающий записи рейсов которые прилетели вовремя
г. Разрабатываем CombinerAggregator который принимает tuple с данными совершенного рейса и подсчитывает агрегированные
   данные <день недели>:<количесво рейсов>
д. Разрабатываем фильтр который будет принимать результат с данными CombinerAggregator и печатать их в консоль
е. В топологии либо используем .groupBy + .aggregate и тогда в CombinerAggregator можно использовать простой объект,
   который хранит день недели и количество перелетов  (<- реализован этот вариант)
   либо используем .partitionBy + .partitionAggregate и тогда CombinerAggregator использует в качестве аккумулятора набор
   значений день недели:количество рейсов

3. Посдказки.
а. Spout реализует интерфейс IBatchSpout.
б. Функция расширяет класс BaseFunction.
в. Фильтр расширяет класс BaseFilter.
г. Агрегатор реализует интерфейс CombinerAggregator.

*/
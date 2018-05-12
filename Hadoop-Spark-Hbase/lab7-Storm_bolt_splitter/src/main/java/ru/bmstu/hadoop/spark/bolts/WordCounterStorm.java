package ru.bmstu.hadoop.spark.bolts;


import org.apache.storm.Config;
import org.apache.storm.LocalCluster;
import org.apache.storm.topology.TopologyBuilder;
import org.apache.storm.tuple.Fields;

public class WordCounterStorm {

    public static String inputStreamDir    = "/home/nastasia/5sem/Hadoop/Labs/lab7/input_files";
    public static String processedFilesDir = "/home/nastasia/5sem/Hadoop/Labs/lab7/output_files";

    public static void main(String[] args) throws Exception {
        TopologyBuilder builder = new TopologyBuilder();
        builder.setSpout("generator", new SpoutFiles());

        /*ж. В основном цикле spout в методе nextTuple мы должны проверить, читаем ли
мы в данный момент файл. Если читаем то отправить вызовом метода
SpoutOutputCollector.emit следующую строку в поток words. Если строки в
файле кончились, то отправить сигнал в поток sync и перенести файл в папку
обработанных файлов.
*/
        builder.setBolt("splitter", new BoltSplitter(),10)
                .shuffleGrouping("generator", "words"); //Shuffle Grouping,  bolt выбирается случайно, равномерно используются все экземаляры


        builder.setBolt("counter", new BoltCounter(),10)
                .fieldsGrouping("splitter",new Fields("word")) //Fields Grouping Для группировки указывается ряд полей.
                                                               //Гарантируется что записи с одинаковым набором
                                                               //значений попадут одному и тому экземпляру bolt
                .allGrouping("generator", "sync"); //All Grouping Сообщение отсылается каждому экземпляру bolt


        Config config = new Config();
        config.setDebug(false);
        config.put(SpoutFiles.POLL_DIR,      inputStreamDir);
        config.put(SpoutFiles.PROCESSED_DIR, processedFilesDir);

        LocalCluster cluster = new LocalCluster();
        cluster.submitTopology("Frequency Dictionary", config, builder.createTopology());
    }
}

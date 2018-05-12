package ru.bmstu.hadoop.spark.bolts;


import com.google.common.io.Files;
import org.apache.storm.shade.com.google.common.base.Charsets;
import org.apache.storm.spout.SpoutOutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichSpout;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Values;
import org.apache.storm.utils.*;

import java.io.*;
import java.util.Map;

//Узел Spout только генерирует сообщения(обычно связан со внешней системой)
public class SpoutFiles extends BaseRichSpout {

    public String inputStreamDir ;    // = "/home/nastasia/5sem/Hadoop/Labs/lab7/input_files";
    public String processedFilesDir;  // = "/home/nastasia/5sem/Hadoop/Labs/lab7/output_files";

    public static String POLL_DIR      = "POLL_DIR";
    public static String PROCESSED_DIR = "PROCESSED_DIR";

    private SpoutOutputCollector outputCollector; //В outputCollector отправляется tuple, содержащий слово
    private BufferedReader       reader;          //е. Читать файл построчно можно с помощью класса BufferedReader
    private File    workingDirectory,
                    currentFile;
    //ж. В основном цикле spout в методе nextTuple мы должны проверить, читаем ли мы в данный момент файл.
    private Boolean isReadingFile;


    //declareOutputFields, описывает исходящие tuple
    /*Поток данных
    Bolt и Spout в методе declareOutputFields могут задекларировать несколько потоков исходящих данных
    ● Для каждого потока задается набор полей
    – outputFieldsDeclarer.declareStream("words", new Fields("line"));
    – outputFieldsDeclarer.declareStream("sync", new Fields("command"));
    ● В момент связывания узлов можно явно указать поток данных
    builder.setBolt("splitter", new SimpleSplitBolt(), 10).shuffleGrouping("generator", "words");
    */

    @Override
    public void declareOutputFields(OutputFieldsDeclarer outputFieldsDeclarer) {
        outputFieldsDeclarer.declareStream("words", new Fields("word")); //Fields(String... fields)
        outputFieldsDeclarer.declareStream("sync",  new Fields());
    }

    //open, иницализирует spout
    // д. Параметры извлекаются в spout в методе open, в параметрах которого есть Config
    @Override
    public void open(Map map, TopologyContext topologyContext, SpoutOutputCollector spoutOutputCollector) {
        outputCollector   = spoutOutputCollector;
        inputStreamDir    = (String)map.get(POLL_DIR);
        processedFilesDir = (String)map.get(PROCESSED_DIR);
        workingDirectory  = new File(inputStreamDir);
        isReadingFile     = false;
    }


    //nextTuple, генерирует следующее сообщение (NextTuple вызывается в бесконечном цикле)
    /* ж. В основном цикле spout в методе nextTuple мы должны проверить, читаем ли мы в данный момент файл.
    Если читаем то отправить вызовом метода SpoutOutputCollector.emit следующую строку в поток words.
    Если строки в файле кончились, то отправить сигнал в поток sync и перенести файл в папку обработанных файлов.
    Если файл не читаем, то просканировать директорию в поисках нового файла для чтения.
    В случае если файла нет то вызовом org.apache.storm.utils.Utils.sleep простановить работу потока на время
    (например 100мс) */
    /*
    public List<Integer> emit(String streamId, List<Object> tuple, Object messageId);
    public List<Integer> emit(List<Object> tuple, Object messageId);
    public List<Integer> emit(List<Object> tuple);
    public List<Integer> emit(String streamId, List<Object> tuple); <- используется эта функция
    */

    @Override
    public void nextTuple() {
        if (isReadingFile) {
            try {
                String line = reader.readLine();
                if (line != null){
                    outputCollector.emit("words", new Values(line)); //Values extends ArrayList<Object>
                } else {
                        Files.move(currentFile, new File(processedFilesDir + File.separator  + currentFile.getName()));
                        outputCollector.emit("sync", new Values());
                        isReadingFile = false;
                }
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }
        else {
            File[] filesList = workingDirectory.listFiles();
            if (filesList == null || filesList.length == 0) {
                Utils.sleep(100);
            }
            else {
                try {
                    //Брать по одному первый файл из списка в директории, считывать построчно
                    currentFile = filesList[0];
                    reader = new BufferedReader(new InputStreamReader(new FileInputStream(currentFile), Charsets.UTF_8));
                    isReadingFile = true;
                }
                catch (FileNotFoundException e) {
                    e.printStackTrace();
                }
            }
        }

    }

    //Методы для работы с подтвержениями (можно сравнивать messageId, если использованы методы
    // public List<Integer> emit(List<Object> tuple, Object messageId                     или
    // public List<Integer> emit(String streamId, List<Object> tuple, Object messageId);)

    /*
Подтверждение сообщений
● Надежные (reliable) bolt и spout посылают вместе с сообщением id сообщения
● Принимающий bolt может подтвердить успешную обработку(ack) или послать сообщение об ошибке(fail)
●  Отправитель получает нотификацию о каждом сообщении которое он отправил
*/
    @Override
    public void ack(Object msgId) {
    }
}

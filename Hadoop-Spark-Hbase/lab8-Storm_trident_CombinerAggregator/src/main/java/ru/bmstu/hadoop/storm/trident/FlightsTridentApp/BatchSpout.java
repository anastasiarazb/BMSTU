package ru.bmstu.hadoop.storm.trident.FlightsTridentApp;


import com.google.common.io.Files;
import org.apache.storm.shade.com.google.common.base.Charsets;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.trident.operation.TridentCollector;
import org.apache.storm.trident.spout.IBatchSpout;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Values;

import java.io.*;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

public class BatchSpout implements IBatchSpout {

    public String inputStreamDir ;    // = "/home/nastasia/5sem/Hadoop/Labs/lab8/input_files";
    public String processedFilesDir;  // = "/home/nastasia/5sem/Hadoop/Labs/lab8/output_files";

    public static String POLL_DIR      = "POLL_DIR";
    public static String PROCESSED_DIR = "PROCESSED_DIR";

    private TopologyContext topologyContext;
    private File workingDirectory;

    public BatchSpout() {}

    //open, иницализирует spout
    // д. Параметры извлекаются в spout в методе open, в параметрах которого есть Config
    @Override
    public void open(Map map, TopologyContext topologyContext) {
        this.topologyContext = topologyContext;
        inputStreamDir    = (String)map.get(POLL_DIR);
        processedFilesDir = (String)map.get(PROCESSED_DIR);
        workingDirectory  = new File(inputStreamDir);
    }

    @Override
    public void emitBatch(long batchId, TridentCollector tridentCollector) {
        File[] filesListDirectory = workingDirectory.listFiles();
        if (filesListDirectory != null && filesListDirectory.length != 0) {
            //Читать файлы по одному построчно
            File currentFile = filesListDirectory[0];
            try {
                BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(currentFile), Charsets.UTF_8));
                // Забрать 1-ю строку из потока
                String header = reader.readLine();
                for (String line = reader.readLine(); line != null; line = reader.readLine()) {
                    tridentCollector.emit(new Values(line));
                }
                reader.close();
                Files.move(currentFile, new File(processedFilesDir + File.separator  + currentFile.getName()));
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void ack(long  batchId) {}

    @Override
    public void close() {}

    @Override
    public Map<String, Object> getComponentConfiguration() {
        return null;
    }

    @Override
    public Fields getOutputFields() {
        return new Fields("row");
    }
}

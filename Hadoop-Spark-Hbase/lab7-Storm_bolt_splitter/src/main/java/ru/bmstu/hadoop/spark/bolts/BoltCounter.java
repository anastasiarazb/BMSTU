package ru.bmstu.hadoop.spark.bolts;


import org.apache.storm.task.OutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichBolt;
import org.apache.storm.tuple.Tuple;

import java.util.HashMap;
import java.util.Map;


// Во втором bolt принимамем слова и строим частотный словарь.

public class BoltCounter extends BaseRichBolt {
    private HashMap<String, Integer> dictionary;

    @Override
    public void prepare(Map map, TopologyContext topologyContext, OutputCollector outputCollector) {
        dictionary = new HashMap<>();
    }

    @Override
    public void execute(Tuple tuple) {
        //В этот поток отправляются сообщения после считывания и закрытия файла - слова уже загружены в частотный словарь
        if (tuple.getSourceStreamId().equals("sync")) {
            for (Map.Entry<String, Integer> keyValue: dictionary.entrySet()) {
                System.out.println(keyValue.getKey() + " : " + keyValue.getValue());
            }
            dictionary.clear();
        }
        //Заполнения словаря текущими значениями
        else {
            String word = tuple.getStringByField("word");
            int entriesNum = dictionary.containsKey(word)
                      ? dictionary.get(word) //Взять счетчик из словаря
                      : 0;                   //Такого слова в словаре пока нет => счетчик равен 0
            //Положить в словарь слово и инкрементировать счетчик, так как встретилось новое вхождение
            dictionary.put(word, entriesNum + 1);
        }
    }

    @Override
    public void declareOutputFields(OutputFieldsDeclarer outputFieldsDeclarer) {}
}

import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import java.io.IOException;
import java.lang.String;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class WordMapper extends Mapper<LongWritable, Text, Text, IntWritable> {
    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
//        <получаем строку из value, удаляем все спецсимволы, переводим в lowercase, разбиваем на слова и каждое слово пишем в контекст с счетчиком 1
//        в контекст пишется пара — Text и IntWritable >
        String line = value.toString();
//        Regex rgx = new Regex("[^a-zA-Z0-9 -]");
//        line = rgx.Replace(line, "");
        String[] words = line.toLowerCase().split("[^a-zA-Z0-9 -]");
//        String[] words = line.removeAllNonSymbols().toLowerCase().split();
        for (String word : words) {
            context.write(new Text(word), new IntWritable(1));
        }


    }
}

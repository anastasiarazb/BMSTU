package ru.bmstu.labs.hadoop.lab2;

import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;

public class FlightSortApp {
    public static void main(String[] args) throws Exception {
        if (args.length != 2) {
            System.err.println("Usage: FlightSortApp <input path> <output path>");
            System.err.println("Usage: FlightSortApp <input path> <output path>");
            System.exit(-1);
        }
        Job job = Job.getInstance();
        job.setJarByClass(FlightSortApp.class);
        job.setJobName("Flight sort");
        //Настраиваем формат данных
//        job.setInputFormatClass(SequenceFileInputFormat.class); //new
//        SequenceFileInputFormat.addInputPath(job, new Path(args[0])); //new
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        //Настраиваем mapper, partitioner, GroupingComparatorClass
        job.setMapperClass(FlightMapper.class); //new
        job.setPartitionerClass(FlightPartitioner.class); //new
        job.setGroupingComparatorClass(FlightComparator.class); //new

        //Настраиваем типы данных
        job.setMapOutputKeyClass(FlightKey.class); //new
        job.setReducerClass(FlightReducer.class);
        //Mapper<.., .., KEYOUT, VALUEOUT>
        job.setOutputKeyClass(FlightKey.class);
        job.setOutputValueClass(Text.class);
        //Распараллеленность
        job.setNumReduceTasks(2);
        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}

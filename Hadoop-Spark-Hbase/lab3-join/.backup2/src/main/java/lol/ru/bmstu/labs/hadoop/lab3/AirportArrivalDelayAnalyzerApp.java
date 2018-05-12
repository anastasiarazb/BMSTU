package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class AirportArrivalDelayAnalyzerApp {
    public static void main(String[] args)throws Exception{
        if (args.length != 3){
            System.err.println("Usage: AirportArrivalDelayAnalyzerApp <input FLIGHTS table path> <input AIRPORTS ID table path> <OUTPUT path>");
            System.exit(-1);
        }
        Job job = Job.getInstance();
        job.setJarByClass(AirportArrivalDelayAnalyzerApp.class);
        job.setJobName("Join Airports and Flights tables");


        //Настраиваем формат входных данных и соответствующие mapper'ы, а также выходной файл
        MultipleInputs.addInputPath(job, new Path(args[0]), TextInputFormat.class, FlightMapper.class);
        MultipleInputs.addInputPath(job, new Path(args[1]), TextInputFormat.class, AirportMapper.class);
        FileOutputFormat.setOutputPath(job, new Path(args[2]));

        //Настраиваем partitioner, reducer, GroupingComparatorClass
        job.setPartitionerClass(AirportPartitioner.class);
        job.setGroupingComparatorClass(AirportGroupingComparator.class);
        job.setReducerClass(AirportReducer.class);

        //Настраиваем типы данных Mapper<.., .., KEYOUT, VALUEOUT>
        job.setMapOutputKeyClass(AirportWritableComparable.class);
        job.setMapOutputValueClass(Text.class);

        //job output data
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        //Распараллеленность
        job.setNumReduceTasks(2);

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}

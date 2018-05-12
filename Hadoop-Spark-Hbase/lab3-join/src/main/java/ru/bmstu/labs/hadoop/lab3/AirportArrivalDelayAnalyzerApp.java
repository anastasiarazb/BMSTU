package ru.bmstu.labs.hadoop.lab3;

import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;

import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;



public class AirportArrivalDelayAnalyzerApp {
    public static void main(String[] args) throws Exception {
        if (args.length != 3) {
            System.err.println("Usage: AirportArrivalDelayAnalyzerApp <input FLIGHTS table path> <input AIRPORTS ID table path> <OUTPUT path>");
            System.exit(-1);
        }
        Job job = Job.getInstance();
        job.setJarByClass(AirportArrivalDelayAnalyzerApp.class);
        job.setJobName("Join Airports and Flights tables");

        //Настраиваем формат входных данных и соответствующие mapper'ы, а также выходной файл
        MultipleInputs.addInputPath(job, new Path(args[0]), TextInputFormat.class, FlightDelayMapper.class);
        MultipleInputs.addInputPath(job, new Path(args[1]), TextInputFormat.class, AirportIDMapper.class);
        FileOutputFormat.setOutputPath(job, new Path(args[2]));

        //Настраиваем partitioner, reducer, GroupingComparatorClass
        job.setPartitionerClass(AirportIDPartitioner.class);
        job.setGroupingComparatorClass(AirportIDComparator.class);
        job.setReducerClass(FlightReducer.class);

        //Настраиваем типы данных <.., .., KEYOUT, VALUEOUT>
        job.setMapOutputKeyClass(JoinedAirportFlightKey.class);
        job.setMapOutputValueClass(Text.class);

        //job output data
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        //Распараллеленность
        job.setNumReduceTasks(2);

        Debug.set(true);
        Debug.out("[AirportArrivalDelayAnalyzer]: INITIALIZATION DONE");

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
#!/bin/bash

currentdir=$(pwd)
cd $currentdir
labname=`basename $currentdir`
rm -r output/*
hadoop fs -rm -r output
mvn package
hadoop fs -copyFromLocal 664600583_T_ONTIME_sample.csv
hadoop fs -copyFromLocal L_AIRPORT_ID.csv
spark-submit --class ru.bmstu.labs.hadoop.spark.AirSort --master yarn-client --num-executors 3  target/spark-examples-1.0-SNAPSHOT.jar
hadoop fs -copyToLocal output 

#!/bin/bash

currentdir=$(pwd)
cd $currentdir
labname=`basename $currentdir`
mvn package

hadoop-start.sh

rm -r output/*

hadoop fs -copyFromLocal $currentdir/sample.csv
export HADOOP_CLASSPATH=target/hadoop-$labname-1.0-SNAPSHOT.jar
hadoop ru.bmstu.labs.hadoop.$labname.FlightSortApp sample.csv output
hadoop fs -copyToLocal output 

#!/bin/bash

currentdir=$(pwd)
cd $currentdir
labname=`basename $currentdir`
mvn package

hadoop-start.sh

hadoop fs -copyFromLocal $currentdir/664600583_T_ONTIME_sample.csv
hadoop fs -copyFromLocal $currentdir/L_AIRPORT_ID.csv

export HADOOP_CLASSPATH=target/hadoop-$labname-1.0-SNAPSHOT.jar
if hadoop ru.bmstu.labs.hadoop.$labname.AirportArrivalDelayAnalyzerApp 664600583_T_ONTIME_sample.csv L_AIRPORT_ID.csv output
then
    rm -r output/*
    hadoop fs -copyToLocal output
    echo "SUCCESS :)"
else
    echo "FAILED :("
fi

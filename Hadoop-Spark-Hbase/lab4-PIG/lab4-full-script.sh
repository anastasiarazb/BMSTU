#!/bin/bash
/home/nastasia/5sem/Hadoop/myscripts/2_hadoop-start.sh

hadoop fs -copyFromLocal /home/nastasia/5sem/Hadoop/Labs/lab4/T_ONTIME.csv
hadoop fs -copyFromLocal /home/nastasia/5sem/Hadoop/Labs/lab4/L_AIRPORT_ID.csv
hadoop fs -copyFromLocal /home/nastasia/5sem/Hadoop/Labs/lab4/DAYS_OF_WEEK.csv 

/home/nastasia/Programs/pig-0.16.0/bin/pig "/home/nastasia/5sem/Hadoop/Labs/lab4/backup.pig"

#!/bin/bash

stop-yarn.sh 
stop-dfs.sh
sudo rm -R /home/nastasia/Hadoop_data/datanode/*
sudo rm -R /home/nastasia/Hadoop_data/namenode/*
hdfs namenode -format
start-dfs.sh
start-yarn.sh
#Чтобы не было copyFromLocal: `.': No such file or directory
hadoop fs -mkdir -p /user/nastasia
hadoop fs -chown nastasia:nastasia /user/nastasia

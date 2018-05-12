#!/bin/bash

stop-yarn.sh 
stop-dfs.sh
sudo rm -R ~/Hadoop_data/datanode/*
sudo rm -R ~/Hadoop_data/namenode/*
hdfs namenode -format
start-dfs.sh
start-yarn.sh
#Чтобы не было copyFromLocal: `.': No such file or directory
hadoop fs -mkdir -p /user/<username>
hadoop fs -chown <username>:<username> /user/<username>

#!/bin/bash
cd /home/nastasia/5sem/Hadoop/Labs/lab1/probe/
#Собрать пакет. В одной директории с pom.xml
mvn package
#Чтобы не было copyFromLocal: `.': No such file or directory
hadoop fs -mkdir -p /user/nastasia
hadoop fs -chown nastasia:nastasia /user/nastasia

hadoop fs -copyFromLocal /home/nastasia/5sem/Hadoop/Labs/lab1/probe/warandpeace1.txt

export HADOOP_CLASSPATH=target/hadoop-examples-1.0-SNAPSHOT.jar
hadoop WordCountApp warandpeace1.txt output
hadoop fs -copyToLocal output

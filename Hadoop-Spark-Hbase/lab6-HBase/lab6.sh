#!/usr/bin/env bash

cd $(pwd)
mvn package
mvn compile exec:java -Dexec.mainClass=ru.bmstu.hadoop.hbase.FlightsTableLoader
stop-hbase.sh
start-hbase.sh
mvn compile exec:java -Dexec.mainClass=ru.bmstu.hadoop.hbase.FilterMainClass

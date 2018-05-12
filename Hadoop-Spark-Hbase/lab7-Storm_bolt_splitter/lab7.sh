#!/usr/bin/env bash
cd $(pwd)
rm -r target/*
mvn package
mvn compile exec:java -Dexec.mainClass=ru.bmstu.hadoop.spark.bolts.WordCounterStorm

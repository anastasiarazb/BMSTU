#!/bin/bash

cd  $(pwd)
mvn package
mvn compile exec:java -Dexec.mainClass=ru.bmstu.hadoop.storm.trident.FlightsTridentApp.FlightsTridentApp

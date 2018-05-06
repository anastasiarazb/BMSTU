#!/bin/bash

javac -d output2 -sourcepath src src/ApplicationMain.java
javac -d output2 -sourcepath src src/JustParseGrammar.java
cd output2
#путь к самой грамматике (самоприменение), путь к грамматике арифметических выражений, файл с выражением, n
java ApplicationMain ../data/grammar.txt ../data/file.txt ../data/expr.txt 3


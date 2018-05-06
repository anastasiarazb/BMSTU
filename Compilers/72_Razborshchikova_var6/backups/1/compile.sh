#!/bin/bash

javac -d output2 -sourcepath src src/grammar_parser/GrammarParser.java
cd output2
java grammar_parser.GrammarParser ../data/file.txt

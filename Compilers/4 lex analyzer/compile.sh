#!/bin/bash

rm test.cpp a.out
#CPPLINE_FLAGS=-DSHOW_DEBUG=1 srmake test.sref #дебаг с итерации 1
srmake test.sref
./a.out prog.txt

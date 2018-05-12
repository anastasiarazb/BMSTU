#!/bin/bash

hadoop fs -rm -r output
pig airports.pig

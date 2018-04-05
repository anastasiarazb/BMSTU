#!/usr/bin/python3
# -*- coding: utf-8 -*-
import make_dictionary
import scanner
import sys

dict = make_dictionary.make_dictionary(sys.argv[1])
try:
    file = open(sys.argv[2])
    text = file.read()
    file.close()
except IOError:
    print("Can't open file", sys.argv[2])
    sys.exit(1)

scanner.scanner(dict, text)
sys.exit()

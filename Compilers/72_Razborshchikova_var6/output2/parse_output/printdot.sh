#!/bin/bash
dot -Tpdf "parse_graph.dot" -o "parse_graph.pdf"
dot -Tpdf "arithm_parse_graph.dot" -o "arithm_parse_graph.pdf"
dot -Tpdf "self_parse_graph.dot" -o "self_parse_graph.pdf"
dot -Tpdf "expr_parse_graph.dot" -o "expr_parse_graph.pdf"

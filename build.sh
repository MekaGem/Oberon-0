#!/bin/bash
bison -vd oberon.y
flex oberon.l
g++-4.8 -O3 -o oberon lex.yy.c data_type.cpp node.cpp oberon.tab.c -lfl 

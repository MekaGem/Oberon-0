#!/bin/bash
bison -vd oberon.y
flex oberon.l
g++ -o oberon lex.yy.c node.c oberon.tab.c -lfl 

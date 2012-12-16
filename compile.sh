#!/bin/bash
bison oberon.y
flex oberon.l
g++ -o oberon lex.yy.c node.cpp -lfl
rm oberon.tab.c lex.yy.c

#!/usr/make -f
# Author: Eraldo P Marinho
# First version: Wed Oct 10 14:47:20 -03 2018
# Makefile for code directory of the MyPas project
#

projectname=mypas
CC=gcc
RM=erase
includedir=../include/

execdir=../bin/

execpath=$(execdir)$(projectname)
# execpath=$(execdir)

cfiles=\
		mypas.c\
		lexer.c\
		parser.c\
		keywords.c\
		
ofiles=\
       mypas.o\
       lexer.o\
       parser.o\
       keywords.o\

CFLAGS=-g -I$(includedir)

$(execpath): $(ofiles)
	$(CC)	-o $@	$^

clean:
	$(RM) $(ofiles)

mostlyclean: clean
	$(RM) *.tmp *~




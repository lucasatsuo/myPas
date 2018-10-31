#!/usr/make -f
# Author: Eraldo P Marinho
# First version: Wed Oct 10 14:47:20 -03 2018
# Makefile for include directory of the MyPas project
#

projectname=mypas
projectdir=$(PWD)/../
codesdir=$(projectdir)/codes/
includedir=$(projectdir)/include/

default:
	(cd $(codesdir); make)

clean:
	($(RM) *.o)

mostlyclean: clean
	$(RM) *.tmp *~ mypas

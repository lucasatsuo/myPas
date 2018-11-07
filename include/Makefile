#!/usr/make -f
# Author: Felipe Balascio, Leonardo Kanashiro, Lucas Atsuo
# First version: Wed Nov 7 2018
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

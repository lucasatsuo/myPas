#!/usr/make -f
# Author: Felipe Balascio, Leonardo Kanashiro, Lucas Atsuo
# First version: Wed Nov 7 2018
# Makefile for main directory of the MyPas project
#

projectname=mypas
projectdir=$(PWD)/
codesdir=$(projectdir)/codes/
includedir=$(projectdir)/include/
bindir=$(projectdir)/bin/

default:
	(cd $(codesdir); make )

mypas: default
	(cd $(bindir); ./$(projectname))

clean:
	(cd $(codesdir); make $@ )
	(cd $(includedir); make $@ )

mostlyclean:
	(cd $(codesdir); make $@ )
	(cd $(includedir); make $@ )
	(cd $(bindir); $(RM) *)


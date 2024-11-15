#!/bin/bash
#usage make <target> FILE=<filename>

CC=g++
CPPFLAGS= -fdiagnostics-color=always -g -std=c++23 -Wall -I.
CPPFLAGS_DEBUG=-g
CPPFLAGS_OPTIMIZE=-O1
CPPFLAGES_NO_LOG=-U__DEBUG_LOG
CPPFLAGES_BIG_TEST=-D__DEBUG_BIG_TEST

SRCS=test_loader.cpp "$(FILE)"
#OBJS=
EXE=bin/$(basename $(FILE))

precheck:
ifeq ("$(FILE)", "")
	echo "Error: FILE is empty. Usage: make <target> FILE=<filename>"
	exit 1
endif

dummy: precheck
	echo $(EXE)

clean: precheck
	rm -f $(EXE)

build_d: precheck
	$(CC) $(CPPFLAGS) $(CPPFLAGS_DEBUG) $(SRCS) -o $(EXE)
build_nolog: precheck
	$(CC) $(CPPFLAGS) $(CPPFLAGS_DEBUG) $(CPPFLAGES_NO_LOG) $(SRCS) -o $(EXE)
build_o: precheck
	$(CC) $(CPPFLAGS) $(CPPFLAGS_OPTIMIZE) $(SRCS) -o $(EXE)

run_d: clean build_d 
	./$(EXE)
run_o: clean build_o 
	./$(EXE)

valgrind: clean build_d
	valgrind ./$(EXE)
callgrind: clean build_nolog
	valgrind --tool=callgrind ./$(EXE)

build_big: precheck
	$(CC) $(CPPFLAGS) $(CPPFLAGS_DEBUG) $(CPPFLAGES_NO_LOG) $(CPPFLAGES_BIG_TEST) $(SRCS) -o $(EXE)
run_big: clean build_big
	./$(EXE)
callgrind_big: clean build_big
	valgrind --tool=callgrind ./$(EXE)

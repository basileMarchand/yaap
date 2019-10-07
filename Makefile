CXX=g++
CXXFLAGS=-std=c++11 -Wall
YAAP_INC=$(shell pwd)/include 

## In Makefile.inc define
## CATCH2_INC : path to the catch2 include dir
include Makefile.inc

export CXX
export CXXFLAGS
export YAAP_INC
export CATCH2_INC

all: examples
	@echo "If you want to compile tests please define a Makefile.inc or set the CATCH2_INC variable"

examples:
	@echo "Compile examples"
	(cd examples ; make  )

tests:
	@echo "Compile tests using Catch2 framework"
	(cd tests ; make )


.PHONY: all tests

clean:
	(cd examples ; make clean)
	(cd tests ; make clean )

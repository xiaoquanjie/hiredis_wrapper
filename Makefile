ALL_DIR_SRC 	= ./
INCLUDES 	= -Ihiredis
CPPFLAGS	= -pthread -gdwarf-2 -lhiredis

all : project

include Makefile.inc

project : $(ALL_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

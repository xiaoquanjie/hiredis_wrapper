ALL_DIR_SRC 	= ./
INCLUDES 	= -Ihiredis
CPPFLAGS	= -lhiredis -L.

all : project

include Makefile.inc

project : $(ALL_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

CXX = g++
CXXFLAGS = -Wall -O2

.PHONY: all clean

all: cricket-scheduling

cricket-scheduling: cricket-scheduling.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f cricket-scheduling

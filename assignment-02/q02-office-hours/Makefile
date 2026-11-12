CXX = g++
CXXFLAGS = -Wall -O2 -pthread

.PHONY: all clean run

all: office-hours

office-hours: office-hours.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f office-hours

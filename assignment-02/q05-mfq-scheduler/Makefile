CXX = g++
CXXFLAGS = -Wall -O2

.PHONY: all clean

all: mfq-scheduler

mfq-scheduler: mfq-scheduler.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f mfq-scheduler

CXX = g++
CXXFLAGS = -Wall -O2

.PHONY: all clean run

all: zombie-survival

zombie-survival: zombie-survival.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f zombie-survival

run: zombie-survival
	./zombie-survival

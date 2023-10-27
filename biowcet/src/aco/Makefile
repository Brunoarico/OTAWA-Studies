CXX = g++
CXXFLAGS = -std=c++11

all: aco

aco: aco.cpp aco.h main.cpp
	$(CXX) $(CXXFLAGS) -o aco -g aco.cpp aco.h main.cpp

clean:
	rm -f aco

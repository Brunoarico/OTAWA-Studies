CXX = g++
CXXFLAGS = -std=c++11

all: loop-cluster

loop-cluster: ACO_class/aco.cpp ACO_class/aco.h loop-cluster.cpp
	$(CXX) $(CXXFLAGS) -o loop-cluster -g ACO_class/aco.cpp ACO_class/aco.h loop-cluster.cpp

clean:
	rm -f loop-cluster	

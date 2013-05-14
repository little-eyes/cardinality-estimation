CXX = g++
CXXFLAGS = -g -c -Wall -pthread
LXXFLAGS = -g -Wall -pthread
DEPS = graph.h estimator.h randomcast.h
OBJS = graph.o estimator.o randomcast.o driver.o

all: simulator

simulator: $(OBJS) 
	$(CXX) -o simulator $(LXXFLAGS) $(OBJS)

driver.o: $(DEPS) driver.cc
	$(CXX) $(CXXFLAGS) driver.cc

graph.o: $(DEPS) graph.cc
	$(CXX) $(CXXFLAGS) graph.cc

estimator.o: $(DEPS) estimator.cc
	$(CXX) $(CXXFLAGS) estimator.cc

randomcast.o: $(DEPS) randomcast.cc
	$(CXX) $(CXXFLAGS) randomcast.cc

clean:
	rm -rf graph.o randomcast.o estimator.o driver.o simulator

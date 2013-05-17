CXX = g++
CXXFLAGS = -g -c
LXXFLAGS = -g
DEPS = graph.h estimator.h randomcast.h glog/logging.h
OBJS = graph.o estimator.o randomcast.o driver.o
LDFLAGS = -L./glog/ -lglog

all: simulator

simulator: $(OBJS) 
	$(CXX) -o simulator $(LXXFLAGS) $(LDFLAGS) $(OBJS)

driver.o: $(DEPS) driver.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) driver.cc

graph.o: $(DEPS) graph.cc
	$(CXX) $(CXXFLAGS) graph.cc

estimator.o: $(DEPS) estimator.cc
	$(CXX) $(CXXFLAGS) estimator.cc

randomcast.o: $(DEPS) randomcast.cc
	$(CXX) $(CXXFLAGS) randomcast.cc

clean:
	rm -rf graph.o randomcast.o estimator.o driver.o simulator

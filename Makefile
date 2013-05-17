CXX = g++
CXXFLAGS = -g -c -pthread
LXXFLAGS = -g -pthread
DEPS = graph.h estimator.h randomcast.h ./glog/logging.h
OBJS = graph.o estimator.o randomcast.o driver.o ./glog/libglog.so
LDFLAGS = -L./glog -lglog

all: simulator

simulator: $(DEPS) $(OBJS) 
	$(CXX) -o simulator $(LXXFLAGS) $(LDFLAGS) $(OBJS)

driver.o: $(DEPS) driver.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) driver.cc

graph.o: $(DEPS) graph.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) graph.cc

estimator.o: $(DEPS) estimator.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) estimator.cc

randomcast.o: $(DEPS) randomcast.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) randomcast.cc

clean:
	rm -rf graph.o randomcast.o estimator.o driver.o simulator

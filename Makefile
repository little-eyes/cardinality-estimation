CXX = g++
CXXFLAGS = -g -c -pthread
LXXFLAGS = -g -pthread
DEPS = graph.h estimator.h randomcast.h glog/logging.h glog/libglog.a 
OBJS = graph.o estimator.o randomcast.o driver.o glog/libglog.a 
LDFLAGS = -L./glog -lglog -static

all: simulator

simulator: $(DEPS) $(OBJS)
	$(CXX) -o simulator $(LXXFLAGS) $(LDFLAGS) $(OBJS)

driver.o: $(DEPS) driver.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) driver.cc

graph.o: $(DEPS) graph.cc
	$(CXX) $(CXXFLAGS) $(LDFLAGS) graph.cc

estimator.o: $(DEPS) estimator.cc
	$(CXX) $(CXXFLAGS) estimator.cc

randomcast.o: $(DEPS) randomcast.cc
	$(CXX) $(CXXFLAGS) randomcast.cc

clean:
	rm -rf graph.o randomcast.o estimator.o driver.o simulator

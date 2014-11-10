CXX = g++ -std=c++11

INCLUDE = -I. -I../Common

LIBS = -lgsl -lgslcblas

HEADERS = event.H event_queue.H event_factory.H node.H simulator.H

SOURCES = event.C event_queue.C event_factory.C node.C simulator.C

OBJECTS = event.o event_queue.o event_factory.o node.o simulator.o

MAIN = main

FAST = -Ofast

DEBUG_MODE = -O0 -g

default: obj
	$(CXX) $(FAST) $(INCLUDE) $(MAIN).C -o $(MAIN) $(OBJECTS) $(LIBS)

obj:
	$(CXX) $(FAST) -c $(INCLUDE) $(SOURCES)


debug: obj_debug
	$(CXX) $(DEBUG_MODE) $(INCLUDE) $(MAIN).C -o $(MAIN) $(OBJECTS) $(LIBS)

obj_debug:
	$(CXX) $(DEBUG_MODE) -c $(INCLUDE) $(SOURCES)

clean:
	$(RM) *.o *~

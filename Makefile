#
CXXFLAGS = -std=c++14 -Wall 
LDLIBS = 

objs = test.o genome.o
exec = test

debug: CXXFLAGS += -g -DDEBUG
debug: $(exec)

release: CXXFLAGS += -O2
release: $(exec)

$(exec): $(objs)
	$(CXX) $(CXXFLAGS) -o $@ $^

test.o: genome.hpp
genome.o: genome.cpp genome.hpp

.PHONY: clean
clean:
	rm $(exec) $(objs)

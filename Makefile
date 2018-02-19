#
CXXFLAGS = -std=c++14 -Wall 
LDLIBS = 

objs = gene.o genome.o organism.o test.o 
exec = test

debug: CXXFLAGS += -g -DDEBUG
debug: $(exec)

release: CXXFLAGS += -O2
release: $(exec)

$(exec): $(objs)
	$(CXX) $(CXXFLAGS) -o $@ $^

test.o: genome.hpp
gene.o: gene.cpp gene.hpp
genome.o: genome.cpp genome.hpp
organism.o: organism.cpp organism.hpp

.PHONY: clean
clean:
	rm $(exec) $(objs)

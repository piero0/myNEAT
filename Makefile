#
CXXFLAGS = -std=c++14 -Wall 
LDLIBS = 

objs = gene.o genome.o organism.o species.o util.o main.o
exec = main

debug: CXXFLAGS += -g -DDEBUG
debug: $(exec)

release: CXXFLAGS += -O2
release: $(exec)

$(exec): $(objs)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: genome.hpp
gene.o: gene.cpp gene.hpp
genome.o: genome.cpp genome.hpp util.hpp
organism.o: organism.cpp organism.hpp
species.o: species.cpp species.hpp
util.o: util.hpp

.PHONY: clean
clean:
	rm -f $(exec) $(objs)

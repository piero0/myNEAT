CXXFLAGS = -std=c++14 -Wall
LDLIBS = 

main = main.o
objs = gene.o util.o genome.o organism.o species.o population.o
 
exec = main
dynlib = libmyNEAT.so

debug: CXXFLAGS += -g -DDEBUG
debug: sharedobjs $(exec) 
debug: tests

release: CXXFLAGS += -O2
release: sharedobjs $(exec)

#make dynlib a prerequisite to prevent --jobs issues
tests: $(dynlib)
	@$(MAKE) -C tests $(word 2, $(MAKECMDGOALS))

sharedobjs: CXXFLAGS += -fPIC
sharedobjs: $(objs) sharedlib

sharedlib: $(dynlib)

$(dynlib): $(objs)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^

$(exec): $(objs) $(main)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: genome.hpp util.hpp
gene.o: gene.cpp gene.hpp
genome.o: genome.cpp genome.hpp util.hpp
organism.o: organism.cpp organism.hpp
species.o: species.cpp species.hpp
population.o: population.cpp population.hpp util.hpp
util.o: util.hpp

.PHONY: clean tests

clean:
	rm -f $(exec) $(objs) $(main) $(dynlib)

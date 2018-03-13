CXXFLAGS = -std=c++14 -Wall 
LDLIBS = -lmyNEAT
LDFLAGS = -L.. -Wl,-rpath=..

objs = testGene.o testGenome.o testOrganism.o\
	 testSpecies.o testPopulation.o testUtil.o
main = testMain.o
exec = unitTest
catch = catch.hpp

debug: CXXFLAGS += -g -DDEBUG
debug: $(exec)

release: CXXFLAGS += -O2
release: $(exec)

hdr: 
	$(CXX) $(CXXFLAGS) -c $(catch)

$(exec): $(main) $(objs)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean fullclean

clean:
	rm -f $(exec) $(objs)

fullclean: clean
fullclean:
	rm -f $(main)

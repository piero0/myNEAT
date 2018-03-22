CXXFLAGS = -std=c++14 -O2 -Wall -Winvalid-pch
LDLIBS = -lmyNEAT -lpthread
LDFLAGS = -L.. -Wl,-rpath=..

objs = testGene.o testGenome.o testOrganism.o\
	 testSpecies.o testPopulation.o testUtil.o
main = testMain.o
exec = unitTest
catch = catch.hpp

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

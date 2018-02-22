#
CXXFLAGS = -std=c++14 -Wall 
LDLIBS = 

objs = testMain.o test01.o
exec = unitTest
catch = catch.hpp

debug: CXXFLAGS += -g -DDEBUG
debug: $(exec)

release: CXXFLAGS += -O2
release: $(exec)

hdr: 
	$(CXX) $(CXXFLAGS) -c $(catch)

$(exec): $(objs)
	$(CXX) $(CXXFLAGS) -o $@ $^

test01.o: test01.cpp $(catch)

.PHONY: clean
clean:
	rm $(exec) $(objs)

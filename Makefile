
CXXFLAGS = -std=c++14 -Wall 

objs = test.o genome.o
exec = test

debug: CXXFLAGS += -g -DDEBUG
debug: test

release: CXXFLAGS += -O2
release: test

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

$(exec): $(objs)
	$(CXX) $(CXXFLAGS) -o $@ $?

clean:
	rm $(exec) $(objs)

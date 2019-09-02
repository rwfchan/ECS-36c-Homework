CXXFLAGS=-Wall -Werror -std=c++11

all: vector_search
vector_search: vector_search.o
	$(CXX) -o $@ $^	
execs = vector_search

clean:
	rm -rf *.o *.dat vector_search
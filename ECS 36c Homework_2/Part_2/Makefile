CXXFLAGS=-Wall -Werror -std=c++11

all: luggage_handling
luggage_handling: luggage_handling.o
	$(CXX) -o $@ $^	
execs = luggage_handling

clean:
	rm -rf *.o *.dat luggage_handling


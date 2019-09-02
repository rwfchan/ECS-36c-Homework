CXXFLAGS=-Wall -Werror -std=c++11

all: bt_succinct_dec

bt_succinct_dec: bt_succinct_dec.o
	$(CXX) -o $@ $^	

execs = bt_succinct_dec

clean:
	rm -rf *.o *.dat bt_succinct_dec
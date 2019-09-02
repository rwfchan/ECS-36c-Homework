CXXFLAGS=-Wall -Werror -std=c++11

all: bt_succinct_dec bst_tester

bt_succinct_dec: bt_succinct_dec.o
	$(CXX) -o $@ $^
bst_tester: bst_tester.o
	$(CXX) -o $@ $^

execs = bt_succinct_dec bst_tester

clean:
	rm -rf *.o *.dat bt_succinct_dec bst_tester
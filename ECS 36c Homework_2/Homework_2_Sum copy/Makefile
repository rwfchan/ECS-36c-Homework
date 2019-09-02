CXXFLAGS=-Wall -Werror -std=c++11

all: luggage_handling postfix_eval

luggage_handling: luggage_handling.o
	$(CXX) -o $@ $^	
postfix_eval: postfix_eval.o
	$(CXX) -o $@ $^	

execs = luggage_handling
execs = postfix_eval

clean:
	rm -rf *.o *.dat luggage_handling postfix_eval


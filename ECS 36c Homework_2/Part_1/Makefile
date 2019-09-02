CXXFLAGS=-Wall -Werror -std=c++11

all: postfix_eval
postfix_eval: postfix_eval.o
	$(CXX) -o $@ $^	
execs = postfix_eval

clean:
	rm -rf *.o *.dat postfix_eval
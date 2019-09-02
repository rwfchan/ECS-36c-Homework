// Copyright 2018 <Raymond Chan>

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>

// From Lecture Slide #04 - Stacks and quenes, page 4
template <typename T>
class Stack {
 public:
  unsigned int Size();
  T& Top();
  void Pop();
  void Push(const T &item);
  bool IsEmpty();
  bool Operate(char _operator);

 private:
  std::vector<T> items;
};
template<typename T>
unsigned int Stack<T>::Size() {
  return static_cast<unsigned int>(items.size());
}
template<typename T>
T &Stack<T>::Top() {
  return items.back();
}  // what is the & doing here exactly
template <typename T>
void Stack<T>::Pop() {
  items.pop_back();
}
template <typename T>
void Stack<T>::Push(const T &item) {
  items.push_back(item);
}
template <typename T>
bool Stack<T>::IsEmpty() {
  return !static_cast<bool>(items.size());
}
template <typename T>
bool Stack<T>::Operate(char _operator) {
  T item = Top();
  Pop();

  switch (_operator) {
    case '+':
      item = Top() + item;
      break;
    case '-':
      item = Top() - item;
      break;
    case '*':
      item = Top() * item;
      break;
    case '/':
      item = Top() / item;
      break;
    default:
      return false;
  }
  Pop();
  Push(item);
  return true;
}

// From https://stackoverflow.com/
//  questions/5167625/splitting-a-c-stdstring-using-tokens-e-g, by Fox32
// From Lecture Slide #04 - Stacks and quenes

// 5 7 + 6 2 -  *\n54 52 &\n56.3 4 * 2\n3 4 + 2  * 7 /

int main(void) {
  std::string piping;

  while (std::getline(std::cin, piping)) {
    Stack<double> postfix;
    double input;
    char _operator;

    piping += ' ';

    std::string error_operator;
    bool operation_error = false;

    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = piping.find(' ', pos)) != std::string::npos) {
      std::string substring(piping.substr(prev_pos, pos - prev_pos));
      if (substring.empty()) {
        prev_pos = ++pos;
        continue;
      }
      std::stringstream convert(substring);
      if (convert >> input) {
        postfix.Push(input);
      } else {
        if (postfix.IsEmpty()) {
          break;
        }
        std::stringstream convert_2(substring);
        if (!(convert_2 >> _operator && postfix.Operate(_operator))) {
          error_operator = substring;
          operation_error = true;
          break;
        }
        if (substring.size() != 1) {
          error_operator = substring;
          operation_error = true;
          break;
        }
      }
      prev_pos = ++pos;
    }

    if (operation_error) {
      std::cerr << "Error: unknown symbol '"
                << error_operator << "'" << std::endl;
      continue;
    } else if (postfix.Size() != 1) {
      std::cerr << "Error: invalid expression" << std::endl;
      continue;
    } else {
      std::cout << postfix.Top() << std::endl;
      postfix.Pop();
    }
  }

  std::cout << "Bye!" << std::endl;

  return 0;
}

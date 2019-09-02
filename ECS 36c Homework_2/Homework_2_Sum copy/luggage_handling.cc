#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <memory>

// From Lecture Slide #04 - Stacks and quenes, page 4
template <typename T>
class Stack {
 public:
  unsigned int Size();
  T& Top();
  bool Pop();
  void Push(const T &item);
  bool Is_Empty();

 private:
  std::vector<T> items;
};
template <typename T>
unsigned int Stack<T>::Size() {
  return items.size();
}
template <typename T>
T &Stack<T>::Top() {
  return items.back();
}  // what is the & doing here exactly
template <typename T>
bool Stack<T>::Pop() {
  if (!Size()) {
    return false;
  }
  items.pop_back();
  return true;
}
template <typename T>
void Stack<T>::Push(const T &item) {
  items.push_back(item);
}
template <typename T>
bool Stack<T>::Is_Empty() {
  return !Size();
}

// From Lecture Slide #04 - Stacks and quenes, page 11, 12
template <typename T>
class Queue {
 public:
  explicit Queue(unsigned int size) {
    Set_Size(size);
  }

  unsigned int Size();
  void Set_Size(unsigned int size);
  bool Is_Full();
  void Pop();
  void Push(const T &item);
  T& Front();
  T& Back();

 private:
  T* items = nullptr;
  int cur_size = -1;
  unsigned int max_size = 0;
};
template <typename T>
unsigned int Queue<T>::Size() {
  return static_cast<unsigned int>(cur_size + 1);
}
template <typename T>
void Queue<T>::Set_Size(unsigned int size) {
  max_size = size;
  items = new T[size];
}
template <typename T>
bool Queue<T>::Is_Full() {
  return Size() == max_size;
}
template <typename T>
void Queue<T>::Pop() {
  if (!Size())
    throw std::underflow_error("Empty queue!");
  for (int i = 1; i <= cur_size; i++) {
    items[i - 1] = items[i];
  }
  items[cur_size] = -1;
  cur_size--;
}
template <typename T>
void Queue<T>::Push(const T &item) {
  if (Is_Full())
    throw std::overflow_error("Full queue!");
  cur_size++;
  items[cur_size] = item;
}
template <typename T>
T& Queue<T>::Front() {
  if (!Size())
    throw std::underflow_error("Empty queue!");
  return items[0];
}
template <typename T>
T& Queue<T>::Back() {
  if (!Size())
    throw std::underflow_error("Empty queue!");
  return items[cur_size];
}

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: ./luggage_handling <input_file> <container_size>"
              << std::endl;
    return 1;
  }
  if (argv[2][0] == '-') {
    std::cerr << "Error: invalid container size" << std::endl;
    return 1;
  }

  std::vector<int> bags_list;
  unsigned int container_size;

  std::stringstream convert(argv[2]);
  convert >> container_size;

  // Reading file
  std::ifstream myfile;
  myfile.open(argv[1]);
  if (!myfile.is_open()) {
    std::cerr << "Error: cannot open file " << argv[1] << std::endl;
    return 1;
  }
  while (!myfile.eof()) {
    int bag;
    myfile >> bag;
    if (myfile.eof()) {
      break;
    }
    bags_list.push_back(bag);
  }
  myfile.close();

  unsigned int bag_amount = static_cast<unsigned int>(bags_list.size() - 1);
  Stack<std::shared_ptr<Queue<int>>> containers;
  unsigned int i = 0;

  // Inputting
  do {
    std::shared_ptr<Queue<int>> container(new Queue<int>(container_size));
    for (unsigned int cur_size = 0; cur_size < container_size; cur_size++) {
      container->Push(bags_list.at(i));
      if (i == bag_amount) {
        containers.Push(container);
        i++;
        break;
      }
      if (container->Is_Full()) {
        containers.Push(container);
      }
      i++;
    }
  } while (i != bag_amount + 1);

  // Outputting
  while (!containers.Is_Empty()) {
    for (unsigned int j = 0; j < container_size; j++) {
      std::cout << containers.Top()->Front() << ' ';
      containers.Top()->Pop();
      if (!containers.Top()->Size())
        break;
    }
    containers.Pop();
  }

  return 0;
}

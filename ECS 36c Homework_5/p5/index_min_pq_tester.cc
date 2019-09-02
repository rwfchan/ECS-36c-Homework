#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "index_min_pq.h"

// Tester
int main() {
  // Indexed min-priority queue of capacity 100
  IndexMinPQ<double> impq(100);

  // Insert a bunch of key-value
  std::vector<std::pair<double, int>> keyval{
    { 2.2, 99},
    { 51.0, 54},
    { 42.5, 53},
    { 74.32, 93}
  };
  for (auto &i : keyval) {
    impq.Push(i.first, i.second);
  }

  // Key-value at the top should now be (2.2, 99)
  std::cout << "Top()= (" << impq.Top() << ")" << std::endl;
  impq.Pop();

  // Test Contains()
  std::cout << "Contains(93)= " << impq.Contains(93) << std::endl;

  // Key-value at the top should now be (42.5, 53)
  std::cout << "Top()= (" << impq.Top() << ")" << std::endl;
  // Test ChangeKey(): change key associated to value 93
  impq.ChangeKey(1.0, 93);
  // Key-value at the top should now be (1.0, 93)
  std::cout << "Top()= (" << impq.Top() << ")" << std::endl;

  return 0;
}

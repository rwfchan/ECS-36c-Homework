#include <algorithm>
#include <iostream>
#include <vector>

#include "llrb_set.h"

// Tester
int main() {
  LLRB_set<int> set;
  std::vector<int> keys{2, 18, 42, 43, 51, 54, 74, 93, 99};

  // Insert a bunch of keys
  for (auto i : keys) {
    set.Insert(i);
  }
  std::cout << "After insertions:" << std::endl;
  set.Print();

  // Delete all the keys in another order
  std::random_shuffle(keys.begin(), keys.end());
  for (auto i : keys) {
    set.Remove(i);
  }
  std::cout << "After deletions:" << std::endl;
  set.Print();

  return 0;
}

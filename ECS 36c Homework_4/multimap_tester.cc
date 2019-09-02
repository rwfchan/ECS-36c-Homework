#include <algorithm>
#include <iostream>
#include <vector>

#include "llrb_multimap.h"

// Tester
int main() {
  LLRB_multimap<int, int> multimap;
  std::vector<int> keys1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> values1{2, 18, 42, 43, 51, 54, 74, 93, 99};

  std::cout << "Check error if the tree is empty" << std::endl;
  try {
    multimap.Get(5);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  // Insert a bunch of keys
  for (unsigned int i = 0; i < 9; i++) {
    multimap.Insert(keys1.at(i), values1.at(i));
  }

  std::cout << std::endl;
  std::cout << "After insertions:" << std::endl;
  multimap.Print();

  std::cout << std::endl;
  std::cout << "Check error if the key is invalid" << std::endl;
  try {
    multimap.Get(99);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Check if the program could detect existing key "
            << "and insert duplicate key into existing key" << std::endl;
  std::cout << "and the Get function would returns the first items of key 8"
            << std::endl;
  std::cout << "Before inserting key 8: " << std::endl;
  std::cout << "The first item in key 8 contain the value " << multimap.Get(8)
            << std::endl;
  std::cout << "Inserting key 8..." << std::endl;
  multimap.Insert(8, 100);
  std::cout << "The first item in key 8 contain the value " << multimap.Get(8)
            << std::endl;
  multimap.Print();
  
  // Tester #2
  std::cout << std::endl;
  std::cout << "Tester #2, inserting duplicate keys for all elements in map"
            << std::endl;

  std::vector<int> keys2{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> values2{5, 6, 7, 8, 12, 39, -49, 19, -2};

  for (unsigned int i = 0; i < 9; i++) {
    multimap.Insert(keys2.at(i), values2.at(i));
  }

  std::cout << "After double insertions:" << std::endl;
  multimap.Print();

  std::cout << std::endl;
  std::cout << "Inserting unmatched value in already existed key" << std::endl;
  std::cout << "Inserting char 'a' into key 8" << std::endl;
  std::cout << "Inserting bool true into key 8" << std::endl;
  multimap.Insert(8, 'a');
  multimap.Insert(8, true);
  multimap.Print();

  // Delete all the keys in another order
  std::random_shuffle(keys1.begin(), keys1.end());
  for (auto i : keys1) {
    multimap.Remove(i);
  }

  std::cout << std::endl;
  std::cout << "After first time deletions:" << std::endl;
  multimap.Print();

  // Delete all the keys again
  std::random_shuffle(keys1.begin(), keys1.end());
  for (auto i : keys1) {
    multimap.Remove(i);
  }

  std::cout << std::endl;
  std::cout << "After second time deletions:" << std::endl;
  multimap.Print();

  // Delete the rest of the keys
  std::random_shuffle(keys1.begin(), keys1.end());
  for (auto i : keys1) {
    multimap.Remove(i);
  }

  std::cout << std::endl;
  std::cout << "After third time deletions:" << std::endl;
  multimap.Print();

  return 0;
}
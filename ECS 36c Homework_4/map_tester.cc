#include <algorithm>
#include <iostream>
#include <vector>

#include "llrb_map.h"

// Tester
int main() {
  LLRB_map<int, int> map;
  std::vector<int> keys1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> values1{2, 18, 42, 43, 51, 54, 74, 93, 99};

  std::cout << "Check error if the tree is empty" << std::endl;
  try {
    map.Get(5);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  // Insert a bunch of keys
  for (unsigned int i = 0; i < 9; i++) {
    map.Insert(keys1.at(i), values1.at(i));
  }

  std::cout << std::endl;
  std::cout << "After insertions:" << std::endl;
  map.Print();

  std::cout << std::endl;
  std::cout << "Check error if the key is invalid" << std::endl;
  try {
    map.Get(99);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Check if the Get function actually works" << std::endl;
  std::cout << "The key 8 contain the value " << map.Get(8) << std::endl;

  std::cout << std::endl;
  std::cout << "Check if the program could detect error when "
      "trying to insert existing key" << std::endl;
  try {
    std::cout << "Inserting key 8" << std::endl;
    map.Insert(8, 100);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  // Delete all the keys in another order
  std::random_shuffle(keys1.begin(), keys1.end());
  for (auto i : keys1) {
    map.Remove(i);
  }

  std::cout << std::endl;
  std::cout << "After deletions:" << std::endl;
  map.Print();


// Tester #2
  std::cout << std::endl;
  std::cout << "Tester #2" << std::endl;

  std::vector<int> keys2{51, 43, 42, 93, 54, 99, 71};
  std::vector<int> values2{2, 18, 42, 43, 51, 54, 74};

  for (unsigned int i = 0; i < 7; i++) {
    map.Insert(keys2.at(i), values2.at(i));
  }

  std::cout << "After insertions:" << std::endl;
  map.Print();

  std::cout << std::endl;
  std::cout << "Test the min node in removal target's right "
      "substree replaced the removal target" << std::endl;
  std::cout << "The key 51 contains " << map.Get(51) << std::endl;
  std::cout << "Removing key 51..." << std::endl;
  map.Remove(51);
  std::cout << "After removing key 51" << std::endl;
  map.Print();
  try {
    map.Get(51);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

  // Since there is no need to return error if node not
  // found when removing this for loop should run fine
  for (auto i : keys2) {
    map.Remove(i);
  }

  std::cout << std::endl;
  std::cout << "After deletions:" << std::endl;
  map.Print();

  return 0;
}

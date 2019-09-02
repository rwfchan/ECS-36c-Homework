// Copyright 2018 <Raymond Chan>
// Prototypes provided by Prof. Joël Porquet

#include <algorithm>
#include <iostream>
#include <random>

#include "bst.h"

int main() {
  std::vector<int> keys{51, 43, 93, 18, 42, 99, 54, 2, 74};

  BST<int> bst;

//  Using runtime_error for Empty Tree
//  because this was detected during run time,
//    when the program tries to access the tree
//    but could not find anything to do with it
  try {
    bst.ceil(42);
  } catch (std::runtime_error &e) {
    std::cout << "Expected exception: " << e.what() << std::endl;
  }

  for (auto i : keys)
    bst.insert(i);

  /* Test floor */
  std::cout << "ceil(54) = " << bst.ceil(54) << "\n";
  std::cout << "ceil(55) = " << bst.ceil(55) << "\n";
  std::cout << "ceil(99) = " << bst.ceil(99) << "\n";
  std::cout << "ceil(98) = " << bst.ceil(99) << "\n";
  std::cout << "ceil(2) = " << bst.ceil(2) << "\n";
  std::cout << "ceil(3) = " << bst.ceil(3) << "\n";
  std::cout << "ceil(41) = " << bst.ceil(41) << "\n";
  std::cout << "ceil(50) = " << bst.ceil(50) << "\n";
  std::cout << "ceil(74) = " << bst.ceil(74) << "\n";
  std::cout << "ceil(93) = " << bst.ceil(93) << "\n";
  std::cout << "ceil(94) = " << bst.ceil(94) << "\n";

//  Using overflow_error for ceiling input being too big,
//    because the input is mathematically too big for the program
//      to do anything with it
  try {
    bst.ceil(100);
  } catch (std::overflow_error &e) {
    std::cout << "Expected exception: " << e.what() << std::endl;
  }

  /* Test ceil */
  std::cout << "floor(54) = " << bst.floor(54) << "\n";
  std::cout << "floor(55) = " << bst.floor(55) << "\n";
  std::cout << "floor(51) = " << bst.floor(51) << "\n";
  std::cout << "floor(50) = " << bst.floor(50) << "\n";
  std::cout << "floor(2) = " << bst.floor(2) << "\n";
  std::cout << "floor(99) = " << bst.floor(99) << "\n";
  std::cout << "floor(98) = " << bst.floor(98) << "\n";
  std::cout << "floor(3) = " << bst.floor(3) << "\n";
  std::cout << "floor(73) = " << bst.floor(73) << "\n";
  std::cout << "floor(74) = " << bst.floor(74) << "\n";
  std::cout << "floor(75) = " << bst.floor(75) << "\n";

//  Using underflow_error for floor input being too small,
//    because the input is mathematically too small for the program
//      to do anything with it
  try {
    bst.floor(0);
  } catch (std::underflow_error &e) {
    std::cout << "Expected exception: " << e.what() << std::endl;
  }

  /* Test kth_small */
  std::cout << "kth(1) = " << bst.kth_small(1) << "\n";
  std::cout << "kth(5) = " << bst.kth_small(5) << "\n";
  std::cout << "kth(7) = " << bst.kth_small(7) << "\n";
  std::cout << "kth(9) = " << bst.kth_small(9) << "\n";

//  Using out of range error for invalid kth_small input
//    because is our case, invalid input for kth_small means
//      the input is either larger or smaller than the number of keys
//    In other word, any numbers that are invalid should be
//       considered as out of the range of the tree
  try {
    bst.kth_small(10);
  } catch (std::out_of_range &e) {
    std::cout << "Expected exception: " << e.what() << std::endl;
  }
  try {
    bst.kth_small(100);
  } catch (std::out_of_range &e) {
    std::cout << "Expected exception: " << e.what() << std::endl;
  }

  return 0;
}

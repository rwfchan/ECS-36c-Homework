//  Copyright 2018 <Raymond Chan>

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>
#include <list>
#include "binary_tree.h"

/*
 *  This function read through structure list and put items into tree accordingly
 */
void recur_tree(Node* n, std::list<int>& structure, std::list<int>& items) {
  //  Base case when finished reading the whole structure list
  if (structure.empty())
    return;

  structure.pop_front();

  //  First, always check if any node for the left child
  //  If structure list says 1, insert node
  if (structure.front()) {
    std::unique_ptr<Node> new_node_left
      (new BinaryTree<int>::Node{items.front()});
    items.pop_front();

    n->left = std::move(new_node_left);
    recur_tree(n->left.get(), structure, items);

  //  else, skip to next element in structure list
  } else {
    structure.pop_front();
  }

  //  Then, check if any node for the right child
  //  If structure list says 1, insert node
  if (structure.front()) {
    std::unique_ptr<Node> new_node_right
      (new BinaryTree<int>::Node{items.front()});
    items.pop_front();

    n->right = std::move(new_node_right);
    recur_tree(n->right.get(), structure, items);

  //  else, skip to next element in structure list
  } else {
    structure.pop_front();
  }
}

/*
 *  Start by calling this function to make tree
 *    This function read in structure and items of the trees
 *    Then return a newly assembled b_tree
 */
BinaryTree<int> make_tree(std::list<int>& structure, std::list<int>& items) {
  BinaryTree<int> b_tree;

  //  If structure is 1 on the first element, put node in root
  if (structure.front()) {
    std::unique_ptr<Node> new_node(new BinaryTree<int>::Node{items.front()});
    items.pop_front();

    b_tree.root = std::move(new_node);

  //  Call recursive function for building the tree
    recur_tree(b_tree.root.get(), structure, items);

  //  else, the tree should be empty
  } else {
    b_tree.root = nullptr;
  }

  //  No matter if the tree is empty or not, return tree
  return b_tree;
}

//  From postfix_eval.cc by Raymond Chan
//  Tokenize read in string to tokenize, and return list of integer
std::list<int> tokenize(const std::string& string) {
  std::list<int> list;
  std::string::size_type prev_pos = 0, pos = 0;

  //  Keep finding ' ' in the string until the end of string
  //    ' ' signify the end of a token
  while ((pos = string.find(' ', pos)) != std::string::npos) {
    //  Store token in substring
    std::string substring(string.substr(prev_pos, pos - prev_pos));

    //  If no token was read, continue
    if (substring.empty()) {
      prev_pos = ++pos;
      continue;
    }

    //  Convert token to integer, and put it into the list
    std::stringstream convert(substring);
    int input;
    if (convert >> input) {
      list.push_back(input);
    } else {
      std::cerr << "Problem with tokenization" << std::endl;
    }
    prev_pos = ++pos;
  }

  return list;
}

/*
 * Error checking function for structre and items input
 */
bool check_structure_input(const std::list<int>& structure) {
  for (auto boolean : structure) {
    if (boolean > 1) {
      return false;
    }
  }
  return true;
}
bool check_items_input(const std::list<int>& items, 
                       const std::list<int>& structure) {
  unsigned int length = 0;
  for (auto boolean : structure) {
    if (boolean) {
      length++;
    }
  }
  return items.size() == length;
}

int main(int argc, char **argv) {
  //  Error for wrong usage
  if (argc < 2) {
    std::cerr << "Usage: ./bt_succinct_dec <input_file>" << std::endl;
    return 1;
  }

  std::string structure_string;
  std::string items_string;

  //  Reading file
  std::ifstream myfile;
  myfile.open(argv[1]);

  if (!myfile.is_open()) {
    std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
    return 1;
  }

  //  If nothing in the file,
  //    print wrong structure line error and return 1
  if (!std::getline(myfile, structure_string)) {
    std::cerr << "Error: cannot read structure line" << std::endl;
    myfile.close();
    return 1;
  }
  structure_string += ' ';

  //  If only one line in the file,
  //    print wrong data line error and return 1
  if (!std::getline(myfile, items_string)) {
    std::cerr << "Error: cannot read data line" << std::endl;
    myfile.close();
    return 1;
  }
  items_string += ' ';
  myfile.close();

  //  Tokenize two strings and put them into respective lists
  std::list<int> structure = tokenize(structure_string);
  if (!check_structure_input(structure)) {
    std::cerr << "Error: structure line is invalid" << std::endl;
    return 1;
  }
  std::list<int> items = tokenize(items_string);
  if (!check_items_input(items, structure)) {
    std::cerr << "Error: data line is invalid" << std::endl;
    return 1;
  }

  // Make the tree based on input and print them out
  BinaryTree<int> b_tree = make_tree(structure, items);
  b_tree.PreorderPrint();
  b_tree.InorderPrint();

  return 0;
}

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
void recur_tree(Node* n, std::ifstream& structure_file,
                std::ifstream& item_file) {
  if (!item_file.is_open()) {
    return;
  }
  if (!structure_file.is_open()) {
    return;
  }

  int structure_item;

  //  First, always check if any node for the left child
  //  If structure list says 1, insert node
  structure_file >> structure_item;

  //  If structure have non 0 or 1, close structure_file
  if (structure_item > 1 || structure_item < 0) {
    structure_file.close();
    return;
  }
  if (structure_item) {
    int item;

    //  if item_file put non integer or nothing into item, close item_file
    if (!(item_file >> item)) {
      item_file.close();
      return;
    }
    std::unique_ptr<Node> new_node_left(new BinaryTree<int>::Node{item});

    n->left = std::move(new_node_left);
    recur_tree(n->left.get(), structure_file, item_file);
  }

  //  Then, check if any node for the right child
  //  If structure list says 1, insert node
  structure_file >> structure_item;

  //  If structure have non 0 or 1, close structure_file
  if (structure_item > 1 || structure_item < 0) {
    structure_file.close();
    return;
  }
  if (structure_item) {
    int item;

    //  if item_file put non integer into item, than data line is invalid
    if (!(item_file >> item)) {
      item_file.close();
      return;
    }
    std::unique_ptr<Node> new_node_right(new BinaryTree<int>::Node{item});

    n->right = std::move(new_node_right);
    recur_tree(n->right.get(), structure_file, item_file);
  }
}

int main(int argc, char **argv) {
  //  Error for wrong usage
  if (argc < 2) {
    std::cerr << "Usage: ./bt_succinct_dec <input_file>" << std::endl;
    return 1;
  }

  BinaryTree<int> b_tree;

  //  Reading file twice
  //  One for structure line the other for data line
  std::ifstream structure_file;
  structure_file.open(argv[1]);
  if (!structure_file.is_open()) {
    std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
    return 1;
  }

  std::ifstream item_file;
  std::string filer;
  item_file.open(argv[1]);
  if (!std::getline(item_file, filer)) {
    std::cerr << "Error: cannot read structure line" << std::endl;
    structure_file.close();
    item_file.close();
    return 1;
  }

  int structure_item = -1;
  //  Structure line error if the the item is neither 1 nor 0
  if (!(structure_file >> structure_item) || structure_item > 1 || structure_item < 0) {
    std::cerr << "Error: structure line is invalid" << std::endl;
    structure_file.close();
    item_file.close();
    return 1;
  }
  if (structure_item) {
    int item;
    if (item_file >> item) {
      std::unique_ptr<Node> new_node(new BinaryTree<int>::Node{item});
      b_tree.root = std::move(new_node);

      recur_tree(b_tree.root.get(), structure_file, item_file);
    } else if (item_file.eof()) {
      std::cerr << "Error: cannot read data line" << std::endl;
      structure_file.close();
      item_file.close();
      return 1;
    } else {
      std::cerr << "Error: data line is invalid" << std::endl;
      structure_file.close();
      item_file.close();
      return 1;
    }
  }

  //  There are too little or
  //    invalid item in data line if program had closed item_file
  //  There are too many items in data line item_file.get() returns actual value
  if (!item_file.is_open() || item_file >> structure_item) {
    std::cerr << "Error: data line is invalid" << std::endl;
    structure_file.close();
    return 1;
  }
  if (!structure_file.is_open()) {
    std::cerr << "Error: structure line is invalid" << std::endl;
    item_file.close();
    return 1;
  }
  structure_file.close();
  item_file.close();

  b_tree.PreorderPrint();
  b_tree.InorderPrint();

  return 0;
}

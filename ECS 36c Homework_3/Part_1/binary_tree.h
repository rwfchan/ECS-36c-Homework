// Copyright 2018 <Raymond Chan>
// Prototypes provided by Prof. Joël Porquet

#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

#include <iostream>
#include <memory>

/*
 * Class definition
 */
template <typename T>
class BinaryTree {
 public:
  struct Node{
    T item;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };
  std::unique_ptr<Node> root;

  void PreorderPrint();
  void InorderPrint();

 private:
  void PreorderRecur(Node *n);
  void InorderRecur(Node *n);
};

/*
 * Implementation
 */
template <typename T>
void BinaryTree<T>::PreorderPrint() {
  std::cout << "Preorder: ";
  PreorderRecur(root.get());
  std::cout << std::endl;
}

template <typename T>
void BinaryTree<T>::InorderPrint() {
  std::cout << "Inorder: ";
  InorderRecur(root.get());
  std::cout << std::endl;
}

template <typename T>
void BinaryTree<T>::PreorderRecur(Node *n) {
  if (!n) return;
  std::cout << n->item << ' ';
  PreorderRecur(n->left.get());
  PreorderRecur(n->right.get());
}

template <typename T>
void BinaryTree<T>::InorderRecur(Node *n) {
  if (!n) return;
  InorderRecur(n->left.get());
  std::cout << n->item << ' ';
  InorderRecur(n->right.get());
}

//  From Lecture Slide 05-trees, Pg.32
using Node = BinaryTree<int>::Node;

/* Function declaration for tree making functions */
BinaryTree<int> make_tree (std::list<int>& structure, std::list<int>& items);
void recur_tree(Node* n, std::list<int>& structure, std::list<int>& items);

#endif /* BINARY_TREE_H_ */

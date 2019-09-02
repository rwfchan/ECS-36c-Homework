// Copyright 2018 <Raymond Chan>
// Prototypes provided by Prof. Joël Porquet

#ifndef BST_H_
#define BST_H_

#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>
#include <stack>
#include <queue>
#include <exception>
#include <sstream>
#include <string>

/*
 * Class definition
 */
template <typename T>
class BST {
 public:
  /* Return floor key in tree */
  const T& floor(const T &key);
  /* Return ceil key in tree */
  const T& ceil(const T &key);
  /* Return k-th smallest key in tree */
  const T& kth_small(const int kth);

  /* Return whether @key is found in tree */
  bool contains(const T& key);

  /* Return max key in tree */
  const T& max();
  /* Return min key in tree */
  const T& min();

  /* Insert @key in tree */
  void insert(const T &key);
  /* Remove @key from tree */
  void remove(const T &key);

  /* Print tree in-order */
  void print();

 private:
  struct Node{
    T key;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
  };
  std::unique_ptr<Node> root;

  /* Useful recursive helper methods */
  void insert(std::unique_ptr<Node> &n, const T &key);
  void remove(std::unique_ptr<Node> &n, const T &key);
  void print(Node *n, int level);

  /* Recursive functions for creating Queue */
  void queueing_tree(std::unique_ptr<Node> &n, std::queue<T> &tree_queue);
};

/*
 * Implementation
 */
template <typename T>
const T& BST<T>::floor(const T &key) {
  if (!root) throw std::runtime_error("Empty tree");
  Node* n = root.get();
  const T* min_item = &min();

  while (n) {
    if (n->key <= key) {  //  check if current key is "flooring" key
      min_item = &n->key;
    }

    //  traverse the tree through sizes
    if (n->key > key) {
      n = n->left.get();
    } else {
      n = n->right.get();
    }
  }

  //  Throw underflow error if key is still smaller than max_item
  if (key < *min_item) {
    std::string error_string = "Cannot find floor for key ";
    std::string integer = std::to_string(key);
    throw std::underflow_error(error_string + integer);
  }

  return *min_item;
}

template <typename T>
const T& BST<T>::ceil(const T &key) {
  if (!root) throw std::runtime_error("Empty tree");
  Node* n = root.get();
  const T* max_item = &max();

  while (n) {
    if (!n->left) {  //  check to see if left is nullptr
      if (n->key >= key) {  //  then check if current key could be max
        max_item = &n->key;
      }
    } else if (n->left->key <= key && n->key >= key) {
      //  left is available and check compare current key and key
      max_item = &n->key;
    }
    //  traverse the tree through sizes
    if (n->key >= key)
      n = n->left.get();
    else
      n = n->right.get();
  }

  //  Throw overflow error if key is still bigger than max_item
  if (key > *max_item) {
    std::string error_string = "Cannot find ceil for key ";
    std::string integer = std::to_string(key);
    throw std::overflow_error(error_string + integer);
  }


  return *max_item;
}

template <typename T>
const T& BST<T>::kth_small(const int kth) {
  //  We can use queue for find kth smallest
  //  Because we can pop out k-1 numbers of items from queue
  std::queue<T> ordered_list;
  queueing_tree(root, ordered_list);

  //  Error for empty tree
  if (ordered_list.empty()) {
    throw std::runtime_error("Empty tree");
  }

  //  Once finished popping,
  //    the front of queue should be the item we are looking for
  for (int i = kth - 1; i; i--) {
    ordered_list.pop();

    //  Error if the input excess number of elements hold by tree
    //    or that input is less than 0
    if (ordered_list.empty()) {
      std::string error_string = "Cannot find ";
      std::string integer = std::to_string(kth);
      std::string error_string_2 = error_string + integer;
      throw std::out_of_range(error_string_2 + "th element");
    }
  }
  return ordered_list.front();
}

/*
 * Simple recur function to store trees into a queue from least to greatest
 *  Very similar to in-order traversing
 */
template <typename T>
void BST<T>::queueing_tree(std::unique_ptr<Node> &n,
                           std::queue<T> &tree_queue) {
  if (!n) {
    return;
  }

  queueing_tree(n->left, tree_queue);
  tree_queue.push(n->key);
  queueing_tree(n->right, tree_queue);
}

/*
 * @@@ Code below should not be modified @@@
 */
template <typename T>
bool BST<T>::contains(const T &key) {
  Node *n = root.get();

  while (n) {
    if (key == n->key)
      return true;

    if (key < n->key)
      n = n->left.get();
    else
      n = n->right.get();
  }

  return false;
}

// Changed the implementation of max and min
template <typename T>
const T& BST<T>::max(void) {
  if (!root) throw std::runtime_error("Empty tree");
  Node *n = root.get();
  while (n->right) n = n->right.get();
  return n->key;
}

template <typename T>
const T& BST<T>::min(void) {
  if (!root) throw std::runtime_error("Empty tree");
  Node *n = root.get();
  while (n->left) n = n->left.get();
  return n->key;
}

template <typename T>
void BST<T>::insert(const T &key) {
  insert(root, key);
}

template <typename T>
void BST<T>::insert(std::unique_ptr<Node> &n, const T &key) {
  if (!n)
    n = std::unique_ptr<Node>(new Node{key});
  else if (key < n->key)
    insert(n->left, key);
  else if (key > n->key)
    insert(n->right, key);
  else
    std::cerr << "Key " << key << " already inserted!\n";
}

template <typename T>
void BST<T>::remove(const T &key) {
  remove(root, key);
}

template <typename T>
void BST<T>::remove(std::unique_ptr<Node> &n, const T &key) {
  /* Key not found */
  if (!n) return;

  if (key < n->key) {
    remove(n->left, key);
  } else if (key > n->key) {
    remove(n->right, key);
  } else {
    /* Found node */
    if (n->left && n->right) {
      /* Two children: replace with min node in right subtree */
      n->key = min(n->right.get())->key;
      remove(n->right, n->key);
    } else {
      /* Replace with only child or with nullptr */
      n = std::move((n->left) ? n->left : n->right);
    }
  }
}

template <typename T>
void BST<T>::print() {
  if (!root) return;
  print(root.get(), 1);
  std::cout << std::endl;
}

template <typename T>
void BST<T>::print(Node *n, int level) {
  if (!n) return;

  print(n->left.get(), level + 1);
  std::cout << n->key
            << " (" << level << ") ";
  print(n->right.get(), level + 1);
}

#endif  //  BST_H_

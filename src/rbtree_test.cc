#include "rbtree.h"

#include "glog/logging.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include <functional>

using namespace std;

TEST(RBTreeInt, CheckBinaryTreeTrivial) {
  trilib::RBTree<int, less<int>> rbtree;
  rbtree.Insert(12);
  EXPECT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting 12";
  rbtree.Insert(13);
  EXPECT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting 13";
  rbtree.Insert(11);
  EXPECT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting 11";
}

TEST(RBTreeInt, CheckInsertIncreasing) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 1024;
  for (int i = 0; i != insert_size; ++i) {
    rbtree.Insert(i);
    ASSERT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting " << i;
  }
}

TEST(RBTreeInt, CheckInsertDecreasing) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 1024;
  for (int i = insert_size; i != 0 ; --i) {
    rbtree.Insert(i);
    ASSERT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting " << i;
  }
}

TEST(RBTreeInt, CheckInsertPerm) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 1024;
  constexpr int my_prime = 104729;
  constexpr int iter_val = 56789;
  int val = 0;
  for (int i = insert_size; i != 0 ; --i) {
    val += iter_val;
    if (val >= my_prime) {
      val -= my_prime;
    }
    rbtree.Insert(i);
    ASSERT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting " << i;
  }
}

TEST(RBTreeInt, CheckBigInsert) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 1024000;
  for (int i = insert_size; i != 0 ; --i) {
    rbtree.Insert(i);
  }
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
}

TEST(RBTreeInt, CheckBigInsertPerm) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int my_prime = 30402457;
  constexpr int iter_val = 5678910;
  constexpr int insert_size = 1024000;
  int val = 0;
  for (int i = insert_size; i != 0 ; --i) {
    val += iter_val;
    if (val >= my_prime) {
      val -= my_prime;
    }
    rbtree.Insert(i);
  }
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
}
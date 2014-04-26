#include "rbtree.h"

#include "glog/logging.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <algorithm>
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
  ASSERT_TRUE(rbtree.IsBlackProperty());
}

TEST(RBTreeInt, CheckInsertIncreasing) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 1024;
  for (int i = 0; i != insert_size; ++i) {
    rbtree.Insert(i);
    ASSERT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting " << i;
  }
  ASSERT_TRUE(rbtree.IsBlackProperty());
}

TEST(RBTreeInt, CheckInsertDecreasing) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 1024;
  for (int i = insert_size; i != 0; --i) {
    rbtree.Insert(i);
    ASSERT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting " << i;
  }
  ASSERT_TRUE(rbtree.IsBlackProperty());
}

TEST(RBTreeInt, CheckBigInsert) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 1024000;
  for (int i = insert_size; i != 0; --i) {
    rbtree.Insert(i);
  }
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
}

TEST(RBTreeInt, CheckInsertPerm) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 1024;
  constexpr int my_prime = 104729;
  constexpr int iter_val = 56789;
  int val = 0;
  for (int i = insert_size; i != 0; --i) {
    val += iter_val;
    if (val >= my_prime) {
      val -= my_prime;
    }
    rbtree.Insert(val);
    ASSERT_TRUE(rbtree.IsBinarySearchTree()) << "after inserting " << i;
  }
}

TEST(RBTreeInt, CheckInsertDeletePerm) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int insert_size = 15;
  constexpr int my_prime = 31;
  constexpr int iter_val = 7;

  int val = 0;
  for (int i = insert_size; i != 0; --i) {
    val += iter_val;
    if (val >= my_prime) {
      val -= my_prime;
    }
    ASSERT_EQ(rbtree.end(), rbtree.Search(val));
    rbtree.Insert(val);
  }
  ASSERT_TRUE(rbtree.IsBinarySearchTree());

  val = 0;
  for (int i = insert_size; i != 0; --i) {
    val += iter_val;
    if (val >= my_prime) {
      val -= my_prime;
    }
    auto iter = rbtree.Search(val);
    ASSERT_EQ(val, *iter);
    ASSERT_NE(iter, rbtree.end());
    rbtree.Delete(iter);
    ASSERT_TRUE(rbtree.IsBlackProperty()) << "after deleting " << val;
  }
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
}

TEST(RBTreeInt, CheckBigInsertPerm) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int my_prime = 30402457;
  constexpr int iter_val = 5678910;
  constexpr int insert_size = 1024000;
  int val = 0;
  for (int i = insert_size; i != 0; --i) {
    val += iter_val;
    if (val >= my_prime) {
      val -= my_prime;
    }
    rbtree.Insert(val);
  }
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
}

// TEST(RBTreeInt, CheckVeryBigInsertRemovePerm) {
//  trilib::RBTree<int, less<int>> rbtree;
//  constexpr int my_prime = 30402457;
//  constexpr int iter_val = 5678910;
//  constexpr int insert_size = 3000000;
//
//  int val = 0;
//  for (int i = insert_size; i != 0; --i) {
//    val += iter_val;
//    if (val >= my_prime) {
//      val -= my_prime;
//    }
//    rbtree.Insert(val);
//  }
//  ASSERT_TRUE(rbtree.IsBinarySearchTree());
//
//  val = 0;
//  for (int i = insert_size; i != 0; --i) {
//    val += iter_val;
//    if (val >= my_prime) {
//      val -= my_prime;
//    }
//    auto iter = rbtree.Search(val);
//    ASSERT_EQ(val, *iter);
//    ASSERT_NE(iter, rbtree.end());
//    rbtree.Delete(iter);
//  }
//  ASSERT_TRUE(rbtree.IsBinarySearchTree());
//}

int factorial(int x, int result = 1) {
  if (x == 1)
    return result;
  else
    return factorial(x - 1, x * result);
}

TEST(RBTreeInt, AllPermInsertDelete) {
  trilib::RBTree<int, less<int>> rbtree;
  constexpr int size = 4;
  int values[size];
  int del_values[size];
  for (int i = 0; i < size; ++i) {
    values[i] = i;
    del_values[i] = i;
  };

  int perm_size = factorial(size);
  for (int perm_ins = 0; perm_ins < perm_size; ++perm_ins) {
    for (int perm = 0; perm < perm_size; ++perm) {
      for (int i = 0; i < size; ++i) {
        rbtree.Insert(values[i]);
      }
      ASSERT_TRUE(rbtree.IsBinarySearchTree());
      for (int i = 0; i < size; ++i) {
        rbtree.Delete(rbtree.Search(del_values[i]));
        ASSERT_TRUE(rbtree.IsBlackProperty());
      }
      ASSERT_TRUE(rbtree.IsBinarySearchTree());
      std::next_permutation(del_values, del_values + size);
    }
    std::next_permutation(values, values + size);
  }
}

class IteratorFixture : public ::testing::Test {
 protected:
  virtual void SetUp() {
    for (int i = 0; i < elems_size; ++i) {
      rbtree.Insert(elems[i]);
    }
  }
  static const int elems[];
  static const int elems_size;
  trilib::RBTree<int, less<int>> rbtree;
};
const int IteratorFixture::elems[] = {10, 12, 14};
const int IteratorFixture::elems_size = 3;

TEST_F(IteratorFixture, IsBinarySearchTree) {
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
}

TEST_F(IteratorFixture, IterBegin) {
  trilib::RBTree<int, less<int>>::iterator iter = rbtree.begin();
  ASSERT_EQ(10, *iter) << ".begin()";
}

TEST_F(IteratorFixture, IterPrePostNeEqIncrement) {
  trilib::RBTree<int, less<int>>::iterator iter = rbtree.begin();
  ++iter;
  ASSERT_EQ(12, *iter) << "preincrement";

  iter++;
  ASSERT_EQ(14, *iter) << "postincrement";

  ASSERT_NE(iter, rbtree.end()) << "not equal (!=)";

  ++iter;
  ASSERT_EQ(iter, rbtree.end()) << "equal (==)";
}

TEST_F(IteratorFixture, IterLooping) {
  int iter_counter = 0;
  for (auto x : rbtree) {
    ASSERT_EQ(x, elems[iter_counter]);
    ASSERT_GE(elems_size, ++iter_counter);
  }

  iter_counter = 0;
  for (trilib::RBTree<int, less<int>>::iterator iter = rbtree.begin();
       iter != rbtree.end(); ++iter) {
    ASSERT_EQ(*iter, elems[iter_counter]);
    ASSERT_GE(elems_size, ++iter_counter);
  }
}

TEST_F(IteratorFixture, LowerBound) {
  ASSERT_EQ(10, *rbtree.LowerBound(9));
  ASSERT_EQ(12, *rbtree.LowerBound(10));
  ASSERT_EQ(12, *rbtree.LowerBound(11));
  ASSERT_EQ(14, *rbtree.LowerBound(12));
  ASSERT_EQ(rbtree.begin(), rbtree.LowerBound(9));
  ASSERT_EQ(rbtree.end(), rbtree.LowerBound(15));
}

TEST_F(IteratorFixture, UpperBound) {
  ASSERT_EQ(14, *rbtree.UpperBound(15));
  ASSERT_EQ(12, *rbtree.UpperBound(13));
  ASSERT_EQ(10, *rbtree.UpperBound(12));
  ASSERT_EQ(10, *rbtree.UpperBound(11));
  ASSERT_EQ(10, *rbtree.UpperBound(11));
  //  ASSERT_EQ(rbtree.end(), rbtree.UpperBound(15));
  ASSERT_EQ(rbtree.end(), rbtree.UpperBound(0));
}

TEST(RBTreeInt, DeleteOnlyOne) {
  trilib::RBTree<int, less<int>> rbtree;
  rbtree.Insert(12);
  auto iter = rbtree.begin();
  rbtree.Delete(iter);
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
  ASSERT_TRUE(rbtree.IsBlackProperty());
}

TEST(RBTreeInt, Destructor) {
  trilib::RBTree<int, less<int>> rbtree;
  rbtree.Insert(12);
  rbtree.Insert(13);
  rbtree.Insert(11);
  auto iter = rbtree.begin();
}

TEST(RBTreeInt, DeleteLeaves) {
  trilib::RBTree<int, less<int>> rbtree;
  rbtree.Insert(12);
  rbtree.Insert(11);
  auto iter = rbtree.begin();
  rbtree.Delete(iter);
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
  ASSERT_TRUE(rbtree.IsBlackProperty());

  rbtree.Insert(13);
  iter = rbtree.begin();
  ++iter;
  rbtree.Delete(iter);
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
  ASSERT_TRUE(rbtree.IsBlackProperty());
}

TEST(RBTreeInt, DeleteRoot) {
  trilib::RBTree<int, less<int>> rbtree;
  rbtree.Insert(12);
  rbtree.Insert(11);
  rbtree.Insert(13);
  auto iter = rbtree.begin();
  ++iter;  // 12
  EXPECT_EQ(12, *iter);
  rbtree.Delete(iter);
  ASSERT_TRUE(rbtree.IsBinarySearchTree());
  ASSERT_TRUE(rbtree.IsBlackProperty());
}

class FullTreeFixture : public ::testing::Test {
 protected:
  virtual void SetUp() {
    for (int i = 0; i < elems_size; ++i) {
      rbtree.Insert(elems[i]);
    }
  }
  static const int elems[];
  static const int elems_size;
  trilib::RBTree<int, less<int>> rbtree;
};
const int FullTreeFixture::elems[] = {8, 4, 12, 2, 6,  10, 14, 1,
                                      3, 5, 7,  9, 11, 13, 15};
const int FullTreeFixture::elems_size = 15;

TEST_F(FullTreeFixture, Search) {
  for (int i = 1; i < 16; ++i) {
    ASSERT_EQ(i, *rbtree.Search(i));
  }
  ASSERT_EQ(rbtree.end(), rbtree.Search(0));
  ASSERT_EQ(rbtree.end(), rbtree.Search(16));
}

#define TEST_FULL_TREE_DELETE(num)            \
  TEST_F(FullTreeFixture, Delete##num) {      \
    auto iter = rbtree.begin();               \
    for (int i = 0; i < num; ++i) {           \
      ++iter;                                 \
    }                                         \
    rbtree.Delete(iter);                      \
    ASSERT_TRUE(rbtree.IsBinarySearchTree()); \
    ASSERT_TRUE(rbtree.IsBlackProperty());    \
  }

TEST_FULL_TREE_DELETE(0);
TEST_FULL_TREE_DELETE(1);
TEST_FULL_TREE_DELETE(2);
TEST_FULL_TREE_DELETE(3);
TEST_FULL_TREE_DELETE(4);
TEST_FULL_TREE_DELETE(5);
TEST_FULL_TREE_DELETE(6);
TEST_FULL_TREE_DELETE(7);
TEST_FULL_TREE_DELETE(8);
TEST_FULL_TREE_DELETE(9);
TEST_FULL_TREE_DELETE(10);
TEST_FULL_TREE_DELETE(11);
TEST_FULL_TREE_DELETE(12);
TEST_FULL_TREE_DELETE(13);
TEST_FULL_TREE_DELETE(14);

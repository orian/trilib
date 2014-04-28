#include <trilib/rbtree.h>
#include <functional>
#include <iostream>

using namespace std;

// C++11 feature
using RBTree = trilib::RBTree<int, less<int>>;

int main() {
  RBTree rbtree;
  rbtree.Insert(15);
  for (int i = 0; i < 100; i += 7) {
    rbtree.Insert(i);
  }
  RBTree::iterator iter = rbtree.LowerBound(16);
  cout << *iter << endl;  // 21
  ++iter;
  cout << *iter << endl;                                // 28
  rbtree.Delete(iter);                                  // delete by iterator
  rbtree.Delete(15);                                    // delete by value
  cout << (rbtree.Search(15) == rbtree.end()) << endl;  // 1, value not found
  cout << (rbtree.Search(7) == rbtree.end()) << endl;   // 0, value found
  cout << rbtree.HasValue(7) << endl;                   // 1, value found
  return 0;
}

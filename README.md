trilib - C++ template tree library
======

Red Black Tree as template implementation.

**Status**: work in progress.

### Quick demo

```cpp
// import library
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
```
Compile by:
`gcc -std=c++11 -o demo demo.cc`

If you experience strange errors consider using clang as it gives more readable and helpful error messages than gcc (gcc is a bit better when optimizing code).

To use clang one can:
```bash
# In main project's directory.
source use_clang.sh
# or in build dir
source ../use_clang.sh
```
Remember, a change of compiler require to cleanup build dir.

### Installation and compilation
The must have requirement include `cmake`.
```bash
# get source
git clone https://github.com/orian/trilib.git

# build and install
cd trilib
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make && make install
```

The library by default is installed to `/usr/local/include` if one want to change `CMAKE_INSTALL_PREFIX`:
```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
```
One can check what would be installed by invoking `make install DESTDIR=tmp`
wher `tmp` will be a directory created under current working directory.
One can find more examples in `rbtree_test.cc` file.

The library is tested. To build tests one must have Google Test Library: https://code.google.com/p/googletest/ .
Building and running tests:
```bash
cd trilib
mkdir dev_build
cd dev_build
cmake -DCMAKE_BUILD_TYPE=Dev ..
make all
./bin/rbtree_test
```

#### Few words about implementation

In contrast to the widly adopted implementation, this one doesn't use extra Nil node.

### License

This code is licensed under any (you choose) of license: GPL2 or GPL3 or MIT. One string attached: when you start making money:
* if you are a single developer, donate at least $20 to some educational charity.
* if you are a corporation, donate at least $500 to some educational charity.

Notice: it's a honour based system.

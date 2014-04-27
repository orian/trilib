trilib - C++ template tree library
======

Red Black Tree as template implementation.

### Quick demo

```cpp
// import library
#include "trilib/rbtree.h"
#include <iostream>

int main () { 
  trilib::RBTree<int> rbtree;
  rbtree.Insert(15);
  for (int i=0; i< 100; i+=7) {
    rbtree.Insert(i);
  }
  trilib::RBTree<int>::iterator iter = rbtree.LowerBound(16);
  std::cout << *iter << std::endl;  // 21
  ++iter;
  std::cout << *iter << std::endl;  // 28
  rbtree.Delete(iter);  // delete by iterator
  rbtree.Delete(15);    // delete by value
  std::cout << rbtree.Search(15) == rbtree.end() << std::endl;  // 1, value not found
  std::cout << rbtree.Search(7) == rbtree.end() vstd::endl;     // 0, value found
  std::cout << rbtree.HasValue(7) << std::endl;                 // 0, value found
  return 0;
}
```
Compile by:
`gcc -std=c++11 -o demo demo.cc`

If you experience strange errors consider using clang as it gives more readable and helpful error messages than gcc (gcc is a bit better when optimizing code).

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

### License

For education purposes this code goes under MIT license.

If you want to use it for a business purposes you can choose:

* GPL2 or GPL3 license free of charge.
* MIT license for private person: donate at least $20 to some educational charity.
* MIT license for corporations: donate at least $500 to some educational charity.

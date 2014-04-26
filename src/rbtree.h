#ifndef RBTREE_H_
#define RBTREE_H_

#include <iostream>

namespace trilib {

namespace {

template <typename ValueT>
class RBTreeNode {
 public:
  RBTreeNode()
      : properties(0),
        parent(nullptr),
        left_child(nullptr),
        right_child(nullptr) {}

  RBTreeNode(ValueT value)
      : value_(value),
        properties(0),
        parent(nullptr),
        left_child(nullptr),
        right_child(nullptr) {}

  inline bool IsLeftChild() const { return parent->left_child == this; }

  inline bool SafeIsLeftChild() const {
    return (parent != nullptr && parent->left_child == this);
  }

  inline bool IsRightChild() const { return parent->right_child == this; }

  inline bool SafeIsRightChild() const {
    return (parent != nullptr && parent->right_child == this);
  }

  inline bool HasParent() const { return (parent != nullptr); }

  inline bool HasLeftChild() const { return (left_child != nullptr); }

  inline bool HasRightChild() const { return (right_child != nullptr); }

  void SetColorRed() { properties &= ~1; }
  void SetColorBlack() { properties |= 1; }
  void SetColor(bool is_black) { is_black ? SetColorBlack() : SetColorRed(); }
  bool IsColorRed() const { return (properties & 1) == 0; }
  bool IsColorBlack() const { return properties & 1; }

  ValueT value_;
  // Tree properties
  unsigned int properties;
  RBTreeNode* parent;
  RBTreeNode* left_child;
  RBTreeNode* right_child;
  //
  static const unsigned int kColorRed;
  static const unsigned int kColorBlack;
  static const unsigned int kLeftChild;
  static const unsigned int kRightChild;
};

template <typename ValueT>
const unsigned int RBTreeNode<ValueT>::kColorRed = 1;

template <typename ValueT>
const unsigned int RBTreeNode<ValueT>::kColorBlack = 2;

template <typename ValueT>
const unsigned int RBTreeNode<ValueT>::kLeftChild = 4;

template <typename ValueT>
const unsigned int RBTreeNode<ValueT>::kRightChild = 8;

// helper methods
template <typename ValueT>
const ValueT& GetValue(const RBTreeNode<ValueT>* const ptr) {
  return ptr->value_;
}

template <typename ValueT>
const ValueT& GetValue(const RBTreeNode<ValueT>& ref) {
  return ref.value_;
}

template <typename ValueT, typename CompT>
bool RunCmp(const ValueT* const a, const ValueT* const b, const CompT& comp) {
  return comp(GetValue(a), GetValue(b));
}

template <typename ValueT, typename CompT>
bool RunCmp(const ValueT& a, const ValueT& b, const CompT& comp) {
  return comp(GetValue(a), GetValue(b));
}

template <typename ValueT, typename CompT>
bool CheckIsBinarySearchTree(const ValueT& root) {
  return (!root.HasLeftChild() || (RunCmp(*root.left_child, root, CompT()) &&
                                   CheckIsBinarySearchTree<ValueT, CompT>(
                                       *root.left_child))) &&  // left child
         (!root.HasRightChild() ||
          (RunCmp(root, *root.right_child, CompT()) &&
           CheckIsBinarySearchTree<ValueT, CompT>(*root.right_child)));
}

template <typename ValueT, typename CompT>
bool CheckIsBinarySearchTree(RBTreeNode<ValueT>* root,
                             RBTreeNode<ValueT>* left_bound,
                             RBTreeNode<ValueT>* right_bound,
                             const CompT& cmp) {
  return (is_null(left_bound) || RunCmp(left_bound, root, cmp)) &&
         (is_null(right_bound) || RunCmp(root, right_bound, cmp)) &&
         (!root->HasLeftChild() ||
          CheckIsBinarySearchTree(root->left_child, left_bound, root, cmp)) &&
         (!root->HasRightChild() ||
          CheckIsBinarySearchTree(root->right_child, root, right_bound, cmp));
}

template <typename ValueT, typename CompT>
bool CheckIsBinarySearchTree(const ValueT& root, const CompT& cmp) {
  return (!root.HasLeftChild() ||
          (RunCmp(*root.left_child, root, cmp) &&
           CheckIsBinarySearchTree(*root.left_child, cmp))) &&  // left child
         (!root.HasRightChild() ||
          (RunCmp(root, *root.right_child, cmp) &&
           CheckIsBinarySearchTree(*root.right_child, cmp)));
}

template <typename ValueT>
int BlackToLeaves(RBTreeNode<ValueT>* x) {
  if (is_null(x)) {
    return 0;
  }
  const int left = BlackToLeaves(x->left_child);
  const int right = BlackToLeaves(x->right_child);
  if (left == right) {
    return left + (x->IsColorBlack() ? 1 : 0);
  }
  return -1;
}

template <typename ValueT>
bool CheckBlackEquals(RBTreeNode<ValueT>* x) {
  const int left = BlackToLeaves(x->left_child);
  const int right = BlackToLeaves(x->right_child);
  return left == right;
}

template <typename ValueT>
bool CheckRedHasTwoBlackChildren(RBTreeNode<ValueT>* x) {
  return (!x->IsColorRed() ||
          ((!x->HasLeftChild() || x->left_child->IsColorBlack()) &&
           (!x->HasRightChild() || x->right_child->IsColorBlack()))) &&
         (!x->HasLeftChild() || CheckRedHasTwoBlackChildren(x->left_child)) &&
         (!x->HasRightChild() || CheckRedHasTwoBlackChildren(x->right_child));
}

template <typename ValueT>
void PrintTree(RBTreeNode<ValueT>* x) {
  if (is_null(x)) {
    std::cout << "()";
    return;
  }
  auto m = (x->IsColorBlack() ? "b" : "");
  std::cout << "(" << m << x->value_;
  if (x->HasLeftChild() || x->HasRightChild()) {
    PrintTree(x->left_child);
    std::cout << ",";
    PrintTree(x->right_child);
  }
  std::cout << ")";
}

template <typename ValueT>
RBTreeNode<ValueT>* TreeMinimum(RBTreeNode<ValueT>* x) {
  while (x->HasLeftChild()) {
    x = x->left_child;
  }
  return x;
}

template <typename ValueT>
RBTreeNode<ValueT>* TreeMaximum(RBTreeNode<ValueT>* x) {
  while (x->HasRightChild()) {
    x = x->right_child;
  }
  return x;
}

template <typename ValueT>
RBTreeNode<ValueT>* TreeSuccessor(RBTreeNode<ValueT>* x) {
  if (x->HasRightChild()) {
    return TreeMinimum(x->right_child);
  }
  RBTreeNode<ValueT>* y = x->parent;
  while (y != nullptr && x == y->right_child) {
    x = y;
    y = y->parent;
  }
  return y;
}

template <typename ValueT, typename CompT>
RBTreeNode<ValueT>* LowerBound(const ValueT& val, RBTreeNode<ValueT>* x,
                               const CompT& cmp) {
  RBTreeNode<ValueT>* y = nullptr;
  while (x != nullptr) {
    if (cmp(val, x->value_)) {
      y = x;
      x = x->left_child;
    } else {
      x = x->right_child;
    }
  }
  return y;
}

template <typename ValueT, typename CompT>
RBTreeNode<ValueT>* UpperBound(const ValueT& val, RBTreeNode<ValueT>* x,
                               const CompT& cmp) {
  RBTreeNode<ValueT>* y = nullptr;
  while (x != nullptr) {
    if (cmp(x->value_, val)) {
      y = x;
      x = x->right_child;
    } else {
      x = x->left_child;
    }
  }
  return y;
}

template <typename ValueT>
inline bool is_null(const ValueT& ptr) {
  return ptr == nullptr;
}

}  // namespace

template <typename ValueT, typename CompT>
class RBTree {
 public:
  using RBTreeNodeT = RBTreeNode<ValueT>;

  RBTree() : root_(nullptr), value_cmp_() {}

  ~RBTree() { RemoveTree(root_); }

  // STL like iterators
  class iterator {
   public:
    iterator() : node_(nullptr) {}
    iterator(const iterator& iter) : node_(iter.node_) {}
    iterator(RBTreeNodeT* node) : node_(node) {}
    // PreInc
    iterator& operator++() {
      node_ = trilib::TreeSuccessor(node_);
      return *this;
    }
    // PostInc
    iterator operator++(int) {
      iterator tmp(*this);
      node_ = trilib::TreeSuccessor(node_);
      return tmp;
    }
    ValueT& operator*() { return node_->value_; }
    bool operator==(const iterator& iter) const { return iter.node_ == node_; }
    bool operator!=(const iterator& iter) const { return !(*this == iter); }

   private:
    RBTreeNodeT* node_;
    friend class RBTree;
  };

  iterator begin() { return iterator(TreeMinimum(root_)); }

  iterator end() { return iterator(); }

  // extra methods

  void Insert(ValueT value) {
    RBTreeNodeT* node = BinarySearchInsert(value);
    node->SetColorRed();

    while (true) {
      if (!node->HasParent()) {  // insert_case1
        node->SetColorBlack();
        return;
      } else if (node->parent->IsColorBlack()) {  // insert_case2
        return;
      }
      RBTreeNodeT* uncle = Uncle(node);  // insert_case3
      if (uncle != nullptr && uncle->IsColorRed()) {
        node->parent->SetColorBlack();
        uncle->SetColorBlack();
        RBTreeNodeT* grandparent = GrandParent(node);
        grandparent->SetColorRed();
        // insert_case1(g);
        // return;
        node = grandparent;
        continue;
      } else {  // insert_case4
        if ((node->IsRightChild()) && node->parent->IsLeftChild()) {
          LeftRotate(node->parent);
          node = node->left_child;
        } else if (node->IsLeftChild() && node->parent->IsRightChild()) {
          RightRotate(node->parent);
          node = node->right_child;
        }
        // insert_case5
        RBTreeNodeT* grandparent = GrandParent(node);
        node->parent->SetColorBlack();
        grandparent->SetColorRed();
        if (node->IsLeftChild()) {
          RightRotate(grandparent);
        } else {
          LeftRotate(grandparent);
        }
        return;
      }
    }
  }

  iterator LowerBound(const ValueT& val) {
    return iterator(trilib::LowerBound(val, root_, value_cmp_));
  }

  iterator UpperBound(const ValueT& val) {
    return iterator(trilib::UpperBound(val, root_, value_cmp_));
  }

  iterator Search(const ValueT& value) {
    RBTreeNodeT* ptr = root_;
    while (ptr != nullptr && value != ptr->value_) {
      // DCHECK(ptr != nullptr);
      if (value_cmp_(value, ptr->value_)) {
        ptr = ptr->left_child;
      } else {
        ptr = ptr->right_child;
      }
    }
    return iterator(ptr);
  }

  bool IsBinarySearchTree() const {
    //    return is_null(root_) || CheckIsBinarySearchTree(*root_,
    //    value_cmp_);
    return is_null(root_) || CheckIsBinarySearchTree<ValueT, CompT>(
                                 root_, nullptr, nullptr, value_cmp_);
  }

  bool IsBlackProperty() const {
    return is_null(root_) || CheckBlackEquals(root_);
  }

  bool IsRedHasTwoBlacks() const {
    return is_null(root_) || CheckRedHasTwoBlackChildren(root_);
  }

  void Print() {
    PrintTree(root_);
    std::cout << std::endl;
  }

  void Delete(iterator node) {
    RBTreeNodeT* z = node.node_;
    RBTreeNodeT* y = z;
    bool y_orig_is_black = y->IsColorBlack();
    RBTreeNodeT* x = nullptr;
    bool was_x_right = false;
    if (!z->HasLeftChild()) {
      x = z->parent;
      was_x_right = z->SafeIsRightChild();
      Transplant(z, z->right_child);
    } else if (!z->HasRightChild()) {
      x = z->parent;
      was_x_right = z->SafeIsRightChild();
      Transplant(z, z->left_child);
    } else {                            // z has both parents
      y = TreeMinimum(z->right_child);  // minimum never has left_child
      y_orig_is_black = y->IsColorBlack();
      if (y != z->right_child) {
        x = y->parent;
        Transplant(y, y->right_child);
        y->right_child = z->right_child;
        y->right_child->parent = y;
      } else {
        x = y;  // can x be null? yes.
        was_x_right = true;
      }
      Transplant(z, y);
      y->left_child = z->left_child;  // we know z has left child, and y doesn't
      y->left_child->parent = y;
      y->SetColor(z->IsColorBlack());
    }
    delete z;
    if (y_orig_is_black && x != nullptr) {
      DeleteFixup(x, was_x_right);
    }
  }

 private:
  void RemoveTree(RBTreeNodeT* x) {
    if (x != nullptr) {
      RemoveTree(x->left_child);
      RemoveTree(x->right_child);
      delete x;
    }
  }

  RBTreeNodeT* RightChildDeleteFixup(RBTreeNodeT* x, bool* was_right) {
    // black sibling and has red child
    RBTreeNodeT* sibling = x->left_child;
    if (is_null(sibling)) {
      return nullptr;  // ?
    }
    if (sibling->IsColorRed()) {
      RightRotate(x);
      x->SetColorRed();
      sibling->SetColorBlack();
      sibling = x->left_child;
    }
    if (sibling->HasLeftChild() && sibling->left_child->IsColorRed()) {
      sibling->left_child->SetColorBlack();
      sibling->SetColor(x->IsColorBlack());
      x->SetColorBlack();
      RightRotate(x);
      return nullptr;
    } else if (sibling->HasRightChild() && sibling->right_child->IsColorRed()) {
      sibling->right_child->SetColor(x->IsColorBlack());
      x->SetColorBlack();
      LeftRotate(sibling);
      RightRotate(x);
      return nullptr;
    }
    // we can assume that sibling is black
    if (x->IsColorRed() && sibling->IsColorBlack()) {
      x->SetColorBlack();
      sibling->SetColorRed();
      return nullptr;
    } else if (sibling->IsColorBlack()) {
      sibling->SetColorRed();
      if (x->HasParent()) {
        *was_right = x->IsRightChild();
      }
      x = x->parent;
      return x;
    }
    // TODO decide what to do with unexpected cases assert ?
    return nullptr;
  }

  RBTreeNodeT* LeftChildDeleteFixup(RBTreeNodeT* x, bool* was_right) {
    // black sibling and has red child
    RBTreeNodeT* sibling = x->right_child;
    if (is_null(sibling)) {
      return nullptr;  // ?
    }
    if (sibling->IsColorRed()) {
      LeftRotate(x);
      x->SetColorRed();
      sibling->SetColorBlack();
      sibling = x->right_child;
    }
    if (sibling->HasRightChild() && sibling->right_child->IsColorRed()) {
      sibling->right_child->SetColorBlack();
      sibling->SetColor(x->IsColorBlack());
      x->SetColorBlack();
      LeftRotate(x);
      return nullptr;
    } else if (sibling->HasLeftChild() && sibling->left_child->IsColorRed()) {
      sibling->left_child->SetColor(x->IsColorBlack());
      x->SetColorBlack();
      RightRotate(sibling);
      LeftRotate(x);
      return nullptr;
    }
    // we can assume that sibling is black
    if (x->IsColorRed() && sibling->IsColorBlack()) {
      x->SetColorBlack();
      sibling->SetColorRed();
      return nullptr;
    } else if (sibling->IsColorBlack()) {
      sibling->SetColorRed();
      if (x->HasParent()) {
        *was_right = x->IsRightChild();
      }
      x = x->parent;
      return x;
    }
    // TODO what to do?
    return nullptr;
  }

  void DeleteFixup(RBTreeNodeT* x, bool was_right_child_removed) {
    bool was_right_child = was_right_child_removed;
    while (x != nullptr) {
      if (!was_right_child) {
        x = LeftChildDeleteFixup(x, &was_right_child);
      } else {
        x = RightChildDeleteFixup(x, &was_right_child);
      }
    }
  }

  // Replaces subtree rooted at u with subtree rooted at v.
  // v can be nullptr.
  void Transplant(RBTreeNodeT* u, RBTreeNodeT* v) {
    if (!u->HasParent()) {
      root_ = v;
    } else if (u->IsLeftChild()) {
      u->parent->left_child = v;
    } else {
      u->parent->right_child = v;
    }
    if (v != nullptr) {
      v->parent = u->parent;
    }
  }

  RBTreeNodeT* TreeMinimum(RBTreeNodeT* x) { return trilib::TreeMinimum(x); }

  RBTreeNodeT* TreeMaximum(RBTreeNodeT* x) { return trilib::TreeMaximum(x); }

  RBTreeNodeT* TreeSuccessor(RBTreeNodeT* x) {
    return trilib::TreeSuccessor(x);
  }

  RBTreeNodeT* GrandParent(RBTreeNodeT* node) {
    if ((node != nullptr) && (node->parent != nullptr)) {
      return node->parent->parent;
    } else {
      return nullptr;
    }
  }

  RBTreeNodeT* Uncle(RBTreeNodeT* node) {
    RBTreeNodeT* g = GrandParent(node);
    if (is_null(g)) {
      return nullptr;                          // No grandparent means no uncle
    } else if (node->parent->IsLeftChild()) {  // node->properties & kLeftChild
      return g->right_child;
    } else {
      return g->left_child;
    }
  }

  void LeftRotate(RBTreeNodeT* x) {
    RBTreeNodeT* y = x->right_child;
    x->right_child = y->left_child;
    if (y->HasLeftChild()) {
      y->left_child->parent = x;
    }
    y->parent = x->parent;
    if (!x->HasParent()) {
      root_ = y;
    } else if (x->IsLeftChild()) {
      x->parent->left_child = y;
    } else {
      x->parent->right_child = y;
    }
    y->left_child = x;
    x->parent = y;
  }

  void RightRotate(RBTreeNodeT* x) {
    RBTreeNodeT* y = x->left_child;
    x->left_child = y->right_child;  // 1
    if (y->HasRightChild()) {
      y->right_child->parent = x;  // 2
    }
    y->parent = x->parent;  // 3
    if (!x->HasParent()) {  // 4
      root_ = y;
    } else if (x->IsRightChild()) {
      x->parent->right_child = y;
    } else {
      x->parent->left_child = y;
    }
    y->right_child = x;  // 5
    x->parent = y;       // 6
  }

  RBTreeNodeT* BinarySearchInsert(ValueT value) {
    if (is_null(root_)) {
      root_ = new RBTreeNodeT(value);
      return root_;
    }
    RBTreeNodeT* ptr = root_;
    while (true) {
      // DCHECK(ptr != nullptr);
      if (value_cmp_(value, ptr->value_)) {
        if (ptr->HasLeftChild()) {
          ptr = ptr->left_child;
        } else {
          ptr->left_child = new RBTreeNodeT(value);
          ptr->left_child->parent = ptr;
          return ptr->left_child;
        }
      } else {
        if (ptr->HasRightChild()) {
          ptr = ptr->right_child;
        } else {
          ptr->right_child = new RBTreeNodeT(value);
          ptr->right_child->parent = ptr;
          return ptr->right_child;
        }
      }
    }
    return nullptr;
  }

  RBTreeNodeT* root_;
  const CompT value_cmp_;
};

}  // trilib

#endif  // RBTREE_H_

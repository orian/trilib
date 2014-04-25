namespace trilib {

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

namespace {

// helper methods
template <typename ValueT>
const ValueT& GetValue(const RBTreeNode<ValueT>* const ptr) {
  return ptr->value;
}

template <typename ValueT>
const ValueT& GetValue(const RBTreeNode<ValueT>& ref) {
  return ref.value;
}

template <typename ValueT, typename CompT>
bool RunCmp(const RBTreeNode<ValueT>* const a,
            const RBTreeNode<ValueT>* const b, const CompT& comp) {
  return comp(a->value_, b->value_);
}

// template<typename NodeT, typename CompT>
// bool RunCmp(const NodeT a, const NodeT b) {
//   static CompT comp;
//   return comp(GetValue(a), GetValue(b));
// }

// template<typename NodeT, typename CompT>
// bool RunCmp(const NodeT a, const NodeT b, const CompT& comp) {
//   return comp(GetValue(a), GetValue(b));
// }

// template<typename NodeT, typename CompT>
// bool RunCmp(const NodeT a, const NodeT b, const CompT& comp) {
//   return comp(GetValue(a), GetValue(b));
// }

}  // namespace

template <typename ValueT, typename CompT>
bool CheckIsBinarySearchTree(const RBTreeNode<ValueT>* const root) {
  return (!root->HasLeftChild() ||
          (RunCmp<ValueT, CompT>(root->left_child, root, CompT()) &&
           CheckIsBinarySearchTree<ValueT, CompT>(
               root->left_child))) &&  // left child
         (!root->HasRightChild() ||
          (RunCmp(root, root->right_child, CompT()) &&
           CheckIsBinarySearchTree<ValueT, CompT>(root->right_child)));
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

template <typename ValueT, typename CompT>
class RBTree {
 public:
  using RBTreeNodeT = RBTreeNode<ValueT>;

  RBTree() : root_(nullptr), value_cmp_() {}

  ~RBTree() {}

  // STL like iterators
  class iterator {
   public:
    iterator() : node_(nullptr) {}
    iterator(const iterator& iter) : node_(iter.node_) {}
    iterator(RBTreeNodeT* node) : node_(node) {}
    // prefix
    iterator& operator++() {
      node_ = trilib::TreeSuccessor(node_);
      return *this;
    }
    // postfix
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

  bool IsBinarySearchTree() const {
    return CheckIsBinarySearchTree<ValueT, CompT>(root_);
  }

 private:
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
    if (g == nullptr) {
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
    x->left_child = y->right_child;
    if (y->HasRightChild()) {
      y->right_child->parent = x;
    }
    y->parent = x->parent;
    if (!x->HasParent()) {
      root_ = y;
    } else if (x->IsRightChild()) {
      x->parent->right_child = y;
    } else {
      x->parent->left_child = y;
    }
    y->right_child = x;
    x->parent = y;
  }

  RBTreeNodeT* BinarySearchInsert(ValueT value) {
    if (root_ == nullptr) {
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
          // ptr->left_child->SetIsLeftChild(true);
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

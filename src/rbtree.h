namespace trilib {

template <typename ValueT>
class RBTreeNode {
 public:
  RBTreeNode()
      : properties(0),
        parent(nullptr),
        left_child(nullptr),
        right_child(nullptr) {}

  RBTreeNode(ValueT value) : value_(value), properties(0),
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

template <typename ValueT, typename CompT>
class RBTree {
 public:
  using RBTreeNodeT = RBTreeNode<ValueT>;

  RBTree() : root_(nullptr), value_cmp_() {}

  ~RBTree() {

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
      RBTreeNodeT* grandparent = GrandParent(node);
      if (uncle != nullptr && uncle->IsColorRed()) {
        node->parent->SetColorBlack();
        uncle->SetColorBlack();
        grandparent->SetColorRed();
        // insert_case1(g);
        // return;
        node = grandparent;
        continue;
      } else {  // insert_case4
        if ((node->IsRightChild()) && node->parent->IsLeftChild()) {
          RotateLeft(node);
          node = node->left_child;
        } else if (node->IsLeftChild() && node->parent->IsRightChild()) {
          RotateRight(node);
          node = node->right_child;
        }
        // insert_case5
        grandparent = GrandParent(node);
        node->parent->SetColorBlack();
        grandparent->SetColorRed();
        if (node->IsLeftChild()) {
          RotateRight(node->parent);
        } else {
          RotateLeft(node->parent);
        }
        return;
      }
    }
  }

  bool IsBinarySearchTree() const {
    return CheckIsBinarySearchTree<ValueT, CompT>(root_);
  }

 private:
  void RotateLeft(RBTreeNodeT* node) {
    RBTreeNodeT* grandparent = GrandParent(node);
    RBTreeNodeT* saved_p = node->parent;
    RBTreeNodeT* saved_left_n = node->left_child;
    if (grandparent != nullptr) {
      if (saved_p->IsLeftChild()) {
        grandparent->left_child = node;
      } else {
        grandparent->right_child = node;
      }
    }
    node->left_child = saved_p;
    saved_p->right_child = saved_left_n;

    node->parent = grandparent;
    saved_p->parent = node;
    if (saved_left_n != nullptr) {
      saved_left_n->parent = saved_p;
    }
  }

  void RotateRight(RBTreeNodeT* node) {
    RBTreeNodeT* grandparent = GrandParent(node);
    RBTreeNodeT* saved_p = node->parent;
    RBTreeNodeT* saved_right_n = node->right_child;
    if (grandparent != nullptr) {
      if (saved_p->IsLeftChild()) {
        grandparent->left_child = node;
      } else {
        grandparent->right_child = node;
      }
    }
    node->right_child = saved_p;
    saved_p->left_child = saved_right_n;

    node->parent = grandparent;
    saved_p->parent = node;
    if (saved_right_n != nullptr) {
      saved_right_n->parent = saved_p;
    }
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

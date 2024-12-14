#include <iostream>
#include <random>

long long even_end, even_start, odd_end, odd_start;

struct DecNode {
  long long priority;
  long long val, size;
  long long sum;
  DecNode* left_tree;
  DecNode* rigth_tree;
  DecNode(long long val)
      : priority(rand()), val(val), size(1), sum(val), left_tree(nullptr), rigth_tree(nullptr) {
  }
};

long long getSize(DecNode* node) {
  return node ? node->size : 0;
}

long long getsum(DecNode* node) {
  return node ? node->sum : 0;
}

void update(DecNode* node) {
  if (node) {
    node->size = getSize(node->left_tree) + 1 + getSize(node->rigth_tree);
    node->sum = getsum(node->left_tree) + getsum(node->rigth_tree) + node->val;
  }
}

void decSplit(DecNode* root, long long key, DecNode*& left, DecNode*& right, long long add = 0) {
  if (!root) {
    left = right = nullptr;
    return;
  }
  long long curr_key = add + getSize(root->left_tree);
  if (key <= curr_key) {
    decSplit(root->left_tree, key, left, root->left_tree, add);
    right = root;
  } else {
    decSplit(root->rigth_tree, key, root->rigth_tree, right, curr_key + 1);
    left = root;
  }
  update(root);
}

DecNode* decMerge(DecNode* left, DecNode* right) {
  if (!left || !right)
    return left ? left : right;
  if (left->priority > right->priority) {
    left->rigth_tree = decMerge(left->rigth_tree, right);
    update(left);
    return left;
  } else {
    right->left_tree = decMerge(left, right->left_tree);
    update(right);
    return right;
  }
}

void pushBack(DecNode*& root, long long value) {
  DecNode* node = new DecNode(value);
  root = decMerge(root, node);
}

void replaceOp(DecNode*& even_root, DecNode*& odd_root, long long lft_bound, long long rght_bound) {
  long long len = rght_bound - lft_bound + 1;
  if (len % 2 != 0)
    return;

  long long even_start = (lft_bound + 1) / 2 - 1;
  long long even_end = rght_bound / 2 - 1;
  long long even_count = even_end - even_start + 1;

  long long odd_start = lft_bound / 2;
  long long odd_end = (rght_bound + 1) / 2 - 1;
  long long odd_count = odd_end - odd_start + 1;

  DecNode *even_left, *even_mid, *even_right;
  DecNode *odd_left, *odd_mid, *odd_right;

  decSplit(even_root, even_start, even_left, even_mid);
  decSplit(even_mid, even_count, even_mid, even_right);

  decSplit(odd_root, odd_start, odd_left, odd_mid);
  decSplit(odd_mid, odd_count, odd_mid, odd_right);

  even_root = decMerge(decMerge(even_left, odd_mid), even_right);
  odd_root = decMerge(decMerge(odd_left, even_mid), odd_right);
}

void sumCalculateOp(
    DecNode*& even_root,
    DecNode*& odd_root,
    long long left_bound,
    long long right_bound,
    long long& res
) {
  res = 0;

  long long even_start = (left_bound + 1) / 2 - 1;
  long long even_end = right_bound / 2 - 1;
  long long even_count = even_end - even_start + 1;

  long long odd_start = left_bound / 2;
  long long odd_end = (right_bound + 1) / 2 - 1;
  long long odd_count = odd_end - odd_start + 1;

  DecNode *even_left, *even_mid, *even_right;
  DecNode *odd_left, *odd_mid, *odd_right;

  if (even_count > 0) {
    decSplit(even_root, even_start, even_left, even_mid);
    decSplit(even_mid, even_count, even_mid, even_right);

    res += getsum(even_mid);

    even_root = decMerge(decMerge(even_left, even_mid), even_right);
  }

  if (odd_count > 0) {
    decSplit(odd_root, odd_start, odd_left, odd_mid);
    decSplit(odd_mid, odd_count, odd_mid, odd_right);

    res += getsum(odd_mid);

    odd_root = decMerge(decMerge(odd_left, odd_mid), odd_right);
  }
}

void clear(DecNode* node) {
  if (node == nullptr)
    return;
  clear(node->left_tree);
  clear(node->rigth_tree);
  delete node;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long test_number = 1;
  long long n_val = 0;
  long long r_val = 0;

  while (std::cin >> n_val >> r_val, n_val != 0 || r_val != 0) {
    DecNode* even_root = nullptr;
    DecNode* odd_root = nullptr;

    for (long long i = 1; i <= n_val; i++) {
      long long val = 0;
      std::cin >> val;
      if (i % 2 == 0) {
        pushBack(even_root, val);
      } else {
        pushBack(odd_root, val);
      }
    }

    std::cout << "Suite " << test_number++ << ":\n";

    for (long long i = 0; i < r_val; i++) {
      long long type = 0;
      long long fst_val = 0;
      long long scnd_val = 0;
      std::cin >> type >> fst_val >> scnd_val;

      if (type == 1) {
        long long len = scnd_val - fst_val + 1;
        if (len % 2 == 0) {
          replaceOp(even_root, odd_root, fst_val, scnd_val);
        }
      } else if (type == 2) {
        long long result = 0;
        sumCalculateOp(even_root, odd_root, fst_val, scnd_val, result);
        std::cout << result << "\n";
      }
    }

    std::cout << "\n";

    clear(even_root);
    clear(odd_root);
  }

  return 0;
}

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>

const float MValue = 2.0;

struct TreeNode {
  int priority{}, size = 1;
  int64_t sum = 0;
  int data{};
  TreeNode* left_tree = nullptr;
  TreeNode* rigth_tree = nullptr;
};

int UpdateNodeSize(TreeNode* node) {
  int res = 1;
  if (node->left_tree != nullptr) {
    res += node->left_tree->size;
  }
  if (node->rigth_tree != nullptr) {
    res += node->rigth_tree->size;
  }
  return res;
}

int64_t UpdateNodeSum(TreeNode* node) {
  int64_t sum = node->data;
  if (node->left_tree != nullptr) {
    sum += node->left_tree->sum;
  }
  if (node->rigth_tree != nullptr) {
    sum += node->rigth_tree->sum;
  }
  return sum;
}

int CalculIndex(TreeNode* node, int value = 0) {
  return node->left_tree != nullptr ? (node->left_tree->size + value + 1) : (1 + value);
}

std::pair<TreeNode*, TreeNode*> SplitTree(TreeNode* node, int key) {
  if (node == nullptr) {
    return {NULL, NULL};
  }
  int index = CalculIndex(node);
  if (index <= key) {
    std::pair<TreeNode*, TreeNode*> tmp = SplitTree(node->rigth_tree, key - index);
    node->rigth_tree = tmp.first;
    node->size = UpdateNodeSize(node);
    node->sum = UpdateNodeSum(node);
    return {node, tmp.second};
  }
  std::pair<TreeNode*, TreeNode*> tmp = SplitTree(node->left_tree, key);
  node->left_tree = tmp.second;
  node->size = UpdateNodeSize(node);
  node->sum = UpdateNodeSum(node);
  return {tmp.first, node};
}

TreeNode* MergeTree(TreeNode* left, TreeNode* right) {
  if (left == nullptr) {
    return right;
  }
  if (right == nullptr) {
    return left;
  }
  if (left->priority > right->priority) {
    left->rigth_tree = MergeTree(left->rigth_tree, right);
    left->size = UpdateNodeSize(left);
    left->sum = UpdateNodeSum(left);
    return left;
  }
  right->left_tree = MergeTree(left, right->left_tree);
  right->size = UpdateNodeSize(right);
  right->sum = UpdateNodeSum(right);
  return right;
}
TreeNode* InsertNewNode(TreeNode* root, int data) {
  TreeNode* new_node = new TreeNode();
  new_node->priority = rand();
  new_node->data = data;
  new_node->sum = data;
  new_node->left_tree = nullptr;
  new_node->rigth_tree = nullptr;
  return MergeTree(root, new_node);
}

TreeNode* FindeNode(TreeNode* root, int key, int value = 0) {
  if (root == nullptr) {
    return nullptr;
  }
  TreeNode* res = nullptr;
  int index = CalculIndex(root, value);
  if (index == key) {
    return root;
  }
  if (index < key) {
    res = FindeNode(root->rigth_tree, key, index);
  } else {
    res = FindeNode(root->left_tree, key, value);
  }
  return res;
}

std::pair<TreeNode*, TreeNode*> ReversNodes(
    TreeNode* nod_node, TreeNode* odd_node, int start, int end
) {
  int nod_en = 0;
  int nod_st = 0;
  int odd_en = 0;
  int odd_st = 0;
  if (end % 2 != 0) {
    nod_st = start / 2;
    odd_st = nod_st + 1;
    nod_en = floor(end / MValue);
    odd_en = ceil(end / MValue);
  } else {
    nod_en = odd_en = end / 2;
    nod_st = odd_st = start == 1 ? 1 : ceil(start / MValue);
  }
  std::pair<TreeNode*, TreeNode*> nod_r = SplitTree(nod_node, nod_en);
  std::pair<TreeNode*, TreeNode*> nod_l = SplitTree(nod_r.first, nod_st - 1);
  std::pair<TreeNode*, TreeNode*> odd_r = SplitTree(odd_node, odd_en);
  std::pair<TreeNode*, TreeNode*> odd_l = SplitTree(odd_r.first, odd_st - 1);
  return {
      MergeTree(nod_l.first, MergeTree(odd_l.second, nod_r.second)),
      MergeTree(odd_l.first, MergeTree(nod_l.second, odd_r.second))
  };
}

std::pair<TreeNode*, TreeNode*> SumCalcul(
    TreeNode* nod_tree, TreeNode* odd_tree, int left, int right, int64_t& res
) {
  int nod_rig = 0;
  int odd_rig = 0;
  int nod_lef = 0;
  int odd_lef = 0;
  if (left == right) {
    TreeNode* node = nullptr;
    if (left % 2 == 0) {
      nod_lef = left == 1 ? 1 : left / 2;
      node = FindeNode(nod_tree, nod_lef);
    } else {
      nod_lef = left == 1 ? 1 : ceil(left / MValue);
      node = FindeNode(odd_tree, nod_lef);
    }
    std::cout << node->data << "\n";
    return {nod_tree, odd_tree};
  }
  if (right % 2 == 0) {
    nod_rig = odd_rig = right / 2;
  } else {
    nod_rig = floor(right / MValue);
    odd_rig = ceil(right / MValue);
  }
  if (left % 2 == 0) {
    nod_lef = left / 2;
    odd_lef = nod_lef + 1;
  } else {
    nod_lef = odd_lef = left == 1 ? 1 : ceil(left / MValue);
  }
  std::pair<TreeNode*, TreeNode*> nod_r = SplitTree(nod_tree, nod_rig);
  std::pair<TreeNode*, TreeNode*> nod_l = SplitTree(nod_r.first, nod_lef - 1);
  std::pair<TreeNode*, TreeNode*> odd_r = SplitTree(odd_tree, odd_rig);
  std::pair<TreeNode*, TreeNode*> odd_l = SplitTree(odd_r.first, odd_lef - 1);
  res = nod_l.second->sum + odd_l.second->sum;
  return {
      MergeTree(nod_l.first, MergeTree(nod_l.second, nod_r.second)),
      MergeTree(odd_l.first, MergeTree(odd_l.second, odd_r.second))
  };
}

void DelTree(TreeNode* node) {
  if (node == nullptr) {
    return;
  }
  DelTree(node->left_tree);
  DelTree(node->rigth_tree);
  delete node;
}

int main() {
  srand(time(0));

  int test_number = 1;
  int array_leng = 0;
  int req_count = 0;

  std::cin >> array_leng >> req_count;
  while ((array_leng != 0) && (req_count != 0)) {
    TreeNode* nood_tree = nullptr;
    TreeNode* odd_tree = nullptr;

    for (int i = 1; i <= array_leng; i++) {
      int data = 0;
      std::cin >> data;
      if (i % 2 == 0) {
        nood_tree = InsertNewNode(nood_tree, data);
      } else {
        odd_tree = InsertNewNode(odd_tree, data);
      }
    }
    std::cout << "Suite " << test_number << ":\n";
    for (int i = 0; i < req_count; i++) {
      int type = 0;
      int start_index = 0;
      int end_index = 0;
      std::cin >> type >> start_index >> end_index;
      if ((start_index <= 0) || (end_index > array_leng)) {
        continue;
      }
      if (type == 1) {
        if (start_index == end_index) {
          continue;
        }
        if ((start_index + end_index) % 2 == 0) {
          continue;
        }
        std::pair<TreeNode*, TreeNode*> rev_tree =
            ReversNodes(nood_tree, odd_tree, start_index, end_index);
        nood_tree = rev_tree.first;
        odd_tree = rev_tree.second;
      } else if (type == 2) {
        int64_t result = 0;
        std::pair<TreeNode*, TreeNode*> trees =
            SumCalcul(nood_tree, odd_tree, start_index, end_index, result);
        nood_tree = trees.first;
        odd_tree = trees.second;
        std::cout << result << "\n";
      }
    }
    std::cin >> array_leng >> req_count;
    test_number++;
    DelTree(nood_tree);
    DelTree(odd_tree);
  }
}

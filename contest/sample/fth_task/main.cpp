#include <iostream>
#include <random>

struct TreapNode {
  int value;
  int size;
  long long sum;
  int priority;
  TreapNode* left;
  TreapNode* right;

  TreapNode(int val) : value(val), size(1), sum(val), left(nullptr), right(nullptr) {
    static std::mt19937 rng(std::random_device{}());
    priority = rng();
  }
};

void update(TreapNode* node) {
  if (node) {
    node->size = 1;
    node->sum = node->value;
    if (node->left) {
      node->size += node->left->size;
      node->sum += node->left->sum;
    }
    if (node->right) {
      node->size += node->right->size;
      node->sum += node->right->sum;
    }
  }
}

void split(TreapNode* node, int key, TreapNode*& left, TreapNode*& right, int add = 0) {
  if (!node) {
    left = right = nullptr;
    return;
  }
  int curr_key = add + (node->left ? node->left->size : 0);
  if (key <= curr_key) {
    split(node->left, key, left, node->left, add);
    right = node;
  } else {
    split(node->right, key, node->right, right, curr_key + 1);
    left = node;
  }
  update(node);
}

TreapNode* merge(TreapNode* left, TreapNode* right) {
  if (!left || !right)
    return left ? left : right;
  if (left->priority > right->priority) {
    left->right = merge(left->right, right);
    update(left);
    return left;
  } else {
    right->left = merge(left, right->left);
    update(right);
    return right;
  }
}

void insertAtEnd(TreapNode*& root, int value) {
  TreapNode* node = new TreapNode(value);
  root = merge(root, node);
}

void swapPairs(TreapNode*& evenRoot, TreapNode*& oddRoot, int x, int y) {
  int len = y - x + 1;
  if (len % 2 != 0)
    return;

  int evenStart = x / 2;
  int evenCount = len / 2;

  int oddStart = (x - 1) / 2;
  int oddCount = len / 2;

  TreapNode *evenLeft, *evenMid, *evenRight;
  TreapNode *oddLeft, *oddMid, *oddRight;

  split(evenRoot, evenStart, evenLeft, evenMid);
  split(evenMid, evenCount, evenMid, evenRight);

  split(oddRoot, oddStart, oddLeft, oddMid);
  split(oddMid, oddCount, oddMid, oddRight);

  evenRoot = merge(merge(evenLeft, oddMid), evenRight);
  oddRoot = merge(merge(oddLeft, evenMid), oddRight);
}

long long rangeSum(TreapNode*& evenRoot, TreapNode*& oddRoot, int a, int b) {
  int evenStart = (a + 1) / 2 - 1;
  int evenEnd = (b + 1) / 2 - 1;
  int evenCount = evenEnd - evenStart + 1;

  int oddStart = a / 2;
  int oddEnd = b / 2;
  int oddCount = oddEnd - oddStart + 1;

  TreapNode *evenLeft, *evenMid, *evenRight;
  TreapNode *oddLeft, *oddMid, *oddRight;

  split(evenRoot, evenStart, evenLeft, evenMid);
  split(evenMid, evenCount, evenMid, evenRight);

  split(oddRoot, oddStart, oddLeft, oddMid);
  split(oddMid, oddCount, oddMid, oddRight);

  long long totalSum = 0;
  if (evenMid)
    totalSum += evenMid->sum;
  if (oddMid)
    totalSum += oddMid->sum;

  evenRoot = merge(merge(evenLeft, evenMid), evenRight);
  oddRoot = merge(merge(oddLeft, oddMid), oddRight);

  return totalSum;
}

void destroyTreap(TreapNode* node) {
  if (node) {
    destroyTreap(node->left);
    destroyTreap(node->right);
    delete node;
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, R;
  int testCase = 1;

  while (std::cin >> N >> R, N || R) {
    std::cout << "Suite " << testCase++ << ":\n";

    TreapNode* evenRoot = nullptr;
    TreapNode* oddRoot = nullptr;

    for (int i = 1; i <= N; ++i) {
      int value;
      std::cin >> value;
      if (i % 2 == 0) {
        insertAtEnd(evenRoot, value);
      } else {
        insertAtEnd(oddRoot, value);
      }
    }

    for (int i = 0; i < R; ++i) {
      int type, x, y;
      std::cin >> type >> x >> y;

      if (type == 1) {
        swapPairs(evenRoot, oddRoot, x, y);
      } else if (type == 2) {
        long long sum = rangeSum(evenRoot, oddRoot, x, y);
        std::cout << sum << '\n';
      }
    }

    std::cout << '\n';

    destroyTreap(evenRoot);
    destroyTreap(oddRoot);
  }

  return 0;
}
#include <array>
#include <iostream>
#include <memory>
#include <vector>

class PathFinder {
private:
  struct BinaryNode {
    std::array<std::unique_ptr<BinaryNode>, 2> branches;
    BinaryNode() : branches{nullptr, nullptr} {
    }
  };

  class BinaryTree {
  private:
    std::unique_ptr<BinaryNode> root;

  public:
    BinaryTree() : root(std::make_unique<BinaryNode>()) {
    }

    void addPath(int value) {
      BinaryNode* current = root.get();
      for (int pos = 31; pos >= 0; --pos) {
        int bit = (value >> pos) & 1;
        if (!current->branches[bit]) {
          current->branches[bit] = std::make_unique<BinaryNode>();
        }
        current = current->branches[bit].get();
      }
    }

    int findMaxXorPair(int value) {
      BinaryNode* current = root.get();
      int result = 0;

      for (int pos = 31; pos >= 0; --pos) {
        int bit = (value >> pos) & 1;
        int opposite = 1 - bit;

        if (current->branches[opposite]) {
          result |= (1 << pos);
          current = current->branches[opposite].get();
        } else if (current->branches[bit]) {
          current = current->branches[bit].get();
        } else {
          break;
        }
      }
      return result;
    }
  };

  std::vector<std::vector<std::pair<int, int>>> graph;
  std::vector<int> pathXors;
  std::vector<bool> visited;
  int vertexCount;
  int shopLocation;

public:
  PathFinder(int n, int shop)
      : graph(n + 1), pathXors(n + 1), visited(n + 1, false), vertexCount(n), shopLocation(shop) {
  }

  void addRoad(int from, int to, int weight) {
    graph[from].emplace_back(to, weight);
    graph[to].emplace_back(from, weight);
  }

  void calculatePaths(int vertex, int currentXor) {
    visited[vertex] = true;
    pathXors[vertex] = currentXor;

    for (const auto& [nextVertex, weight] : graph[vertex]) {
      if (!visited[nextVertex]) {
        calculatePaths(nextVertex, currentXor ^ weight);
      }
    }
  }

  int findMaxDiscount() {
    calculatePaths(shopLocation, 0);

    std::vector<int> possiblePaths;
    for (int i = 1; i <= vertexCount; ++i) {
      if (i != shopLocation) {
        possiblePaths.push_back(pathXors[i]);
      }
    }

    BinaryTree xorTree;
    int maxDiscount = 0;

    for (int value : possiblePaths) {
      xorTree.addPath(value);
    }

    for (int value : possiblePaths) {
      maxDiscount = std::max(maxDiscount, xorTree.findMaxXorPair(value));
    }

    return maxDiscount;
  }
};

int main() {
  int n, f;
  std::cin >> n >> f;

  PathFinder solver(n, f);

  for (int i = 0; i < n - 1; ++i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    solver.addRoad(u, v, w);
  }

  std::cout << solver.findMaxDiscount() << std::endl;
  return 0;
}
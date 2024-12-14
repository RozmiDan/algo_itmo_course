#include <cmath>
#include <iostream>
#include <vector>

const int MAX_BINARY_LIFT = 17;

class TimelineManager {
private:
  std::vector<std::vector<int>> timeConnections;
  std::vector<std::vector<int>> binaryLift;
  std::vector<int> timelineDepth;
  std::vector<int> entryTime;
  std::vector<int> exitTime;
  int timeCounter;

public:
  TimelineManager(int size) : timeCounter(0) {
    timeConnections.resize(size);
    binaryLift.resize(size, std::vector<int>(MAX_BINARY_LIFT, -1));
    timelineDepth.resize(size, 0);
    entryTime.resize(size, 0);
    exitTime.resize(size, 0);
  }

  void addConnection(int from, int to) {
    timeConnections[from].push_back(to);
    timeConnections[to].push_back(from);
  }

  void buildTimelineTree(int currentNode, int parentNode) {
    entryTime[currentNode] = ++timeCounter;
    binaryLift[currentNode][0] = parentNode;

    for (int level = 1; level < MAX_BINARY_LIFT; ++level) {
      if (binaryLift[currentNode][level - 1] != -1)
        binaryLift[currentNode][level] = binaryLift[binaryLift[currentNode][level - 1]][level - 1];
      else
        binaryLift[currentNode][level] = -1;
    }

    for (int nextNode : timeConnections[currentNode]) {
      if (nextNode != parentNode) {
        timelineDepth[nextNode] = timelineDepth[currentNode] + 1;
        buildTimelineTree(nextNode, currentNode);
      }
    }
    exitTime[currentNode] = ++timeCounter;
  }

  bool isTimelineAncestor(int ancestor, int descendant) {
    return entryTime[ancestor] <= entryTime[descendant] &&
           exitTime[ancestor] >= exitTime[descendant];
  }

  int findConvergencePoint(int timeline1, int timeline2) {
    if (isTimelineAncestor(timeline1, timeline2))
      return timeline1;
    if (isTimelineAncestor(timeline2, timeline1))
      return timeline2;

    for (int i = MAX_BINARY_LIFT - 1; i >= 0; --i) {
      if (binaryLift[timeline1][i] != -1 &&
          !isTimelineAncestor(binaryLift[timeline1][i], timeline2))
        timeline1 = binaryLift[timeline1][i];
    }
    return binaryLift[timeline1][0];
  }

  int calculateTimeDistance(int point1, int point2) {
    int convergence = findConvergencePoint(point1, point2);
    return timelineDepth[point1] + timelineDepth[point2] - 2 * timelineDepth[convergence];
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int vertexCount;
  std::cin >> vertexCount;

  TimelineManager timeline(vertexCount);

  for (int i = 0; i < vertexCount - 1; ++i) {
    int from, to;
    std::cin >> from >> to;
    timeline.addConnection(from, to);
  }

  timeline.buildTimelineTree(0, -1);

  int queryCount;
  std::cin >> queryCount;
  while (queryCount--) {
    int start, end, energy;
    std::cin >> start >> end >> energy;
    int requiredEnergy = timeline.calculateTimeDistance(start, end);
    std::cout << (requiredEnergy <= energy ? "Yes\n" : "No\n");
  }

  return 0;
}
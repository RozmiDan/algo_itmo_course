#include <algorithm>
#include <cstddef>
#include <iostream>
#include <queue>
#include <vector>

struct TeamState {
  double cohesion;
  int connections;
  int size;
};

TeamState calculateTeamState(
    const std::vector<bool>& team, const std::vector<std::vector<bool>>& adjacencyMatrix
) {
  int teamSize = 0;
  int connections = 0;

  for (size_t i = 1; i < team.size(); i++) {
    if (team[i]) {
      teamSize++;
      for (size_t j = i + 1; j < team.size(); j++) {
        if (team[j] && adjacencyMatrix[i][j]) {
          connections++;
        }
      }
    }
  }

  double cohesion = teamSize == 0 ? 0 : static_cast<double>(connections) / teamSize;
  return {cohesion, connections, teamSize};
}

bool isTeamBetter(const TeamState& newState, const TeamState& bestState) {
  if (newState.size == 0)
    return false;
  if (bestState.size == 0)
    return true;

  // Сравниваем сплоченность с учетом размера команды
  if (std::abs(newState.cohesion - bestState.cohesion) < 1e-9) {
    return newState.size > bestState.size;
  }
  return newState.cohesion > bestState.cohesion;
}

int main() {
  int n, m;
  std::cin >> n >> m;

  std::vector<std::vector<bool>> adjacencyMatrix(n + 1, std::vector<bool>(n + 1, false));

  for (int i = 0; i < m; ++i) {
    int ai, bi;
    std::cin >> ai >> bi;
    adjacencyMatrix[ai][bi] = true;
    adjacencyMatrix[bi][ai] = true;
  }

  // Начинаем с полной команды
  std::vector<bool> bestTeam(n + 1, true);
  TeamState bestState = calculateTeamState(bestTeam, adjacencyMatrix);

  for (int mask = 0; mask < (1 << n); ++mask) {
    std::vector<bool> currentTeam(n + 1, false);
    for (int i = 0; i < n; ++i) {
      if (mask & (1 << i)) {
        currentTeam[i + 1] = true;
      }
    }

    TeamState currentState = calculateTeamState(currentTeam, adjacencyMatrix);
    if (isTeamBetter(currentState, bestState)) {
      bestState = currentState;
      bestTeam = currentTeam;
    }
  }

  std::vector<int> result;
  for (int i = 1; i <= n; i++) {
    if (bestTeam[i]) {
      result.push_back(i);
    }
  }

  std::cout << result.size() << std::endl;
  for (int id : result) {
    std::cout << id << std::endl;
  }

  return 0;
}

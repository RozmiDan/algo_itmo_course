#include <bitset>
#include <iostream>
#include <vector>

const int MAX_N = 20;  // Максимальное число сотрудников для полного перебора

int main() {
  int n, m;
  std::cin >> n >> m;

  if (n > MAX_N) {
    std::cerr << "n слишком велико для полного перебора (n <= " << MAX_N << ")." << std::endl;
    return 1;
  }

  std::vector<std::vector<bool>> adjacencyMatrix(n, std::vector<bool>(n, false));

  for (int i = 0; i < m; ++i) {
    int ai, bi;
    std::cin >> ai >> bi;
    ai--;  // Приводим индексы к 0-based
    bi--;
    adjacencyMatrix[ai][bi] = true;
    adjacencyMatrix[bi][ai] = true;
  }

  double maxCohesion = -1.0;
  int maxTeamSize = 0;
  int maxEdges = 0;
  std::vector<int> bestTeam;

  int totalSubsets = 1 << n;  // Общее число подмножеств

  for (int mask = 1; mask < totalSubsets; ++mask) {
    std::vector<int> teamMembers;
    for (int i = 0; i < n; ++i) {
      if (mask & (1 << i)) {
        teamMembers.push_back(i);
      }
    }

    int teamSize = teamMembers.size();

    // Подсчитываем количество пар ладящих сотрудников внутри команды
    int edges = 0;
    for (size_t i = 0; i < teamMembers.size(); ++i) {
      for (size_t j = i + 1; j < teamMembers.size(); ++j) {
        int u = teamMembers[i];
        int v = teamMembers[j];
        if (adjacencyMatrix[u][v]) {
          edges++;
        }
      }
    }

    // Вычисляем сплоченность команды
    double cohesion = (double)edges / teamSize;

    // Обновляем лучшую команду
    if (cohesion > maxCohesion || (cohesion == maxCohesion && teamSize > maxTeamSize)) {
      maxCohesion = cohesion;
      maxTeamSize = teamSize;
      maxEdges = edges;
      bestTeam = teamMembers;
    }
  }

  // Выводим результат
  std::cout << bestTeam.size() << std::endl;
  for (int id : bestTeam) {
    std::cout << id + 1 << std::endl;  // Приводим индексы обратно к 1-based
  }

  return 0;
}
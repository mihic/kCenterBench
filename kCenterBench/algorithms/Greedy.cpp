#include "../stdafx.h"

void Graph::greedyAddCenter(vector<int>& to, unordered_set<int>& from) {
  if (from.empty()) {
    cerr << "greedy adding from empty" << endl;
    return;
  }
  int bestScore = MAX_INT;
  int bestCenter;
  for (int i : from) {
    to.push_back(i);
    int score = evalKCenter(to);
    if (score < bestScore) {
      bestScore = score;
      bestCenter = i;
    }
    //TODO hevristika
    else if (score == bestScore) {
      if (i < bestCenter) {
        bestCenter = i;
      }
    }
    to.pop_back();
  }
  to.push_back(bestCenter);
  from.erase(bestCenter);

}

int Graph::centersGreedy(int k) {
  calcShortestPath();
  unordered_set<int> from;
  vector<int> to;
  for (int i = 0; i < n; ++i) {
    from.emplace(i);
  }
  for (int i = 0; i < k; ++i) {
    greedyAddCenter(to, from);
  }
  int score = evalKCenter(to);
  if (debug) {
    cout << "Score = " << score << " with centers:" << endl;
    printVec(to);
  }
  return score;
}

int Graph::centersGreedyPlus(int k) {
  calcShortestPath();
  vector<int> from;
  k--;
  int bestScore = MAX_INT;
  vector<int> bestCenters;

  for (int i = 0; i < n; ++i) {
    vector<int> to = { i };
    unordered_set<int> from;
    for (int j = 0; j < n; ++j) {
      if (j != i)
        from.emplace(j);
    }
    for (int i = 0; i < k; ++i) {
      greedyAddCenter(to, from);
    }
    int score = evalKCenter(to);
    if (score < bestScore) {
      bestScore = score;
      bestCenters = to;
    }
  }
  if (debug) {
    cout << "Score = " << bestScore << " with centers:" << endl;
    printVec(bestCenters);
  }
  return bestScore;
}

int Graph::centersGreedyRandom(int k) {
  calcShortestPath();
  std::random_device rd;
  std::uniform_int_distribution<int> dist(0, n - 1);
  int r = dist(rd);
  if (debug) {
    cout << "random = " << r << endl;
  }
  vector<int> to = { r };
  unordered_set<int> from;
  for (int j = 0; j < n; ++j) {
    if (j != r)
      from.emplace(j);
  }
  k--;
  for (int i = 0; i < k; ++i) {
    greedyAddCenter(to, from);
  }
  int score = evalKCenter(to);
  if (debug) {
    cout << "Score = " << score << " with centers:" << endl;
    printVec(to);
  }
  return score;
}

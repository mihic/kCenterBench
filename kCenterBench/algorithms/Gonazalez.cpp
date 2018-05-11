#include "../stdafx.h"

void Graph::gonzalezAddCenter(vector<int>& to, vector<int>& from) {
  int maxdist = -1;
  int bestCidx;
  int sizeFrom = from.size();
  int sizeTo = to.size();
  for (int i = 0; i < sizeFrom; ++i) {
    int closestCenteridx = 0;
    int dist = shortestPaths[from[i]][to[0]];
    for (int j = 1; j < sizeTo; ++j) {
      int d = shortestPaths[from[i]][to[j]];
      if (d < dist) {
        dist = d;
        closestCenteridx = j;
      }
    }
    if (maxdist < dist) {
      maxdist = dist;
      bestCidx = i;
    }
  }
  to.emplace_back(from[bestCidx]);
  from.erase(from.begin() + bestCidx);
}

int Graph::centersGonzalezRandom(int k) {
  calcShortestPath();
  std::random_device rd;
  std::uniform_int_distribution<int> dist(0, n - 1);
  int r = dist(rd);
  if (debug) {
    cout << "random = " << r << endl;
  }
  vector<int> to = { r };
  vector<int> from;
  for (int j = 0; j < n; ++j) {
    if (j != r)
      from.emplace_back(j);
  }
  k--;
  for (int i = 0; i < k; ++i) {
    gonzalezAddCenter(to, from);
  }
  int score = evalKCenter(to);
  if (debug) {
    cout << "Score = " << score << " with centers(" << to.size() << "):" << endl;
    printVec(to);
  }
  return score;
}

int Graph::centersGonazalezPlus(int k) {
  calcShortestPath();
  int k1 = k - 1;
  bestScore = MAX_INT;
  for (int r = 0; r < n; ++r) {
    vector<int> to = { r };
    vector<int> from;
    for (int j = 0; j < n; ++j) {
      if (j != r)
        from.emplace_back(j);
    }    
    for (int i = 0; i < k1; ++i) {
      gonzalezAddCenter(to, from);
    }
    int score = evalKCenter(to);
    if (score < bestScore) {
      bestScore = score;
      bestCenters = to;
      if (debug) {
        cout << score << endl;
      }
    }
  }
  if (debug) {
    cout << "Score = " << bestScore << " with centers(" << bestCenters.size() << "):" << endl;
    printVec(bestCenters);
  }
  return bestScore;
}

int Graph::centersGonzalez1C(int k) {
  centersBruteForce(1, false);
  vector<int> from;
  vector<int> to = bestCenters;
  int c1 = bestCenters[0];
  for (int i = 0; i < n; ++i) {
    if (i != c1)
      from.emplace_back(i);
  }
  k--;
  for (int i = 0; i < k; ++i) {
    gonzalezAddCenter(to, from);
  }
  int score = evalKCenter(to);
  if (debug) {
    cout << "Score = " << score << " with centers(" << to.size() << "):" << endl;
    printVec(to);
  }
  return score;
}

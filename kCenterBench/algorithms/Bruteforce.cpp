#include "../stdafx.h"




int Graph::centersBruteForce(int k, bool optimized) {
  if (k >= n) {
    cerr << "k>n" << endl;
    exit(-1);
  }
  vector<int> tmp;
  bestScore = MAX_INT;
  calcShortestPath();
  if (optimized) {
    currentScore = MAX_INT;
    centersBruteForceRecOpt(tmp, 0, k);
  }
  else {
    centersBruteForceRec(tmp, 0, k);
  }
  return bestScore;
}

void Graph::centersBruteForceRec(vector<int>& vec, int i, int d) {
  if (d == 0) {
    int score = evalKCenter(vec);
    if (score < bestScore) {
      bestCenters = vec;
      bestScore = score;
    }
  }
  else {
    for (int j = i; j < n; ++j) {
      vec.push_back(j);
      centersBruteForceRec(vec, j + 1, d - 1);
      vec.pop_back();
    }
  }

}

void Graph::centersBruteForceRecOpt(vector<int>& vec, int i, int d) {
  if (d == 0) {
    return;
  }
  else {
    for (int j = i; j < n; ++j) {
      vec.push_back(j);
      int oldScore = currentScore;
      int score = evalKCenter(vec);
      if (score < currentScore) {
        currentScore = score;
        if (score < bestScore) {
          bestCenters = vec;
          bestScore = score;
        }
        centersBruteForceRecOpt(vec, j + 1, d - 1);
        currentScore = oldScore;
      }
      else {
        cout << "skipping " << j + 1 << " lvl:" << d << endl;
      }

      vec.pop_back();
    }
  }

}
void Graph::BruteForceIndependentSet(vector<int>& vec, set<int>& result, int i, int d) {
  if (d == 0) {
    set<int> selected(vec.begin(), vec.end());
    if (checkCovered(selected)) {
      result = selected;
    }
    return;
  }
  else {
    for (int j = i; j < n; ++j) {
      vec.push_back(j);
      BruteForceIndependentSet(vec, result, j + 1, d - 1);
      if (result.size() > 0) {
        return;
      }
      vec.pop_back();
    }
  }

}

set<int> Graph::BestMaximalIndependentSet(int k) {
  set<int> result;
  for (int i = 0; i <= k; ++i) {
    vector<int> tmp;
    BruteForceIndependentSet(tmp, result, 0, i);
    if (result.size() > 0) {
      break;
    }
  }
  if (result.size() == 0) {
    for (int i = 0; i <= k; ++i) {
      result.emplace(i); //dummy fill for >k results
    }
  }
  return result;
}

int Graph::centersBottleneckBruteForce(int k) {
  calcShortestPath();
  set<int> edgeLengths;
  for (auto i : shortestPaths) {
    for (int j : i) {
      edgeLengths.emplace(j);
    }
  }
  //OPTIMIZE -- only add new edges to Gi
  for (int m : edgeLengths) {
    Graph Gi(n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (shortestPaths[i][j] <= m) {
          Gi.adjMatrix[i][j] = shortestPaths[i][j];
        }
      }
    }
    set<int> centers = Gi.BestMaximalIndependentSet(k);
    if (centers.size() <= k) {
      vector<int> Vcenters(centers.begin(), centers.end());
      int score = evalKCenter(Vcenters);
      if (debug) {
        cout << "Score = " << score << " with centers:" << endl;
        for (int i : centers) {
          cout << i << ' ';
        }
        cout << endl;
      }
      return score;
    }
  }
}
int Graph::centersBottleneckBruteForceBin(int k) {
  calcShortestPath();
  set<int> edgeLengths;
  for (auto i : shortestPaths) {
    for (int j : i) {
      edgeLengths.emplace(j);
    }
  }
  //OPTIMIZE -- only add new edges to Gi
  int a = 0;
  int b = edgeLengths.size();
  int c, s;
  set<int> centers;
  vector<int> VedgeLengths(edgeLengths.begin(), edgeLengths.end());
  while (b - a > 1) {
    int c = (a + b) / 2;
    int m = VedgeLengths[c];
    Graph Gi(n);

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (shortestPaths[i][j] <= m) {
          Gi.adjMatrix[i][j] = shortestPaths[i][j];
        }
      }
    }
    centers = Gi.BestMaximalIndependentSet(k);
    int s = centers.size();
    if (debug) {
      cout << "m=" << m << " csize=" << s << " c=" << c << endl;
    }

    if (centers.size() > k) {
      a = c;
    }
    else {
      b = c;
    }
  }
  int m = VedgeLengths[c];
  Graph Gi(n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (shortestPaths[i][j] <= m) {
        Gi.adjMatrix[i][j] = shortestPaths[i][j];
      }
    }
  }
  centers = Gi.BestMaximalIndependentSet(k);



  vector<int> Vcenters(centers.begin(), centers.end());
  int score = evalKCenter(Vcenters);
  if (debug) {
    cout << "Score = " << score << " with centers:" << endl;
    for (int i : Vcenters) {
      cout << i << ' ';
    }
    cout << endl;
  }
  return score;
}
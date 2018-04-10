#include "../stdafx.h"




int Graph::centersBruteForce(int k, bool optimized) {
  if (k >= n) {
    cerr << "k>n" << endl;
    exit(-1);
  }
  vector<int> tmp;
  bestScore = MAX_INT;
  calcShortestPath();
  //printMatrix(shortestPaths);
  if (optimized) {
    currentScore = MAX_INT;
    centersBruteForceRecOpt(tmp, n, 0, k);
  }
  else {
    centersBruteForceRec(tmp, n, 0, k);
  }
  return bestScore;
}

void Graph::centersBruteForceRec(vector<int>& vec, int n, int i, int d) {
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
      centersBruteForceRec(vec, n, j + 1, d - 1);
      vec.pop_back();
    }
  }

}

void Graph::centersBruteForceRecOpt(vector<int>& vec, int n, int i, int d) {
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
        centersBruteForceRecOpt(vec, n, j + 1, d - 1);
        currentScore = oldScore;
      }
      else {
        cout << "skipping " << j + 1 << " lvl:" << d << endl;
      }

      vec.pop_back();
    }
  }

}


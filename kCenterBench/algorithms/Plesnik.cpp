#include "../stdafx.h"

bool Graph::Range(int k, int B, bool deg) {
  vector<int> V;
  makeVbyDeg();
  V = deg ? VbyDeg : oneToN;
  B = 2 * B;
  while (!V.empty()) {
    int v = V.back();
    V.pop_back();

    rangeCenters.emplace_back(v);
    for (int i = 0; i < n; ++i) {
      int len = shortestPaths[v][i];
      if (len != -1 && len <= B) {
        V.erase(remove(V.begin(), V.end(), i), V.end());
      }
    }
  }
  return rangeCenters.size() <= k;
}

int Graph::centersPlesnik(int k, bool deg) {
  calcShortestPath();
  if (deg) {
    makeVbyDeg();
  }
  else {
    makeOneToN();
  }

  vector<int> edgeLengths;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      int len = shortestPaths[i][j];
      edgeLengths.emplace_back(len);
    }
  }
  sort(edgeLengths.begin(), edgeLengths.end());
  for (int len : edgeLengths) {
    rangeCenters.clear();
    if (Range(k, len, deg)) {
      int score = evalKCenter(rangeCenters);
      if (debug) {
        cout << "Score = " << score << " with centers:" << endl;
        printVec(rangeCenters);
      }
      return score;
    }
  }
  return 0;
}

int Graph::centersPlesnikPlus(int k) {
  int bestScore = MAX_INT;
  vector<int> bestCenters;
  calcShortestPath();
  vector<int> edgeLengths;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      int len = shortestPaths[i][j];
      edgeLengths.emplace_back(len);
    }
  }

  sort(edgeLengths.begin(), edgeLengths.end());
  makeVbyDeg();
  vector<int> vByDeg_orig = VbyDeg;
  for (int i = 0; i < n; ++i) {
    //cout << i << endl;
    //cout << bestScore << endl;
    VbyDeg = vByDeg_orig;
    VbyDeg.erase(remove(VbyDeg.begin(), VbyDeg.end(), i), VbyDeg.end());
    for (int len : edgeLengths) {
      rangeCenters.clear();
      rangeCenters.emplace_back(i);
      for (int j = 0; j < n; ++j) {
        int ln = shortestPaths[i][j];
        if (ln != -1 && ln <= 2 * len) {
          VbyDeg.erase(remove(VbyDeg.begin(), VbyDeg.end(), j), VbyDeg.end());
        }
      }
      if (Range(k, len, true)) {
        int score = evalKCenter(rangeCenters);
        if (score < bestScore) {
          bestScore = score;
          bestCenters = rangeCenters;
        }
        break;
      }
    }
  }
  if (debug) {
    cout << "Score+ = " << bestScore << " with centers:" << endl;
    printVec(bestCenters);
  }
  return bestScore;
}

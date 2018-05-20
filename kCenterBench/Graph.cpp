#include "stdafx.h"

void Graph::makeOneToN() {
  oneToN.clear();
  for (int i = 0; i < n; ++i) {
    oneToN.emplace_back(i);
  }
}

void Graph::makeVbyDeg() {
  vector<pair<int, int>> pairs;
  for (int i = 0; i < n; ++i) {
    int deg = 0;
    for (int j = 0; j < n; ++j) {
      if (adjMatrix[i][j] != -1) deg++;
    }
    pairs.emplace_back(make_pair(i, deg));
  }
  sort(pairs.begin(), pairs.end(), [](auto &left, auto &right) {
    return left.second < right.second;
  });
  VbyDeg.clear();
  for (auto p : pairs) {
    VbyDeg.emplace_back(p.first);
  }
}

Graph Graph::createRandomGraph(int n, double w, int cmin, int cmax) {
  std::random_device r;
  std::default_random_engine e(r());
  std::uniform_int_distribution<int> r_int(cmin, cmax);
  std::uniform_real_distribution<double> r_double(0, 1);
  Graph g(n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (r_double(e) < w) {
        g.adjMatrix[i][j] = r_int(e);
      }
    }
  }
  return g;
}

void Graph::calcShortestPath() {
  shortestPaths = adjMatrix; //copy adj list
  for (int i = 0; i < n; i++) {
    shortestPaths[i][i] = 0; //set path to itself to 0;
  }
  //Floyd–Warshall algorithm
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (shortestPaths[i][k] != -1 && shortestPaths[k][j] != -1) {
          if (shortestPaths[i][j] == -1) {
            shortestPaths[i][j] = shortestPaths[i][k] + shortestPaths[k][j];
          }
          else {
            shortestPaths[i][j] = min(shortestPaths[i][j], shortestPaths[i][k] + shortestPaths[k][j]);
          }
        }
      }
    }
  }
}

void Graph::Sqaure() {
  vector<vector<int>>sqAdjMatrix = adjMatrix;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      for (int k = i; k < j; ++k) {
        if (adjMatrix[i][k] != -1 && adjMatrix[k][j] != -1) {
          if (sqAdjMatrix[i][j] == -1) {
            sqAdjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
          }
          else {
            sqAdjMatrix[i][j] = min(sqAdjMatrix[i][j], adjMatrix[i][k] + adjMatrix[k][j]);
          }
        }
      }
    }
  }
  adjMatrix = sqAdjMatrix;
}

set<int> Graph::RadnomMaximalIndependentSet() {
  set<int> MIS;
  MIS.emplace(0);
  for (int i = 1; i < n; ++i) {
    bool covered = false;
    for (int j = 0; j < n; ++j) {
      if (adjMatrix[i][j] != -1 && MIS.count(j) == 1) {
        covered = true;
        break;
      }
    }
    if (!covered) {
      MIS.emplace(i);
    }
  }
  return MIS;
}

set<int> Graph::GreedyMaximalIndependentSet() {
  vector<int> degs(n);
  vector<bool> covered(n, false);
  set<int> MIS;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (adjMatrix[i][j] != -1) {
        degs[i]++;
      }
    }
  }
  while (true) {
    bool done = true;
    int maxDeg = 0;
    int maxDegIdx = -1;
    for (int i = 0; i < n; ++i) { //find max deg
      if (!covered[i]) {
        done = false;
        if (degs[i] >= maxDeg) {
          maxDeg = degs[i];
          maxDegIdx = i;
        }
      }
    }
    if (done) break;
    //cover max deg
    MIS.emplace(maxDegIdx);
    covered[maxDegIdx] = true;
    for (int i = 0; i < n; ++i) {
      if (adjMatrix[maxDegIdx][i] != -1) {
        covered[i] = true;
        degs[i]--;
      }
    }
  }
  return MIS;
}

set<int> Graph::ScoreDynamicDominatingSet() {
  vector<int> coverageCount(n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (adjMatrix[i][j] != -1) {
        coverageCount[i]++;
      }
    }
  }
  vector<int> scores = coverageCount;

  set<int> DS;
  for (int i = 0; i < n; ++i) {
    int minScore = MAX_INT;
    int minScoreIdx = -1;
    for (int j = 0; j < n; ++j) {
      if (scores[j] < minScore) {
        minScore = scores[j];
        minScoreIdx = j;
      }
    }
    bool added = false;
    for (int j = 0; j < n; ++j) {
      if (adjMatrix[minScoreIdx][j] != -1 && coverageCount[j] == 1) {
        added = true;
        DS.emplace(minScoreIdx);
        for (int k = 0; k < n; ++k) {
          if (adjMatrix[minScoreIdx][k] != -1) {
            coverageCount[k] = 0;
          }
        }
        break;
      }
    }
    if (!added) {
      for (int j = 0; j < n; ++j) {
        if (adjMatrix[minScoreIdx][j] != -1) {
          coverageCount[j]--;
          for (int k = 0; k < n; k++) {
            if (adjMatrix[j][k] != -1) {
              scores[j]++;
            }
          }
        }
      }
    }
    scores[minScoreIdx] = MAX_INT;
  }
  cout << "DS size: " << DS.size() << endl;
  cout << "DS members: \n";
  vector<int>tmp(DS.begin(), DS.end());
  printVec(tmp);
  //printVec(scores);
  //printVec(coverageCount);
  return DS;
}

set<int> Graph::ScoreBacktrackingDominatingSet() {
  vector<int> coverageCount(n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (adjMatrix[i][j] != -1) {
        coverageCount[i]++;
      }
    }
  }
  vector<int> scores = coverageCount;

  set<int> DS;
  for (int i = 0; i < n; ++i) {
    int minScore = MAX_INT;
    int minScoreIdx = -1;
    for (int j = 0; j < n; ++j) {
      if (scores[j] < minScore) {
        minScore = scores[j];
        minScoreIdx = j;
      }
    }
    if (coverageCount[minScoreIdx] == 1) {
      DS.emplace(minScoreIdx);
      for (int j = 0; j < n; ++j) {
        if (adjMatrix[minScoreIdx][j] != -1) {
          coverageCount[j] = 0;
        }
      }
    }
    else {
      for (int j = 0; j < n; ++j) {
        if (adjMatrix[minScoreIdx][j] != -1) {
          int covered_j = coverageCount[j];
          if (covered_j > 1) {
            scores[j]++;
            coverageCount[j]--;
          }
          else if (covered_j == 1) {
            for (int k = j - 1; k >= 0; k--) {
              if (adjMatrix[minScoreIdx][k] != -1) {
                scores[k]--;
                coverageCount[k]++;
              }
            }
            DS.emplace(minScoreIdx);
            for (int k = 0; k < n; ++k) {
              if (adjMatrix[minScoreIdx][k] != -1) {
                coverageCount[k] = 0;
              }
            }
            break;
          }
          else {
            scores[j]++;
          }
        }
      }
    }
    scores[minScoreIdx] = MAX_INT;
  }
  return DS;
}


bool Graph::checkCovered(set<int>& selected) {
  for (int i = 0; i < n; ++i) {
    bool covered = false;
    for (int j = 0; j < n; ++j) {
      if (adjMatrix[i][j] != -1 && selected.count(j) == 1) {
        covered = true;
        break;
      }
    }
    if (!covered) {
      return false;
    }
  }
  return true;
}



int Graph::evalKCenter(vector<int>& centers) {
  int max = -1;
  int size = centers.size();
  if (size == 0)
    return MAX_INT;
  for (int i = 0; i < n; ++i) {
    int minv = MAX_INT;
    for (int j = 0; j < size; ++j) {
      if (shortestPaths[i][centers[j]] != -1)
        minv = std::min(shortestPaths[i][centers[j]], minv);
    }
    max = std::max(max, minv);
  }
  //cout << "eval=" << max << endl;
  return max;
}

int Graph::evalKCenter(set<int>& centers) {
  vector<int> V_centers(centers.begin(), centers.end());
  return evalKCenter(V_centers);
}

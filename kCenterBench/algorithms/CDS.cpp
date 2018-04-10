#include "../stdafx.h"


// n^4 3-aprox
int Graph::centersCDS(int k) {
  calcShortestPath();
  set<int> edgeLengths;
  for (auto i : shortestPaths) {
    for (int j : i) {
      edgeLengths.emplace(j);
    }
  }
  bestScore = MAX_INT;
  for (int m : edgeLengths) {
    set<int> newCenters;
    newCenters = CDS(k, m);
    vector<int> VnewCenters(newCenters.begin(), newCenters.end());
    int newScore = evalKCenter(VnewCenters);
    if (newScore < bestScore) {
      bestScore = newScore;
      bestCenters = VnewCenters;
    }
  }
  if (debug) {
    cout << "Score = " << bestScore << " with centers:" << endl;
    for (int i : bestCenters) {
      cout << i << ' ';
    }
    cout << endl;
  }


  return bestScore;
}

set<int> Graph::CDS(int numCenters, int radius) {
  set<int> centers;
  set<int> dominated;
  Graph Gi(n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (shortestPaths[i][j] <= radius) {
        Gi.adjMatrix[i][j] = shortestPaths[i][j];
      }
    }
  }
  set<int> coveredVertices;
  vector<int> scores(n);
  for (int i = 0; i < n; ++i) {
    int degree = 0;
    for (int j = 0; j < n; ++j) {
      if (Gi.adjMatrix[i][j] != -1) {
        degree++;
      }
    }
    scores[i] = degree - 1; //self link overcount
  }
  for (int i = 0; i < numCenters; ++i) {

    //find vertex with maximum distance its closest center
    int f_i = 0;
    int dist = -1;
    for (int vert = 0; vert < n; vert++) {
      int closest_center = -1;
      int closest_center_distance = MAX_INT;
      for (int c : centers) {
        int d = Gi.adjMatrix[vert][c];
        if (d != -1 && d < closest_center_distance) {
          closest_center = c;
          closest_center_distance = d;
        }
      }
      if (closest_center_distance > dist) {
        f_i = i;
        dist = closest_center_distance;
      }
    }

    //select vertex with highest score in the neighbourhood of f_i
    int best_score = scores[f_i];
    int best_vertex = f_i;
    for (int v = 0; v < n; v++) {
      if (Gi.adjMatrix[f_i][v] != -1) {
        if (scores[v] > best_score && centers.find(v) == centers.end()) {
          best_score = scores[v];
          best_vertex = v;
        }
      }
    }

    //Modify scores
    for (int v = 0; v < n; v++) {
      if (Gi.adjMatrix[best_vertex][v] != -1) { // v = neighbours of best_vertex
        if (dominated.find(v) == dominated.end()) { // that are not already dominated
          for (int v2 = 0; v2 < n; v2++) {
            if (Gi.adjMatrix[v][v2] != -1) { // v2 neighbours of v
              scores[v2]--;
            }
            scores[v]++; // above counts itself as neighbour
          }
          dominated.emplace(v);
        }
      }
    }

    centers.emplace(best_vertex);
  }
  return centers;
}
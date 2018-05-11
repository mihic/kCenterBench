#include "../stdafx.h"


// n^4 3-aprox
int Graph::centersCDSn4(int k) {
  calcShortestPath();
  set<int> edgeLengths;
  for (auto i : shortestPaths) {
    for (int j : i) {
      edgeLengths.emplace(j);
    }
  }
  bestScore = MAX_INT;
  for (int m : edgeLengths) {
    for (int i = 0; i < n; ++i) {
      set<int> newCenters;
      newCenters = CDS(k, m, i);
      vector<int> VnewCenters(newCenters.begin(), newCenters.end());
      int newScore = evalKCenter(VnewCenters);
      if (newScore < bestScore) {
        bestScore = newScore;
        bestCenters = VnewCenters;
      }
    }
  }




  if (debug) {
    cout << "Score = " << bestScore << " with centers(" << bestCenters.size() << "):" << endl;
    for (int i : bestCenters) {
      cout << i << ' ';
    }
    cout << endl;
  }
  return bestScore;
}

// n^2 log n heuristic (bisection)
int Graph::centersCDS(int k, bool heu, bool plus) {
  calcShortestPath();
  set<int> edgeLengths;
  for (auto i : shortestPaths) {
    for (int j : i) {
      edgeLengths.emplace(j);
    }
  }
  vector<int> Vedge_lengths(edgeLengths.begin(), edgeLengths.end());
  sort(Vedge_lengths.begin(), Vedge_lengths.end());

  int low = 0;
  int high = edgeLengths.size();
  int mid = high / 2;
  bestScore = MAX_INT;
  int newScore;
  while (high - low > 2) {
    mid = (high + low) / 2;
    if (plus) {
      for (int i = 0; i < n; ++i) {
        set<int> newCenters;
        newCenters = CDS(k, Vedge_lengths[mid], i);
        vector<int> VnewCenters(newCenters.begin(), newCenters.end());
        newScore = evalKCenter(VnewCenters);
        if (newScore < bestScore) {
          bestScore = newScore;
          bestCenters = VnewCenters;
        }
      }
    }
    else {
      set<int> newCenters;
      newCenters = CDS(k, Vedge_lengths[mid], -1);
      vector<int> VnewCenters(newCenters.begin(), newCenters.end());
      newScore = evalKCenter(VnewCenters);
      if (newScore < bestScore) {
        bestScore = newScore;
        bestCenters = VnewCenters;
      }
    }

    if (heu) { //heuristic search
      if (bestScore <= Vedge_lengths[mid]) {
        high = mid;
      }
      else {
        low = mid;
      }
    }
    else { // 3-aprox search
      if (bestScore <= Vedge_lengths[mid]) {
        high = mid;
      }
      else {
        if (newScore > 3 * Vedge_lengths[mid]) {
          low = mid;
        }
        else {
          high = mid;
        }
      }
    }


    mid = (low + high) / 2;
  }
  if (debug) {
    cout << "Score = " << bestScore << " with centers(" << bestCenters.size() << "):" << endl;
    for (int i : bestCenters) {
      cout << i << ' ';
    }
    cout << endl;
  }


  return bestScore;
}

set<int> Graph::CDS(int numCenters, int radius, int init) {
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
    scores[i] = degree - 2; //self link overcount
  }
  for (int i = 0; i < numCenters; ++i) {
    int f_i = 0;
    int dist = -1;
    if (i == 0) { // first center can't be placed by this heurstic
      if (init == -1) { // if no init given, choose by solving best 1-center on bottlneck graph
        int bestv = 0;
        int vscore = MAX_INT;
        for (int v = 0; v < n; v++) {
          vector<int> vec = { v };
          int sc = evalKCenter(vec);
          if (sc < vscore) {
            vscore = sc;
            bestv = v;
          }
        }
        f_i = bestv;
      }
      else { // use the init given
        f_i = init;
      }
    }
    else {
      //find vertex with maximum distance to its closest center
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
          f_i = vert;
          dist = closest_center_distance;
        }
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
          for (int u = 0; u < n; u++) {
            if (Gi.adjMatrix[v][u] != -1) { // v2 neighbours of v
              scores[u]--;
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
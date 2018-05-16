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

set<int> Graph::CombinatorialMinMaximalIndependentSet(int k) {
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
    set<int> centers = Gi.CombinatorialMinMaximalIndependentSet(k);
    if (centers.size() <= k) {
      vector<int> Vcenters(centers.begin(), centers.end());
      int score = evalKCenter(Vcenters);
      if (debug) {
        cout << "Score = " << score << " with centers(" << Vcenters.size() << "):" << endl;
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
    c = (a + b) / 2;
    int m = VedgeLengths[c];
    Graph Gi(n);

    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (shortestPaths[i][j] <= m) {
          Gi.adjMatrix[i][j] = shortestPaths[i][j];
        }
      }
    }
    centers = Gi.CombinatorialMinMaximalIndependentSet(k);
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
  int m = VedgeLengths[b];
  Graph Gi(n);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (shortestPaths[i][j] <= m) {
        Gi.adjMatrix[i][j] = shortestPaths[i][j];
      }
    }
  }
  centers = Gi.CombinatorialMinMaximalIndependentSet(k);



  vector<int> Vcenters(centers.begin(), centers.end());
  int score = evalKCenter(Vcenters);
  if (debug) {
    cout << "Score = " << score << " with centers(" << Vcenters.size() << "):" << endl;
    for (int i : Vcenters) {
      cout << i << ' ';
    }
    cout << endl;
  }
  return score;
}

int Graph::centersRecursiveOptimalDominatingSetBin(int k) {
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
  int c;
  vector<int> centers;
  vector<int> best_centers;
  int best_score = MAX_INT;
  vector<int> VedgeLengths(edgeLengths.begin(), edgeLengths.end());
  while (b - a > 1) {
    c = (a + b) / 2;
    int m = VedgeLengths[c];
    Graph Gi(n);
    RecursiveState s(n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (shortestPaths[i][j] <= m) {
          Gi.adjMatrix[i][j] = shortestPaths[i][j];
          s.num_choice[i]++;
        }
      }
      if (s.num_choice[i] > s.delta) {
        s.delta = s.num_choice[i];
      }
    }
    s.k = k;
    Gi.RecursiveOptimalDominatingSet(s);
    centers = s.min_dominating_set;
    int num_centers = centers.size();
    if (debug) {
      cout << "m=" << m << " csize=" << num_centers << " c=" << c << endl;
    }

    if (centers.size() > k) {
      a = c;
    }
    else {
      int score = evalKCenter(centers);
      if (score < best_score) {
        best_centers = centers;
        best_score = score;

      }
      b = c;
    }
  }
  if (debug) {
    cout << "Score = " << best_score << " with centers(" << best_centers.size() << "):" << endl;
    for (int i : best_centers) {
      cout << i << ' ';
    }
    cout << endl;
  }
  return best_score;
}

int Graph::centersRecursiveOptimalDominatingSet(int k) {
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
    RecursiveState s(n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (shortestPaths[i][j] <= m) {
          Gi.adjMatrix[i][j] = shortestPaths[i][j];
          s.num_choice[i]++;
        }
      }
      if (s.num_choice[i] > s.delta) {
        s.delta = s.num_choice[i];
      }
    }
    s.k = MAX_INT;
    Gi.RecursiveOptimalDominatingSet(s);
    vector<int> centers = s.min_dominating_set;
    if (debug) {
      cout << "m=" << m << " csize=" << centers.size() << endl;
    }
    if (centers.size() <= k) {
      int score = evalKCenter(centers);
      if (debug) {
        cout << "Score = " << score << " with centers(" << centers.size() << "):" << endl;
        for (int i : centers) {
          cout << i << ' ';
        }
        cout << endl;
      }
      return score;
    }
  }
}



void Graph::RecursiveOptimalDominatingSet(RecursiveState &s) {
  if (s.level > s.n) {
    cout << "That should not have happened";
    exit(-1);
  }
  if (s.dominating_set.size() > s.k) {
    return;
  }
  if (s.num_undominated_vertices == 0) {
    if (s.dominating_set.size() < s.min_dominating_set.size()) {
      s.min_dominating_set = s.dominating_set;
    }
    return;
  }

  //hitreje
  //q = (x + y - 1) / y;

  //ziher
  //q = x / y;
  //if (q * y < x) ++q;

  int extra = s.num_undominated_vertices / s.delta;
  //roundup
  if (s.delta * extra < s.num_undominated_vertices) {
    ++extra;
  }
  if (s.dominating_set.size() + extra >= s.min_dominating_set.size()) {
    return;
  }
  for (int c : s.num_choice) {
    if (c == 0) return;
  }

  //try vertex[level] as not included

  for (int i = 0; i < n; ++i) {
    if (adjMatrix[s.level][i] != -1) {
      s.num_choice[i]--;
    }
  }
  s.level++;
  RecursiveOptimalDominatingSet(s);
  s.level--;
  for (int i = 0; i < n; ++i) {
    if (adjMatrix[s.level][i] != -1) {
      s.num_choice[i]++;
    }
  }


  //try vertex[level] as included

  s.dominating_set.push_back(s.level);
  for (int i = 0; i < n; ++i) {
    if (adjMatrix[s.level][i] != -1) {
      if (s.num_reds[i] == 0) {
        s.num_undominated_vertices--;
      }
      s.num_reds[i]++;
    }
  }
  s.level++;
  RecursiveOptimalDominatingSet(s);
  s.level--;
  for (int i = 0; i < n; ++i) {
    if (adjMatrix[s.level][i] != -1) {
      if (s.num_reds[i] == 1) {
        s.num_undominated_vertices++;
      }
      s.num_reds[i]--;
    }
  }
  s.dominating_set.pop_back();
}

void Graph::RecursiveOptimalDominatingSetWithDeletion(RecursiveState &s) {
  if (isDeleted[s.level]) {
    s.level++;
    RecursiveOptimalDominatingSetWithDeletion(s);
    s.level--;
    return;
  }
  if (s.level > s.n) {
    cout << "That should not have happened";
    exit(-1);
  }
  if (s.dominating_set.size() > s.k) {
    return;
  }
  if (s.num_undominated_vertices == 0) {
    if (s.dominating_set.size() < s.min_dominating_set.size()) {
      s.min_dominating_set = s.dominating_set;
    }
    return;
  }

  //hitreje
  //q = (x + y - 1) / y;

  //ziher
  //q = x / y;
  //if (q * y < x) ++q;

  int extra = s.num_undominated_vertices / s.delta;
  //roundup
  if (s.delta * extra < s.num_undominated_vertices) {
    ++extra;
  }
  if (s.dominating_set.size() + extra >= s.min_dominating_set.size()) {
    return;
  }
  for (int c : s.num_choice) {
    if (c == 0) return;
  }

  //try vertex[level] as not included

  for (int i = 0; i < n; ++i) {
    if (!isDeleted[i] && adjMatrix[s.level][i] != -1) {
      s.num_choice[i]--;
    }
  }
  s.level++;
  RecursiveOptimalDominatingSetWithDeletion(s);
  s.level--;
  for (int i = 0; i < n; ++i) {
    if (!isDeleted[i] && adjMatrix[s.level][i] != -1) {
      s.num_choice[i]++;
    }
  }


  //try vertex[level] as included

  s.dominating_set.push_back(s.level);
  for (int i = 0; i < n; ++i) {
    if (!isDeleted[i] && adjMatrix[s.level][i] != -1) {
      if (s.num_reds[i] == 0) {
        s.num_undominated_vertices--;
      }
      s.num_reds[i]++;
    }
  }
  s.level++;
  RecursiveOptimalDominatingSetWithDeletion(s);
  s.level--;
  for (int i = 0; i < n; ++i) {
    if (!isDeleted[i] && adjMatrix[s.level][i] != -1) {
      if (s.num_reds[i] == 1) {
        s.num_undominated_vertices++;
      }
      s.num_reds[i]--;
    }
  }
  s.dominating_set.pop_back();
}

bool Graph::GraphReductionRule1(int v) {
  if (isDeleted[v])return false;
  set<int> exit;
  set<int> guard;
  set<int> prison;
  set<int> Nv;

  //populate neighbourhood of v
  for (int i = 0; i < n; ++i) {
    if (!isDeleted[i] && adjMatrix[v][i] != -1 && v != i) {
      Nv.emplace(i);
    }
  }


  //then partition that neighbourhood in to 3 disjoint sets
  //populate exit

  for (int u : Nv) {
    for (int i = 0; i < n; ++i) {
      if (i != v && i != u && adjMatrix[u][i] != -1 && !isDeleted[i] && Nv.count(i) == 0) {
        exit.emplace(u);
        break;
      }
    }
  }
  //populate guard
  for (int u : Nv) {
    if (exit.count(u) == 0) {
      for (int i = 0; i < n; ++i) {
        if (i != v && i != u && adjMatrix[u][i] != -1 && !isDeleted[i] && exit.count(i) != 0) {
          guard.emplace(u);
          break;
        }
      }
    }
  }
  //populate prison
  for (int u : Nv) {
    if (exit.count(u) == 0 && guard.count(u) == 0) {
      prison.emplace(u);
    }
  }
  //if (debug) {
  //  cout << "partition: ";
  //  cout << exit.size() << ' ';
  //  cout << guard.size() << ' ';
  //  cout << prison.size() << ' ';
  //  cout << Nv.size() << ' ';
  //  cout << endl;
  //}

  //apply rule 1
  for (int p : prison) {
    if (!isWhite[p]) {
      dominatingSet.push_back(v);
      isDeleted[v] = true;
      for (int u : Nv) {
        isWhite[u] = true;
      }
      for (int u : guard) {
        isDeleted[u] = true;
      }
      for (int u : prison) {
        isDeleted[u] = true;
      }
      return true;
    }
  }
  return false;
}

void Graph::ReduceGraph() {

  isWhite = vector<bool>(n, false);
  bool done = false;
  while (!done) {
    done = true;
    for (int i = 0; i < n; ++i) {
      if (GraphReductionRule1(i)) {
        done = false;
      }
    }
  }
  //cout << "done, dom set so far" << endl;
  //printVec(dominatingSet);
}

int Graph::centersReduceAndRecurse(int k) {
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
    Gi.ReduceGraph();
    int removed = 0;
    for (bool b : Gi.isDeleted) {
      if (b) removed++;
    }
    //cout << "removed:" << removed << " dom_size:" << Gi.dominatingSet.size()<< endl;
    vector<int> centers;
    if (removed < n) {
      RecursiveState s(n);
      for (int i = 0; i < n; ++i) {
        if (!Gi.isDeleted[i]) {
          for (int j = 0; j < n; ++j) {
            if (!Gi.isDeleted[j] && Gi.adjMatrix[i][j] != -1) {
              s.num_choice[i]++;
            }
          }
          if (s.num_choice[i] > s.delta) {
            s.delta = s.num_choice[i];
          }
        }
      }
      s.k = MAX_INT;
      s.min_dominating_set.clear();
      for (int i = 0; i < n; ++i) {
        if (!Gi.isDeleted[i]) {
          s.min_dominating_set.push_back(i);
        }
      }
      s.num_undominated_vertices = n - removed;
      /*for (int i : Gi.dominatingSet) {
        for (int j = 0; j < n; ++j) {
          if (!Gi.isDeleted[j] && Gi.adjMatrix[i][j] != -1) {
            s.num_reds[j]++;
            s.num_choice[j]++;
            if (s.num_reds[j] == 1) {
              s.num_undominated_vertices--;
            }
          }
        }
      }*/
      for (int i = 0; i < n; ++i) {
        if (!Gi.isDeleted[i] && Gi.isWhite[i]) {
          s.num_undominated_vertices--;
          s.num_reds[i]++;
          s.num_choice[i]++;
        }
      }
      //cout << "min dom set" << s.min_dominating_set.size() << endl;
      Gi.RecursiveOptimalDominatingSetWithDeletion(s);
      centers = s.min_dominating_set;
      //cout << "precalc dom set:" << Gi.dominatingSet.size() << endl;
      for (int d : Gi.dominatingSet) {
        centers.push_back(d);
      }
    }
    else {
      centers = Gi.dominatingSet;
    }

    if (debug) {
      cout << "m=" << m << " csize=" << centers.size() << endl;
    }
    if (centers.size() <= k) {
      int score = evalKCenter(centers);
      if (debug) {
        cout << "Score = " << score << " with centers(" << centers.size() << "):" << endl;
        for (int i : centers) {
          cout << i << ' ';
        }
        cout << endl;
      }
      return score;
    }
  }
}

int Graph::centersReduceAndRecurseBin(int k) {
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
  int c;
  vector<int> centers;
  vector<int> best_centers;
  int best_score = MAX_INT;
  vector<int> VedgeLengths(edgeLengths.begin(), edgeLengths.end());
  while (b - a > 1) {
    c = (a + b) / 2;
    int m = VedgeLengths[c];
    Graph Gi(n);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (shortestPaths[i][j] <= m) {
          Gi.adjMatrix[i][j] = shortestPaths[i][j];
        }
      }
    }
    Gi.ReduceGraph();
    int removed = 0;
    for (bool b : Gi.isDeleted) {
      if (b) removed++;
    }
    cout << "removed:" << removed << endl;
    vector<int> centers;
    if (removed < n) {
      RecursiveState s(n);
      for (int i = 0; i < n; ++i) {
        if (!Gi.isDeleted[i]) {
          for (int j = 0; j < n; ++j) {
            if (!Gi.isDeleted[j] && Gi.adjMatrix[i][j] != -1) {
              s.num_choice[i]++;
            }
          }
          if (s.num_choice[i] > s.delta) {
            s.delta = s.num_choice[i];
          }
        }
      }
      s.k = k;
      s.min_dominating_set.clear();
      for (int i = 0; i < n; ++i) {
        if (!Gi.isDeleted[i]) {
          s.min_dominating_set.push_back(i);
        }
      }
      Gi.RecursiveOptimalDominatingSetWithDeletion(s);
      centers = s.min_dominating_set;
      for (int d : Gi.dominatingSet) {
        centers.push_back(d);
      }
    }
    else {
      centers = Gi.dominatingSet;
    }
    if (debug) {
      cout << "m=" << m << " csize=" << centers.size() << " c=" << c << endl;
    }

    if (centers.size() > k) {
      a = c;
    }
    else {
      int score = evalKCenter(centers);
      if (score < best_score) {
        best_centers = centers;
        best_score = score;

      }
      b = c;
    }
  }
  if (debug) {
    cout << "Score = " << best_score << " with centers(" << best_centers.size() << "):" << endl;
    for (int i : best_centers) {
      cout << i << ' ';
    }
    cout << endl;
  }
  return best_score;
}

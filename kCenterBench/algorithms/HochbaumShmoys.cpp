#include "../stdafx.h"

int Graph::centersHochbaumShmoys(int k) {
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
  	Gi.Sqaure();
  	//set<int> centers = Gi.RadnomMaximalIndependentSet();
    set<int> centers = Gi.GreedyMaximalIndependentSet();
  	if (centers.size() <= k) {
  		vector<int> Vcenters(centers.begin(), centers.end());
      int score = evalKCenter(Vcenters);
  		if (debug) {
        cout << "Score = " << score << " with centers(" << Vcenters.size() << "):" << endl;
  			for (int i : centers) {
  				cout << i << ' ';
  			}
  			//cout << " at position m= " << m;
  			cout << endl;
  		}
      return score;
  	}
  }
}
int Graph::centersHochbaumShmoysBin(int k) {
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
   Gi.Sqaure();
    //centers = Gi.RadnomMaximalIndependentSet();
    centers = Gi.GreedyMaximalIndependentSet();
    int s = centers.size();
    if (debug) {
      cout << "m=" << m << "csize=" << s << endl;
    }
    if (centers.size() <= k) {
      b = c;
    }
    else {
      a = c;
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
  Gi.Sqaure();
  centers = Gi.GreedyMaximalIndependentSet();
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

int Graph::centersBottleneckHeuristicsBin(int k) {
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
    centers = Gi.GreedyMaximalIndependentSet();
    int s = centers.size();
    if (debug) {
      cout << "a,c,b=" << a << ','<<c <<','<< b << " csize=" << s << endl;
    }
    if (centers.size() <= k) {
      b = c;
    }
    else {
      a = c;
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
  centers = Gi.GreedyMaximalIndependentSet();
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

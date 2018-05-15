
using namespace std;

class Graph {
public:
  //bruteforce globals
  vector<int> bestCenters;
  int bestScore;
  int currentScore;
  //bruteforce globals

  //
  vector<int> rangeCenters;
  vector<int> oneToN;
  vector<int> VbyDeg;
  //

  void makeOneToN();

  void makeVbyDeg();


  vector<vector<int>> adjMatrix;
  vector<vector<int>> shortestPaths;
  int n;
  Graph(int a) : adjMatrix(a, vector<int>(a, -1)), n(a) {}

  Graph() {

  }


  static Graph createRandomGraph(int n, double w, int cmin, int cmax);



  void calcShortestPath();

  void Sqaure();

  set<int> RadnomMaximalIndependentSet();
  set<int> GreedyMaximalIndependentSet();
  set<int> ScoreDynamicDominatingSet();
  set<int> CombinatorialMinMaximalIndependentSet(int k);
  void BruteForceIndependentSet(vector<int>& vec, set<int>& result, int i, int d);
  bool checkCovered(set<int>& vec);

  int evalKCenter(vector<int> &centers);
  int evalKCenter(set<int> &centers);

  int centersBruteForce(int k, bool optimized);
  void centersBruteForceRec(vector<int> &vec, int i, int d);
  void centersBruteForceRecOpt(vector<int> &vec, int i, int d);
  int centersBottleneckBruteForce(int k);
  int centersBottleneckBruteForceBin(int k);

  int centersRecursiveOptimalDominatingSetBin(int k);

  int centersRecursiveOptimalDominatingSet(int k);



  class RecursiveState {
  public:
    vector<int> dominating_set;
    vector<int> min_dominating_set;
    int num_undominated_vertices;
    vector<int> num_reds;
    vector<int> num_choice;
    int delta;
    int n;
    int level;
    int k;
    RecursiveState(int n) : min_dominating_set(vector<int>(n)), num_undominated_vertices(n),
                            num_reds(vector<int>(n,0)),
                            num_choice(vector<int>(n)),delta(0),level(0),n(n){
      for (int i = 0; i < n; ++i) {
        min_dominating_set[i] = i;
      }
    }
  };

  void RecursiveOptimalDominatingSet(RecursiveState & s);

  void greedyAddCenter(vector<int> &to, unordered_set<int> &from);
  int centersGreedy(int k);
  int centersGreedyPlus(int k);
  int centersGreedyRandom(int k);


  void gonzalezAddCenter(vector<int> &to, vector<int> &from);
  int centersGonzalezRandom(int k);
  int centersGonazalezPlus(int k);
  int centersGonzalez1C(int k);

  int centersPlesnik(int k, bool deg);
  int centersPlesnikPlus(int k);
  bool Range(int k, int B, bool deg);

  int centersHochbaumShmoys(int k);
  int centersHochbaumShmoysBin(int k);
  int centersBottleneckHeuristicsBin(int k);
  int centersBottleneckScore(int k);

  int centersCDSn4(int k);
  int centersCDS(int k, bool heu, bool plus);
  set<int> CDS(int k, int radius, int init);

};

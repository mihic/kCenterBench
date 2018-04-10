
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
  vector<vector<int>> sqAdjMatrix;
  vector<vector<int>> shortestPaths;
  int n;
  Graph(int a) : adjMatrix(a, vector<int>(a, -1)), n(a) {}

  Graph() {

  }


  static Graph createRandomGraph(int n, double w, int cmin, int cmax);



  void calcShortestPath();

  void calcSqAdjMatrix();

  set<int> RadnomMaximalIndependentSet();

  int evalKCenter(vector<int> &centers);
  int evalKCenter(set<int> &centers);

  int centersBruteForce(int k, bool optimized);
  void centersBruteForceRec(vector<int> &vec, int n, int i, int d);
  void centersBruteForceRecOpt(vector<int> &vec, int n, int i, int d);

  void greedyAddCenter(vector<int> &to, unordered_set<int> &from);
  int centersGreedy(int k);
  int centersGreedyPlus(int k);
  int centersGreedyRandom(int k);


  void gonzalezAddCenter(vector<int> &to, vector<int> &from);
  int centersGonzalezRandom(int k);
  int centersGonzalez1C(int k);

  int centersPlesnik(int k, bool deg);
  int centersPlesnikPlus(int k);
  bool Range(int k, int B, bool deg);

  int centersHochbaumShmoys(int k);

  int centersCDS(int k);
  set<int> CDS(int k, int radius);

};

#include "stdafx.h"
#include "kCenterBench.hpp"

bool debug;

int main(int argc, const char * argv [])
{
  Graph g;
  int k = -1;
  cxxopts::Options options("kCenterBench", "k-center algorithms benchmark tool");

  options.add_options()
    ("d,debug", "Enable debugging")
    ("i,input", "Input file path", cxxopts::value<std::string>())
    ("c,centers", "number of centers", cxxopts::value<int>())
    ("m,method", "method to use", cxxopts::value<std::string>())
    ("f,format", "input format type", cxxopts::value<std::string>())
    ;
  auto result = options.parse(argc, argv);
  if (result.count("debug") > 0) {
    debug = true;
    cout << "debuging enabled" << endl;
  }
  else {
    debug = false;
  }
  if (result.count("input") > 0) {
    string file_path = result["input"].as<string>();
    if (debug) {
      cout << "opening file: \"" << file_path << '\"' << endl;
    }
    if (result.count("format") > 0) {
      string format = result["format"].as<std::string>();
      if (format == "pajek") {
        g = read_pajek(file_path);
      }
      else if (format == "pmed") {
        g = read_pmed(file_path, &k);
      }
      else if (format == "graph") {
        g = read_graph(file_path);
      }
      else {
        cerr << "unrecgonized file format \"" << format << '"' << endl;
        exit(-1);
      }
    }
    else {
      cerr << "file format not specified" << endl;
      exit(-1);
    }

  }
  else {
    if (debug) {
      cout << "no file specified" << endl;
    }
    else {
      cerr << "no input file" << endl;
      exit(-1);
    }
  }

  if (result.count("centers") > 0) {
    k = result["centers"].as<int>();
  }
  else if (k == -1) {
    cerr << "number of centers not specified" << endl;
    exit(-1);
  }



  if (result.count("method") > 0) {
    string method = result["method"].as<std::string>();
    int score = -1;
    auto time_start = std::chrono::steady_clock::now();

    if (method == "bf") {
      if (debug) {
        cout << "Bruteforcing " << k << "-center" << endl;
      }
      g.centersBruteForce(k, false);
      if (debug) {
        cout << "Best score: " << g.bestScore << " with centers:" << endl;
        printVec(g.bestCenters);
      }
      score = g.bestScore;
    }
    else if (method == "bf1") {
      if (debug) {
        cout << "Bruteforcing (OPT) " << k << "-center" << endl;
      }
      g.centersBruteForce(k, true);
      if (debug) {
        cout << "Best score: " << g.bestScore << " with centers:" << endl;
        printVec(g.bestCenters);
      }
      score = g.bestScore;
    }
    else if (method == "greedy") {
      score = g.centersGreedy(k);
    }
    else if (method == "greedy+") {
      score = g.centersGreedyPlus(k);
    }
    else if (method == "greedyrnd") {
      score = g.centersGreedyRandom(k);
    }
    else if (method == "gonzalez1c") {
      score = g.centersGonzalez1C(k);
    }
    else if (method == "gonzalezrnd") {
      score = g.centersGonzalezRandom(k);
    }
    else if (method == "plesnikrnd") {
      score = g.centersPlesnik(k, false);
    }
    else if (method == "plesnikdeg") {
      score = g.centersPlesnik(k, true);
    }
    else if (method == "plesnikdeg+") {
      score = g.centersPlesnikPlus(k);
    }
    else if (method == "hochbaumshmoys") {
      score = g.centersHochbaumShmoys(k);
    }
    else if (method == "CDS") {
      score = g.centersCDS(k);
    }
    else if (method == "CDSh") {
      score = g.centersCDSh(k);
    }
    else {
      cerr << "invalid method \"" << method << '"' << endl;
      exit(-1);
    }

    auto time_end = std::chrono::steady_clock::now();
    std::chrono::duration<int, std::milli> time_total = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    int total_time = time_total.count();
    if (debug) {
      cout << "time: " << total_time << " ms" << endl;
    }
    else {
      cout << score << ',' << total_time << endl;
    }

  }


  return 0;
}
Graph read_pmed(string file, int *k) {
  int n;
  ifstream infile(file);
  string line;
  getline(infile, line);
  istringstream iss(line);
  int a, b, c;
  if (iss >> a >> b >> c) {
    n = a;
    *k = c;
  }
  else {
    cerr << "failed to parse pmed" << endl;
    exit(-1);
  }
  Graph g(n);

  while (getline(infile, line)) { //add arcs
    istringstream iss(line);
    int a, b, c;
    if (iss >> a >> b >> c) {
      //cout << "Adding arc " << a << " <-> " << b << " (" << c << ")" << endl;
      g.adjMatrix[a - 1][b - 1] = c;
      g.adjMatrix[b - 1][a - 1] = c;
    }
  }
  return g;
}

Graph read_pajek(string file) {
  int n;
  ifstream infile(file);
  string line;
  while (getline(infile, line)) //look for *vertices n
  {
    istringstream iss(line);
    string token;
    if (iss >> token >> n) {
      //cout << "token=" << token << " n=" << n << endl;
      if (token == "*vertices") {
        //cout << n << " vertices" << endl;
        break;
      }
    }
  }
  Graph g(n);
  while (getline(infile, line)) { //look for *arcs
    istringstream iss(line);
    string token;
    if (iss >> token) {
      if (token == "*arcs") {
        //cout << "arcs" << endl;
        break;
      }
    }
  }

  while (getline(infile, line)) { //add arcs
    istringstream iss(line);
    int a, b, c;
    if (iss >> a >> b >> c) {
      //cout << "Adding arc " << a << " <-> " << b << " (" << c << ")" << endl;
      g.adjMatrix[a - 1][b - 1] = c;
      g.adjMatrix[b - 1][a - 1] = c;
    }
  }
  return g;

}

Graph read_graph(string file) {
  int n = 0;
  ifstream infile(file);
  string line;
  while (getline(infile, line)) { //find [Nodes]
    istringstream iss(line);
    string token;
    if (iss >> token) {
      if (token == "[Nodes]") {
        //cout << "Strat counting nodes..." << endl;
        break;
      }
    }
  }
  while (getline(infile, line)) { //count nodes, find [Edges]
    istringstream iss(line);
    string token;
    if (iss >> token) {
      if (token == "[Edges]") {
        //cout << "counted " << n << " nodes" << endl;
        break;
      }
      else if (token[0] == '(') {
        n++;
      }
    }
  }
  Graph g(n);
  while (getline(infile, line)) {
    int a, b, w, c;
    char ime[10];
    //                                (N2, [-10,-29], [0,0])
    if (sscanf(line.c_str(), "(%9[^,], [%d,%d], [%d,%d])", ime, &a, &b, &w, &c) == 5) {
      //cout << " found edge " << ime << " from " << a << " to " << b <<
      //	" with weight " << w << " and cost " << c << endl;
      g.adjMatrix[a][b] = c;
      g.adjMatrix[b][a] = c;
    }
  }
  return g;
}

void print_pajek(Graph g) {
  int n = g.n;
  cout << "*vertices " << n << endl;
  cout << "*arcs" << endl;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (g.adjMatrix[i][j] != -1) {
        cout << i + 1 << ' ' << j + 1 << ' ' << g.adjMatrix[i][j] << endl;
      }
    }
  }
}


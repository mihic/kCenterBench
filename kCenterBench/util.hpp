#include "stdafx.h"

using namespace std;

#define MAX_INT numeric_limits<int>::max()

template<typename T>
void printVec(vector<T> &c) {
  int n = c.size();
  for (int i = 0; i < n; ++i) {
    cout << c[i] << ' ';
  }
  cout << endl;
}

template<typename T>
void printMatrix(vector<vector<T>> &mat) {
  int n = mat.size();
  int m = mat[0].size();
  cout << setw(3);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      cout << setw(3) << mat[i][j] << ' ';
    }
    cout << endl;
  }
}


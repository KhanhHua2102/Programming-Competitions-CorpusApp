#include <bits/stdc++.h>

using namespace std;

int Q, K;
vector<vector<int>> queries;
vector<int> A;
// l segment = [0, mid_index)
// r segment = [mid_index, size)
int mid_index = 0;

void processInput() {
  cin >> Q >> K;

  // v[i][0] = query type
  // v[i][1] = value if insertion query
  // 0 = M, -1 = L, 1 = R
  queries = vector<vector<int>>(Q, vector<int>(2, 0));
  for (int i = 0; i < Q; ++i) {
    string query_type;
    cin >> query_type;

    if (query_type == "M")
      continue;

    if (query_type == "L") {
      queries[i][0] = -1;
    } else {
      queries[i][0] = 1;
    }
    cin >> queries[i][1];
  }
}

vector<vector<vector<int>>> dp_R;
vector<vector<vector<int>>> dp_L;
vector<vector<vector<int>>> dp_mid_R;
vector<vector<vector<int>>> dp_mid_L;

int dpR(int index, int k, int hasStartedSubsequence) {
  // cout << "BEGIN -- index: " << index << ", k: " << k
  //      << ", ss: " << hasStartedSubsequence << endl;
  // base cases
  // will be forced to remove all remaining elements, so sum would be 0.
  if (index == k - 1)
    return 0;
  // if (index < k)
  //   return INT_MAX;

  // if k is -1, we actually removed one too many elements, and so the
  // subsequence ended at index + 1.
  if (k == -1)
    return 0;

  if (index < 0) {
    // <= 0 ??
    if (k == 0 && hasStartedSubsequence) {
      return 0;
    } else {
      return INT_MIN;
    }
  }

  // check DP table
  if (dp_R[index][k][hasStartedSubsequence] != INT_MAX)
    return dp_R[index][k][hasStartedSubsequence];

  // skip value at index i.e. don't include in menace array
  int skip = 0;
  if (hasStartedSubsequence) {
    skip = dpR(index - 1, k - 1, 1);
  } else {
    // if we have not started the subsequence, and we skip, we get to skip for
    // free i.e. don't decrease k, and don't start the subsequece.
    // we can also start the subsequence with a skip, so try that too.
    skip = max(dpR(index - 1, k, 0), dpR(index - 1, k - 1, 1));
  }

  // don't skip value at index i.e. include in menace array
  // if we haven't started the subsequence yet, we may choose to start it here.
  // we've either already started it or are starting it for the first time, so
  // passing 1
  int dont_skip = A[mid_index + index] + dpR(index - 1, k, 1);

  return dp_R[index][k][hasStartedSubsequence] = max(skip, dont_skip);
}

int dpL(int index, int k, int hasStartedSubsequence) {
  // cout << "BEGIN -- index: " << index << ", k: " << k
  //      << ", ss: " << hasStartedSubsequence << endl;
  // base cases
  // will be forced to remove all remaining elements, so sum would be 0.
  if (index == k - 1)
    return 0;

  // if k is -1, we actually removed one too many elements, and so the
  // subsequence ended at index + 1.
  if (k == -1) {
    // cout << "return 0 (k = 1)" << endl;
    return 0;
  }

  if (index < 0) {
    // <= 0 ??
    if (k == 0 && hasStartedSubsequence) {
      // cout << "return 0 (k = 0)" << endl;
      return 0;
    } else {
      // cout << "return -inf ()" << endl;
      return INT_MIN;
    }
  }

  // cout << "-- pre dp check. index: " << index << ", k: " << k
  // << ", ss: " << hasStartedSubsequence << endl;
  // check DP table
  if (dp_L[index][k][hasStartedSubsequence] != INT_MAX)
    return dp_L[index][k][hasStartedSubsequence];
  // cout << "-- post dp check: " << endl;

  // skip value at index i.e. don't include in menace array
  int skip = 0;
  if (hasStartedSubsequence) {
    skip = dpL(index - 1, k - 1, 1);
  } else {
    // if we have not started the subsequence, and we skip, we get to skip for
    // free i.e. don't decrease k, and don't start the subsequece.
    // we can also start the subsequence with a skip, so try that too.
    skip = max(dpL(index - 1, k, 0), dpL(index - 1, k - 1, 1));
  }

  // don't skip value at index i.e. include in menace array
  // if we haven't started the subsequence yet, we may choose to start it here.
  // we've either already started it or are starting it for the first time, so
  // passing 1
  // - instead of + in dpR
  // cout << "-- index: " << index << ", k: " << k
  // << ", ss: " << hasStartedSubsequence << endl;
  // cout << "-- relative index before: " << mid_index - index - 1 << endl;
  // cout << "-- ADDING " << A[mid_index - index - 1] << endl;
  int dont_skip = A[mid_index - index - 1] + dpL(index - 1, k, 1);
  // cout << "-- index: " << index << ", k: " << k
  // << ", ss: " << hasStartedSubsequence << endl;
  // cout << "-- relative index: " << mid_index - index - 1 << endl;

  dp_L[index][k][hasStartedSubsequence] = max(skip, dont_skip);
  // cout << "result: " << dp_L[index][k][hasStartedSubsequence] << endl;
  return dp_L[index][k][hasStartedSubsequence];
}

int dpEndMidR(int index, int k, int hasStartedSubsequence) {
  // base cases
  // will be forced to remove all remaining elements, so sum would be 0.
  if (index == k - 1)
    return 0;

  // if k is -1, we actually removed one too many elements, and so the
  // subsequence is invalid.
  // so return invalid value, not 0;
  if (k == -1)
    return INT_MIN;

  if (index < 0) {
    // <= 0 ??
    if (k == 0 && hasStartedSubsequence) {
      return 0;
    } else {
      return INT_MIN;
    }
  }

  // check DP table
  if (dp_mid_R[index][k][hasStartedSubsequence] != INT_MAX)
    return dp_mid_R[index][k][hasStartedSubsequence];

  // skip value at index i.e. don't include in menace array
  int skip = 0;
  if (hasStartedSubsequence) {
    skip = dpEndMidR(index - 1, k - 1, 1);
  } else {
    // if we have not started the subsequence, and we skip, we get to skip for
    // free i.e. don't decrease k, and don't start the subsequece.
    // we can also start the subsequence with a skip, so try that too.
    skip = max(dpEndMidR(index - 1, k, 0), dpEndMidR(index - 1, k - 1, 1));
  }

  // don't skip value at index i.e. include in menace array
  // if we haven't started the subsequence yet, we may choose to start it here.
  // we've either already started it or are starting it for the first time, so
  // passing 1
  int dont_skip = A[mid_index + index] + dpEndMidR(index - 1, k, 1);

  return dp_mid_R[index][k][hasStartedSubsequence] = max(skip, dont_skip);
}

int dpEndMidL(int index, int k, int hasStartedSubsequence) {
  // cout << "-- BEGIN -- index: " << index << ", k: " << k
  //      << ", ss: " << hasStartedSubsequence << endl;
  // base cases
  // will be forced to remove all remaining elements, so sum would be 0.
  if (index == k - 1)
    return 0;

  // if k is -1, we actually removed one too many elements, and so the
  // subsequence ended at index + 1.
  if (k == -1) {
    // cout << "return -inf (k = 1)" << endl;
    return INT_MIN;
  }

  if (index < 0) {
    // <= 0 ??
    if (k == 0 && hasStartedSubsequence) {
      // cout << "return 0 (k = 0)" << endl;
      return 0;
    } else {
      // cout << "return -inf (k = 1)" << endl;
      return INT_MIN;
    }
  }

  // cout << "1" << endl;
  // check DP table
  if (dp_mid_L[index][k][hasStartedSubsequence] != INT_MAX)
    return dp_mid_L[index][k][hasStartedSubsequence];
  // cout << "2" << endl;

  // skip value at index i.e. don't include in menace array
  int skip = 0;
  if (hasStartedSubsequence) {
    skip = dpEndMidL(index - 1, k - 1, 1);
  } else {
    // if we have not started the subsequence, and we skip, we get to skip for
    // free i.e. don't decrease k, and don't start the subsequece.
    // we can also start the subsequence with a skip, so try that too.
    skip = max(dpEndMidL(index - 1, k, 0), dpEndMidL(index - 1, k - 1, 1));
  }

  // don't skip value at index i.e. include in menace array
  // if we haven't started the subsequence yet, we may choose to start it here.
  // we've either already started it or are starting it for the first time, so
  // passing 1
  // - instead of + in dpR
  int dont_skip = A[mid_index - index - 1] + dpEndMidL(index - 1, k, 1);

  return dp_mid_L[index][k][hasStartedSubsequence] = max(skip, dont_skip);
}

int menaceQuery() {
  int N = A.size();
  if (N == 0)
    return 0;

  // RHS
  int R = dpR(N - mid_index - 1, K, 0);
  // cout << "R: " << R << endl;
  // LHS
  int L = dpL(mid_index - 1, K, 0);
  // cout << "L: " << L << endl;
  // Crossover
  int C = INT_MIN;
  for (int i = 0; i <= K; ++i) {
    // cout << "-- " << i << endl;
    // cout << "-- " << (K - i) << endl;
    // cout << "-- " << mid_index << endl;
    int mid_L = dpEndMidL(mid_index - 1, i, 0);
    int mid_R = dpEndMidR(N - mid_index - 1, K - i, 0);
    if ((N - mid_index) >= (K - i) && mid_index >= i) {
      // cout << "mid_L: " << mid_L << endl;
      // cout << "mid_R: " << mid_R << endl;
      C = max(C, mid_L + mid_R);
      // cout << "C: " << C << endl;
    }
  }
  return max(max(max(R, L), C), 0);
}

// -1 = L, 1 = R
void insertionQuery(int l_or_r, int v) {
  // L means push front and increment mid_index
  if (l_or_r == -1) {
    A.insert(A.begin(), v);
    mid_index++;
  } else {
    A.push_back(v);
  }
}

void solve() {
  // initialise global variables
  A = vector<int>();
  dp_R = vector<vector<vector<int>>>(
      200000, vector<vector<int>>(10, vector<int>(2, INT_MAX)));
  dp_L = vector<vector<vector<int>>>(
      200000, vector<vector<int>>(10, vector<int>(2, INT_MAX)));
  dp_mid_R = vector<vector<vector<int>>>(
      200000, vector<vector<int>>(10, vector<int>(2, INT_MAX)));
  dp_mid_L = vector<vector<vector<int>>>(
      200000, vector<vector<int>>(10, vector<int>(2, INT_MAX)));

  for (int i = 0; i < Q; ++i) {
    if (queries[i][0] == 0) {
      cout << menaceQuery() << endl;
    } else {
      insertionQuery(queries[i][0], queries[i][1]);
    }
  }
}

void printOutput() { return; }

int main() {
  processInput();
  solve();
  printOutput();
}

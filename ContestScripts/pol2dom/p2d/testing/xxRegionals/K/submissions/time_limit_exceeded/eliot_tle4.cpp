#include <bits/stdc++.h>

std::vector<int64_t> left, right;
int Q, K;

int64_t access(int i) {
  if (i < left.size()) return left[left.size() - i - 1];
  return right[i - left.size()];
}

int64_t answer() {
  int64_t best = 0;
  int N = left.size() + right.size();
  if (N <= K) return 0;

  std::vector<int64_t> heap;
  for (int st = 0; st < N - K; ++st) {
    heap.clear();
    for (int i = st; i < st + K; ++i) heap.push_back(access(i));
    std::make_heap(heap.begin(), heap.end());

    int64_t sum = 0;
    for (int i = st + K; i < N; ++i) {
      heap.push_back(access(i));
      std::push_heap(heap.begin(), heap.end());
      sum += heap[0];
      std::pop_heap(heap.begin(), heap.end());
      heap.pop_back();
      best = std::max(best, sum);
    }
  }

  return best;
}

int main() {
  std::cin >> Q >> K;

  for (int i = 0; i < Q; i++) {
    char c;
    int64_t v;
    std::cin >> c;
    if (c == 'L') {
      std::cin >> v;
      left.push_back(v);
    } else if (c == 'R') {
      std::cin >> v;
      right.push_back(v);
    } else {
      std::cout << answer() << std::endl;
    }
  }
}

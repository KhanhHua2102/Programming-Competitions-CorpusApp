#include <bits/stdc++.h>

std::deque<int64_t> dq;
int Q, K;

int64_t answer() {
  int64_t best = 0;
  if (dq.size() <= K) return 0;

  std::vector<int64_t> heap;
  for (int st = 0; st < dq.size() - K; ++st) {
    heap.clear();
    // add the first K elements
    for (int i = st; i < st + K; ++i) heap.push_back(dq[i]);
    std::make_heap(heap.begin(), heap.end());

    int64_t sum = 0;
    for (int i = st + K; i < dq.size(); ++i) {
      heap.push_back(dq[i]);
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
      dq.push_front(v);
    } else if (c == 'R') {
      std::cin >> v;
      dq.push_back(v);
    } else {
      std::cout << answer() << std::endl;
    }
  }
}

#include <cmath>
#include <iostream>
#include <vector>

int quant_numb, quant_req;
std::vector<int> arr;
std::vector<int> block_zeros;
int zero_count = 0;
int block_size;

void build() {
  block_size = static_cast<int>(sqrt(quant_numb));
  block_zeros.assign((quant_numb + block_size - 1) / block_size, 0);

  for (int i = 0; i < quant_numb; ++i) {
    if (arr[i] == 0) {
      block_zeros[i / block_size]++;
      ++zero_count;
    }
  }
}

void update(int index, int new_value) {
  int block = index / block_size;

  if (arr[index] == 0) {
    block_zeros[block]--;
    zero_count--;
  }
  arr[index] = new_value;
  if (arr[index] == 0) {
    block_zeros[block]++;
    zero_count++;
  }
}

int get_index(int left_bound, int r, int k) {
  if (k > zero_count) {
    return -1;
  }

  int count_zeros = 0;
  int start_block = left_bound / block_size;
  int end_block = r / block_size;

  if (start_block == end_block) {
    for (int i = left_bound; i <= r; ++i) {
      if (arr[i] == 0) {
        count_zeros++;
        if (count_zeros == k) {
          return i + 1;
        }
      }
    }
  } else {
    for (int i = left_bound; i < (start_block + 1) * block_size && i <= r; ++i) {
      if (arr[i] == 0) {
        count_zeros++;
        if (count_zeros == k) {
          return i + 1;
        }
      }
    }

    for (int b = start_block + 1; b < end_block; ++b) {
      if (count_zeros + block_zeros[b] < k) {
        count_zeros += block_zeros[b];
      } else {
        for (int i = b * block_size; i < (b + 1) * block_size; ++i) {
          if (arr[i] == 0) {
            count_zeros++;
            if (count_zeros == k) {
              return i + 1;
            }
          }
        }
      }
    }

    for (int i = end_block * block_size; i <= r; ++i) {
      if (arr[i] == 0) {
        count_zeros++;
        if (count_zeros == k) {
          return i + 1;
        }
      }
    }
  }

  return -1;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::cin >> quant_numb;
  arr.resize(quant_numb);

  for (int i = 0; i < quant_numb; ++i) {
    std::cin >> arr[i];
  }

  build();

  std::cin >> quant_req;
  for (int i = 0; i < quant_req; ++i) {
    char op;
    std::cin >> op;
    if (op == 'u') {
      int index, new_value;
      std::cin >> index >> new_value;
      --index;
      update(index, new_value);
    } else if (op == 's') {
      int l, r, k;
      std::cin >> l >> r >> k;
      --l;
      --r;
      std::cout << get_index(l, r, k) << " ";
    }
  }

  return 0;
}

#include <cstdint>
#include <iostream>
#include <vector>

const int SizeOfX = 200001;
std::vector<uint8_t> x_bits(SizeOfX, 0);

int main() {
  size_t quant_req = 0;
  char operation = '0';
  size_t bit_index = 0;
  int ones_count = 0;

  std::cin >> quant_req;

  for (size_t i = 0; i < quant_req; ++i) {
    std::cin >> operation >> bit_index;

    if (bit_index >= SizeOfX) {
      std::cout << ones_count << std::endl;
      continue;
    }

    if (operation == '+') {
      while (bit_index < SizeOfX) {
        if (x_bits[bit_index] == 0) {
          x_bits[bit_index] = 1;
          ++ones_count;
          break;
        }
        x_bits[bit_index] = 0;
        --ones_count;
        ++bit_index;
      }
    } else if (operation == '-') {
      while (bit_index < SizeOfX) {
        if (x_bits[bit_index] == 1) {
          x_bits[bit_index] = 0;
          --ones_count;
          break;
        }
        x_bits[bit_index] = 1;
        ++ones_count;
        ++bit_index;
      }
    }
    std::cout << ones_count << std::endl;
  }
  return 0;
}
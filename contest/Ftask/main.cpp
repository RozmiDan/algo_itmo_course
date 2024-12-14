#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool main_algo(const std::vector<std::string>& words, const std::string& text) {
  size_t ptr_word = 0;
  size_t i = 0;
  while (i < text.size() && ptr_word < words.size()) {
    if (i + words[ptr_word].size() <= text.size() &&
        text.substr(i, words[ptr_word].size()) == words[ptr_word]) {
      i += words[ptr_word].size();
      ptr_word++;
      if (ptr_word < words.size()) {
        if (i >= text.size()) {
          return false;
        }
        i++;
      }
    } else {
      i++;
    }
  }
  while (i < text.size()) {
    i++;
  }
  return ptr_word == words.size();
}

int main() {
  size_t n_digit, m_digit;
  std::cin >> n_digit >> m_digit;
  std::string s_str;
  std::cin >> s_str;
  std::vector<std::string> words(m_digit);
  for (size_t i = 0; i < m_digit; ++i) {
    std::cin >> words[i];
  }

  if (main_algo(words, s_str)) {
    std::cout << "YES" << std::endl;
  } else {
    std::cout << "NO" << std::endl;
  }
  return 0;
}
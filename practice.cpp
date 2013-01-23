#include <iostream>
#include <functional>

void genInts(std::function<void(int)> handle) {
  for (int i = 0; i < 10; ++i)
    handle(i);
}

void printNum(const char* prefix, int i) {
  std::cout << prefix << " " << i << std::endl;
}

int main() {
  using namespace std::placeholders;
  genInts(std::bind(printNum, "The num is", _1));
	return 0;
}


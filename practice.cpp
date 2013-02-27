#include <iostream>
#include <functional>
#include <pthread.h>

void genInts(std::function<void(int)> handle) {
  for (int i = 0; i < 10; ++i)
    handle(i);
}

void printNum(const char* prefix, int i) {
  std::cout << prefix << " " << i << std::endl;
}

void* thr_func(void* args)
{
  usleep(5000000);
  long long n = reinterpret_cast<long long>(args);
  pthread_t tid = pthread_self();
  std::cout << "thread:" << n << " tid:" << tid << " run" << std::endl;
  return (void*) 0;
}

void pthreadTest()
{
  for (int i = 0; i < 200; ++i)
  {
    pthread_t tidp;
    int err = pthread_create(&tidp, NULL, thr_func, (void*)i);
    if (err)
      std::cout << "error:" << err << std::endl;
  }
}

void bindTest()
{
  using namespace std::placeholders;
  genInts(std::bind(printNum, "The num is", _1));
}

int main() {
  pthreadTest();
  usleep(10000000);
	return 0;
}


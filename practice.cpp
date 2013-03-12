#include <iostream>
#include <functional>
#include <vector>
#include <pthread.h>
#include "Utils.h"

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

void generatorTest()
{
  std::list<std::string> strList;
  strList.push_back("apple");
  strList.push_back("bed");
  strList.push_back("cat");
  strList.push_back("dog");
  strList.push_back("eat");
  strList.push_back("food");
  strList.push_back("god");
  strList.push_back("hello");
  strList.push_back("im");
  strList.push_back("jack");

  RandSeqGenerator<std::string> strGen;
  strGen.init(strList.begin(), strList.end(), std::string(""));
  printGenSeq(strGen, strList.size());

  std::cout << "************************************" << std::endl;

  RandSeqGenerator<std::string> strGen1;
  strGen1.init(strList.begin(), strList.end(), std::string(""));
  printGenSeq(strGen1, strList.size());
}

int main() {
  generatorTest();
	return 0;
}


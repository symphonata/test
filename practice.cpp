#include <iostream>
#include <functional>
#include <list>
#include <algorithm>
#include <pthread.h>

int randBetween(int min, int max)
{
  if (max == min)
    return min;
  unsigned int gap = abs(max - min);
  int ret = max > min ? min : max;
  ret += rand() % (gap + 1);
  return ret;
}

template<typename T>
class RandSeqGenerator
{
  private:
    std::list<T> source;
    std::list<T> rollQ;
    T zero;

  public:
    template<typename InputIt>
      void init(InputIt first, InputIt last, const T& zero)
      {
        this->source.clear();
        this->rollQ.clear();
        std::copy(first, last, std::back_inserter(source));
        this->zero = zero;
      }

    T next()
    {
      if (this->rollQ.empty())
      {
        if (this->source.empty())
          return this->zero;
        else
          std::copy(source.begin(), source.end(), std::back_inserter(rollQ));
      }

      int rdIdx = randBetween(0, rollQ.size() - 1);
      auto it = rollQ.begin();
      std::advance(it, rdIdx);
      T rs = (*it);
      rollQ.erase(it);
      return rs;
    }

    void clear()
    {
      this->source.clear();
      this->rollQ.clear();
    }
};

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
  strList.push_back("Monday");
  strList.push_back("Tuesday");
  strList.push_back("Wednesday");
  strList.push_back("Thursday");

  RandSeqGenerator<std::string> strGen;
  strGen.init(strList.begin(), strList.end(), std::string(""));
  for (int i = 1; i < 30; ++i)
  {
    std::cout << strGen.next() << std::endl;
    if (i % strList.size() == 0)
      std::cout << "----------------------" << std::endl;
  }

  std::list<int> intList;
  intList.push_back(1);
  intList.push_back(2);
  intList.push_back(3);
  intList.push_back(4);
  intList.push_back(5);

  RandSeqGenerator<int> intGen;
  intGen.init(intList.begin(), intList.end(), 0);
  for (int i = 1; i < 50; ++i)
  {
    std::cout << intGen.next() << std::endl;
    if (i % intList.size() == 0)
      std::cout << "----------------------" << std::endl;
  }
}

int main() {
  generatorTest();
	return 0;
}


#include <iostream>
#include <functional>
#include <list>
#include <vector>
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

template<typename T>
class RandSeqGenerator1
{
  private:
    std::vector<T> source;
    typename std::vector<T>::iterator iter;
    T zero;

    void rewind()
    {
      std::random_shuffle(this->source.begin(), this->source.end());
      this->iter = this->source.begin();
    }

  public:
    template<typename InputIt>
      void init(InputIt first, InputIt last, const T& zero)
      {
        this->source.clear();
        this->zero = zero;
        std::copy(first, last, std::back_inserter(this->source));
        rewind();
      }

    T next()
    {
      if (this->source.empty())
        return zero;
      else if (this->iter == this->source.end())
        rewind();

      return *this->iter++;
    }

    void clear()
    {
      this->source.clear();
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

template<typename Gen>
void printGenSeq(Gen& gen, int elemNum)
{
  for (int i = 1; i <= elemNum * 5; ++i)
  {
    std::cout << gen.next() << std::endl;
    if (i % elemNum  == 0)
      std::cout << "----------------------" << std::endl;
  }
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


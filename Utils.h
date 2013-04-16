#include <algorithm>
#include <list>
#include <vector>
#include <iostream>

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

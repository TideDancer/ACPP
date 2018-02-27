#ifndef _OURUTIL_H_
#define _OURUTIL_H_

#include <bitset>
#include <queue>
#include <list>
#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <iostream>
#include <math.h>
#include <time.h>
#include <random>
#include <queue>
#include <boost/dynamic_bitset.hpp>
#include <sparsehash/dense_hash_map>

typedef std::pair<unsigned char, long long> label_idx_pair;
typedef std::vector<std::list<long long>> vec_of_set;

template <typename T>
using value_idx_pair = std::pair<T, long long>;

template <typename T>
bool comparator ( const value_idx_pair<T>& l, const value_idx_pair<T>& r )
   { return l.first < r.first;  }

typedef std::pair<long long, long long> idx_pair;
typedef std::pair<idx_pair, double> tup;
class Comparator_tup
{
  public:
  bool operator() (const tup& lhs, const tup&rhs) const
	{
    return (lhs.second < rhs.second);
	}
};

double distance_our(double *x, double *y, int length , double best_so_far = INF )
{
    int i;
    double sum = 0;
    double bsf2 = best_so_far*best_so_far;
    for ( i = 0 ; i < length && sum < bsf2 ; i=i+4 )
    {
        sum += (x[i]-y[i])*(x[i]-y[i]);
        sum += (x[i+1]-y[i+1])*(x[i+1]-y[i+1]);
        sum += (x[i+2]-y[i+2])*(x[i+2]-y[i+2]);
        sum += (x[i+3]-y[i+3])*(x[i+3]-y[i+3]);
        //sum += (x[i+4]-y[i+4])*(x[i+4]-y[i+4]);
    }
    return sqrt(sum);
    }

int count_one(unsigned char n)
{
    int count=0;
    while(n!=0){
      n = n&(n-1);
      count++;
    }
    return count;
}

// int distance_bit(std::bitset<BIT_LEN*1024> x, std::bitset<BIT_LEN*1024> y )
// {
//     return (x^y).count();
// }

#endif 

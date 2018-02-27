// nlogn divide and conqure alg, not working
#ifndef _ACPP_D_H_
#define _ACPP_D_H_

#include "util.h"
#include "acpp_util.h"
//#include "mk_modified.h"

template <typename T>
class Comparator_vec1dim
{
  public:
  Comparator_vec1dim(int d){ dim = d; }
  bool operator() (const std::vector<T>& lhs, const std::vector<T>& rhs) const
	{
    return (lhs[dim] < rhs[dim]);
	}
  private:
  int dim;
};

template <typename T>
double Distance(const T& x, const T& y, int length , double best_so_far = INF )
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
   
void ACPP_D(double ** data, std::vector<std::vector<double> >& dataVec, int depth, int left, int right, double &currMinDist, long long &id_currMin_1, long long &id_currMin_2, long long &cnt )
{
    double d, t_beginOur, t;
    long long i , j, k , count;
    long long len = right-left + 1;
    //std::cout<<depth<<","<<left<<","<<right<<std::endl;

    // In first recursion, sort elements
		if(depth == 1){
        // clear dataVec
        for(i=0;i<TIMESERIES;i++)
            dataVec[i].clear();

        /*//
        // randomly generate projection matrix
        std::random_device gen;
        std::mt19937 generator(gen());
        std::normal_distribution<double> distribution(0.0,1.0);
        double* data_proj = (double*) malloc (sizeof(double)*TIMESERIES);
        double* project_matrix = (double*) malloc (sizeof(double)*LENGTH);
    		for(i=0;i<LENGTH;i++)
            project_matrix[i] = distribution(generator);
    		
        // project data to lower dimension
				for(i=0;i<TIMESERIES;i++){
            data_proj[i] = 0;
            for(k=0;k<LENGTH;k++)
                data_proj[i] += data[i][k]*project_matrix[k];
				}
        // sort
        value_idx_pair<double> projVec[TIMESERIES];
     		for(i=0;i<TIMESERIES;i++){
             projVec[i].first = data_proj[i];
             projVec[i].second = i;
     		}
        std::sort(projVec, projVec+TIMESERIES, comparator<double>);
        // put to dataVec
        for(i=0;i<TIMESERIES;i++){
            for(j=0;j<LENGTH;j++)
                dataVec[i].push_back(data[projVec[i].second][j]);
            dataVec[i].push_back(projVec[i].second);
		    }
        //
        */
 
        /* original code for randomly pick one dim */
        for(i=0;i<TIMESERIES;i++){
            for(j=0;j<LENGTH;j++)
                dataVec[i].push_back(data[i][j]);
            dataVec[i].push_back(i);
		    }
        srand ( time(NULL) );
        long long random_pick = rand() % LENGTH;
        //std::cout<<random_pick<<std::endl;
        std::sort(dataVec.begin(), dataVec.end(), Comparator_vec1dim<double>(random_pick));
       
    }

    // if N < 1000, brute-force
		if(len < 1000){
        for(i=left;i<right;i++)
					  for(j=i+1;j<right+1;j++){
                d = Distance< std::vector<double> >(dataVec[i], dataVec[j], LENGTH, currMinDist);
                cnt ++;
                if(d<currMinDist) 
                { currMinDist = d; id_currMin_1 = dataVec[i][LENGTH]; id_currMin_2 = dataVec[j][LENGTH]; }
						}
        return;
		}
		else{
        // recursively call same function
        ACPP_D(data, dataVec, depth+1, left, left+len/2, currMinDist, id_currMin_1, id_currMin_2, cnt);
        ACPP_D(data, dataVec, depth+1, left+len/2+1, right, currMinDist, id_currMin_1, id_currMin_2, cnt);
        
        // concentrate on mid index as boundary to target pairs
        std::vector<long long> pairs;
        int midIdx = left + len/2;
        // i = midIdx-1;
				// while(dataVec[midIdx][0] - dataVec[i][0] < currMinDist/sqrt(LENGTH) && i > left){
        //     pairs.insert(pairs.begin(),i); i--;
				// }
        // i = midIdx + 1;
				// while(dataVec[i][0] - dataVec[midIdx][0] < currMinDist/sqrt(LENGTH) && i < right){
        //     pairs.push_back(i); i++;
				// }
        // for(i=0;i<pairs.size()-1;i++)
        //     for(j=i+1;j<pairs.size();j++)
				// 		{
        //         d = Distance< std::vector<double> >(dataVec[i], dataVec[j], LENGTH, currMinDist);
        //         cnt ++;
        //         if(d<currMinDist) 
        //         { currMinDist = d; id_currMin_1 = dataVec[i][LENGTH]; id_currMin_2 = dataVec[j][LENGTH]; }
				// 		}

        const int RANGE = len/5;
        for(i=midIdx-RANGE; i<=midIdx; i++)
						for(j=midIdx+1;j<midIdx+RANGE;j++){
                d = Distance< std::vector<double> >(dataVec[i], dataVec[j], LENGTH, currMinDist);
                cnt ++;
                if(d<currMinDist) 
                { currMinDist = d; id_currMin_1 = dataVec[i][LENGTH]; id_currMin_2 = dataVec[j][LENGTH]; }
						}
        if(depth==1)
            std::cout<<currMinDist<<std::endl;
        return;
    }
}

#endif

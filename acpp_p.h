// this version use 2d-array as neighbor list, rather than use vec-of-list,
// to boost searching performance

#ifndef _ACPP_P_H_
#define _ACPP_P_H_

#include "util.h"
#include "acpp_util.h"
//#include "mk_modified.h"
   
double ACPP_P(double ** data, int REPEAT, int RANGE, double currMinDist )
{
    double d, t_beginOur, t;
    long long i , j, k , count;
 
    t_beginOur = clock();
    srand ( time(NULL) );
    long long random_pick = rand() % TIMESERIES;
    int cnt = 0;
    long long id_currMin_1 = -1; long long id_currMin_2 = -1;

    // repeat projection
    //const int REPEAT = 200;
    const int lower_dim = 1;
    //const int RANGE = 3;
    const long long QSIZE = RANGE*TIMESERIES;
    std::random_device gen;
    std::mt19937 generator(gen());
    std::normal_distribution<double> distribution(0.0,1.0);
    double* data_proj = (double*) malloc (sizeof(double)*TIMESERIES);
    double* project_matrix = (double*) malloc (sizeof(double)*LENGTH);
    value_idx_pair<double>* projVec = (value_idx_pair<double>*) malloc (sizeof(value_idx_pair<double>)*TIMESERIES);
    count = 0;
    //vec_of_set neighbors(TIMESERIES);
    std::vector<std::vector<long long> > neighbors(TIMESERIES, std::vector<long long>(RANGE*2*REPEAT, -1));
    std::vector<long long> nbrEnd(TIMESERIES, 0);

    double t1 = clock();
    double t4, t5, tt; tt = clock(); t4=t5=0;
    for(int rep = 0; rep < REPEAT; rep ++){
        // randomly generate projection matrix
    		for(i=0;i<LENGTH;i++){
          project_matrix[i] = distribution(generator);
    		}
        
        // project data to lower dimension
				for(i=0;i<TIMESERIES;i++){
          data_proj[i] = 0;
          for(k=0;k<LENGTH;k++)
            data_proj[i] += data[i][k]*project_matrix[k];
            //data_proj[i] += (project_matrix[k]>=0 ? data[i][k] : -data[i][k]);
				}
        t4 += (clock() - tt);
        tt = clock();
   
        // sort
     		for(i=0;i<TIMESERIES;i++){
             projVec[i].first = data_proj[i];
             projVec[i].second = i;
     		}
        std::sort(projVec, projVec+TIMESERIES, comparator<double>);
        //scan 
        for(i=0;i<TIMESERIES-RANGE;i++){
    		    for(j=i+1;j<i+RANGE+1;j++){
                if(projVec[i].second < projVec[j].second)
                    neighbors[projVec[i].second][nbrEnd[projVec[i].second]++] = projVec[j].second;
                else
                    neighbors[projVec[j].second][nbrEnd[projVec[j].second]++] = projVec[i].second;
				}}
        t5 += (clock() - tt);
        tt = clock();
    }
    //std::cout<<t4/CLOCKS_PER_SEC<<std::endl;
    //std::cout<<t5/CLOCKS_PER_SEC<<std::endl;
    double t2 = clock();
    //std::cout<<(t2-t1)/CLOCKS_PER_SEC<<std::endl;
    
    // search neighbors list
    std::vector<long long>::iterator it, jt;
	  for(i=0;i<TIMESERIES;i++){
      neighbors[i].resize(nbrEnd[i]);
      std::sort(neighbors[i].begin(), neighbors[i].end());
      it = neighbors[i].begin();
	  	while(it!=neighbors[i].end()){
        jt = it; cnt = 0;
        while(*jt == *it && jt != neighbors[i].end()){ jt ++; cnt ++; }
        if(cnt > 0)
				{
            d = distance_our(data[i], data[*it], LENGTH, currMinDist);
            count ++;
            if(d<currMinDist) 
            { currMinDist = d; id_currMin_1 = i; id_currMin_2 = *it; }
				}
        it = jt;
      }
    }
    double t3 = clock();
    //std::cout<<(t3-t2)/CLOCKS_PER_SEC<<std::endl;

    // print out result
    t = (clock() - t_beginOur)/CLOCKS_PER_SEC;
    //std::cout<<"final min distance = "<<currMinDist<<" for pair "<<id_currMin_1<<","<<id_currMin_2<<std::endl;
    //std::cout<<"total compare is "<<count<<std::endl;
    //std::cout<<"running time = "<<t<<" seconds"<<std::endl;
    return currMinDist;
}
#endif

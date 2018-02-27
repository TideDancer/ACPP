#include "util.h"
#include "acpp_util.h"
//#include "hashmat.h"
#include "mk.h"
#include "acpp_p.h"
#include "acpp_d.h"
  
int main(  int argc , char *argv[] )
{
    long long i , j, k ;
    long long length;
    double t, t_beginData, t_endData, t_beginOrig, t_endOrigin, t_beginOur, t_endOur;
    int verbos = 0;

    bsf = INF;
    signal(SIGINT,stop_exec);
    char* fileName = argv[1];
    TIMESERIES = atol(argv[2]);
    LENGTH = atoi(argv[3]);
    MAXREF = atoi(argv[4]);
    verbos = atoi(argv[5]);
    double threshold = atof(argv[6]);
    int FACTOR = atof(argv[7]); // h parameter, default = 2
    int RANGE = atoi(argv[8]); // default = 5

    int REPEAT = (TIMESERIES/10000)*FACTOR; // for divide and conqure
    //int REPEAT = (TIMESERIES/10000)*(TIMESERIES/10000)*FACTOR; //for random projection

    if( verbos == 1 )
        printf("\nNumber of Time Series : %lld\nLength of Each Time Series : %d\n\n",TIMESERIES,LENGTH);

    data = (double **)malloc(sizeof(double *)*TIMESERIES);
    ind = (long long *)malloc(sizeof(long long)*TIMESERIES);
    if( data == NULL || ind == NULL )
        error(1);
    data[0] = (double *)malloc(sizeof(double)*LENGTH);
    
    if( data[0] == NULL )
        error(1);

    t_beginData = clock();
    read_file(fileName, data);
    t_endData = clock();
    
    if(verbos == 1)
        printf("Data Have been Read and Normalized\n\n");
    std::cout<<"data preparation time is "<<(t_endData-t_beginData)/CLOCKS_PER_SEC<<std::endl;

    // --------------------------------------------------------------------------------------
    for(int k=0; k<10; k++){

    // random projection alg
    t_beginOur = clock();
    double bsf = INF; double result = INF;
    for(int i =0; i< REPEAT; i++)
    {
        result = ACPP_P(data, 100, RANGE, bsf);
        if(result < bsf)
            bsf = result;
    }
    t = (clock() - t_beginOur)/CLOCKS_PER_SEC;   
    std::cout<<"bsf="<<bsf<<std::endl;
    std::cout<<"time="<<t<<std::endl;
		
		// -------------------------------------------------------------------------------------

    MK(verbos, data);
    
    // ------------------------------------------------------------------------------------
    // // n log n divide and conqure alg
    // t_beginOur = clock();
    // double currMinDist = INF; long long id_currMin_1=-1; long long id_currMin_2=-1;
    // std::vector< std::vector<double> > dataVec(TIMESERIES); long long ct;
    // for(int i=0; i< REPEAT; i++){
    //   ACPP_D(data, dataVec,1,0,TIMESERIES-1, currMinDist, id_currMin_1, id_currMin_2, ct);
    // }
    // t = (clock() - t_beginOur)/CLOCKS_PER_SEC; 
    // std::cout<<"bsf="<<currMinDist<<std::endl;
    // std::cout<<"time="<<t<<std::endl;
    // ------------------------------------------------------------------------------------
    }
}

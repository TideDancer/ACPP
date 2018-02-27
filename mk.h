#ifndef _MK_H_
#define _MK_H_

void MK(int verbos, double ** data)
{
    double t, t_beginOrig, t_endOrigin;
    double *cntr, d;
    int *rInd;
    int abandon = 0 ,  r = 0;
    double ex , ex2,  std;
    int clear = 0;
    double count = 0;
    long long offset = 0, i = 0;

    // original code
    //////////////////////////////////////////////////////////////////////////

    t_beginOrig = clock();
    /*Generating the reference time series. Here it is a random time series*/

    dref = (double **)malloc(MAXREF*sizeof(double *));
    indices = (double **)malloc(MAXREF*sizeof(double *));
    stdRef = (double *)malloc(MAXREF*sizeof(double));
    cntr = (double *)malloc(MAXREF*sizeof(double));
    rInd = (int *)malloc(MAXREF*sizeof(int));

    // std::priority_queue<tup, std::vector<tup>, Comparator_tup> remainQ;
    // const int QSIZE = 20;
    // remainQ.push({{loc1, loc2}, bsf});

    if ( dref == NULL || indices == NULL || stdRef == NULL || cntr == NULL || rInd == NULL )
        error(1);

    srand ( time(NULL) );

    for( r = 0 ; r < MAXREF ; r++ )
    {
        dref[r] = (double *)malloc(sizeof(double)*LENGTH);
        indices[r] = (double *)malloc(sizeof(double)*TIMESERIES);
        
        if( dref[r] == NULL || indices[r] == NULL )
            error(1);

        long long random_pick = rand() % TIMESERIES;
        for( i = 0 ; i < LENGTH ; i++ )
            dref[r][i] = data[random_pick][i];

        if( verbos == 1 )
            printf("\nThe %lldth Time Series is chosen as %dth reference\n",random_pick,r);

        /*Creating the indices array which is a 2 by TIMESERIES
        sized vector having the indices (to the data array) and distances (from the
        reference time series) in each row.*/

        ex = 0;
        ex2 = 0;

        for( i = 0 ; i < TIMESERIES ; i++ )
        {
            if( i == random_pick )
            { indices[r][i] = INF; continue; }
            d = indices[r][i] = distance(data[i],dref[r],LENGTH);
            count = count + 1;
            ex += d;
            ex2 += d*d;
            if ( abs(i - random_pick ) <= clear )  continue;
            if ( d < bsf )
            {
                bsf = d; loc1 = i; loc2 = random_pick;
                if(verbos == 1)
                    printf("New best-so-far is %lf and (%lld , %lld) are the new motif pair\n",bsf,loc1,loc2);
            }

        }

        ex = ex/(TIMESERIES-1);
        ex2 = ex2/(TIMESERIES-1);
        std = sqrt(ex2-ex*ex);

        rInd[r] = r;
        stdRef[r] = std;
        cntr[r] = 0;
        ////////////////////////////////////////////////////////////////////
    }
  
    if(verbos == 1)
        printf("\nReferences are picked and Dist has been Computed\n\n");

    /*Sort the standard Deviations*/
    qsort(rInd,MAXREF,sizeof(int),comp2);

    ref = rInd[0];

    long long remaining = TIMESERIES;

    /*Sort indices using the distances*/
    qsort(ind,TIMESERIES,sizeof(long long),comp1);
    ///////////////////////////////////

    /*Motif Search loop of the algorithm that finds the motif. The algorithm
    computes the distances between a pair of time series only when it thinks
    them as a potential motif'*/

    if(verbos == 1)
        printf("Orderings have been Computed and Search has begun\n\n");
    offset = 0;
    abandon = 0;
    
    while (!abandon && offset < remaining)
    {
        abandon = 1;
        offset++;

        for(i = 0 ; i < remaining - offset ; i++ )
        {
            long long left = ind[i];
            long long right = ind[i + offset];
            if( abs(left-right) <= clear )
                continue;

            //According to triangular inequality distances between left and right
            //is obviously greater than lower_bound.
            double lower_bound = 0;
            r = 0;
            do
            {
                lower_bound = fabs(indices[rInd[r]][right] - indices[rInd[r]][left]);
                r++;
            }while( r < MAXREF && lower_bound < bsf );


            if (r >= MAXREF && lower_bound < bsf)
            {

                abandon = 0;
                count =  count + 1;
                d = distance( data[left] , data[right] , LENGTH , bsf );
                signal(SIGINT,SIG_IGN);

                // // use priority queue to store top values
                // if(remainQ.size() < QSIZE) 
                //     remainQ.push({{left, right}, d}); 
                // else if (d < remainQ.top().second ){
                //     remainQ.push({{left, right}, d}); 
                //     remainQ.pop(); 
                // }
                
                if (d < bsf )
                {
                    t_endOrigin = clock();
                    bsf = d;
                    loc1 = left;
                    loc2 = right;
                    if(verbos == 1)
                        printf("New best-so-far is %lf and (%lld , %lld) are the new motif pair\n",bsf,loc1,loc2);

                    }
                signal(SIGINT,stop_exec);
                }
            }
        }
        
    t_endOrigin = clock();
    printf("Final Motif is the pair ( %lld",loc1);
    printf(", %lld ) and the Motif distance is %lf\n",loc2,bsf);
    //if(verbos == 1)
    printf("Execution Time was : %lf seconds\n",(t_endOrigin-t_beginOrig)/CLOCKS_PER_SEC);

    // tup tup;
		// while(remainQ.size()>0){
    //     tup = remainQ.top(); remainQ.pop();
    //     std::cout<<tup.second<<" for "<<tup.first.first<<","<<tup.first.second<<std::endl;
		// }
}

#endif

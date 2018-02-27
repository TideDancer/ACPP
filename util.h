#ifndef _UTIL_H_
#define _UTIL_H_

#include <set>
#include <cmath>
#include <functional>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#define INF 999999999999.0

FILE *fp;
int ref;

double **data;
double **dref;
double **indices;
long long *ind;
long long loc1 , loc2;

double *stdRef;
long long TIMESERIES;
int LENGTH;
int MAXREF;
double bsf;

void error(int id)
{
    if(id==1)
        printf("ERROR : Memory can't be allocated!!!\n\n");
    else if ( id == 2 )
        printf("ERROR : File not Found!!!\n\n");
    else if ( id == 3 )
        printf("ERROR : Can't create Output File!!!\n\n");
    else if ( id == 4 )
        printf("ERROR : Invalid Number of Arguments!!!\n\n");
    else if ( id == 5 )
        printf("ERROR : Invalid Operands!!!\n\n");

    exit(1);
    }


void read_file(char* fileName, double** data){
    double d, ex , ex2 , mean, std;
    long long i, j;
    j = 0; i = 0; 
    ex = ex2 = 0;

    fp = fopen(fileName,"r");
    while(fscanf(fp,"%lf",&d) != EOF && i < TIMESERIES)
    {
        data[i][j] = d;
        ex += d;
        ex2 += d*d;
        if( j == LENGTH - 1 )
        {
            mean = ex = ex/LENGTH;
            ex2 = ex2/LENGTH;
            std = sqrt(ex2-ex*ex);
            // /*no normalization*/
            //for( int k = 0 ; k < LENGTH ; k++ )
            //    data[i][k] = (data[i][k]-mean)/std;
            ex = ex2 = 0;
            ind[i] = i;
            i++;
            if( i != TIMESERIES )
            {
                data[i] = (double *)malloc(sizeof(double)*LENGTH);
                if( data[i] == NULL )
                    error(1);
            }
            j = 0;
        }
        else
            j++;
    }
    fclose(fp);
}
   
/* Calculates the distance between two time series x and y. If the distance is
larger than the best so far (bsf) it stops computing and returns the approximate
distance. To get exact distance the bsf argument should be omitted.*/

double distance(double *x, double *y, int length , double best_so_far = INF )
{
    int i;
    double sum = 0;
    double bsf2 = best_so_far*best_so_far;
    for ( i = 0 ; i < length && sum < bsf2 ; i++ )
        sum += (x[i]-y[i])*(x[i]-y[i]);
    return sqrt(sum);
    }

/*Comparison function for qsort function. Compares two time series by using their
distances from the reference time series. */

int comp1(const void *a,const void *b)
{
    long long *x=(long long *)a;
    long long *y=(long long *)b;

    if (indices[ref][*x]>indices[ref][*y])
        return 1;
    else if (indices[ref][*x]<indices[ref][*y])
        return -1;
    else
        return 0;
    }


int comp2(const void *a,const void *b)
{
    int *x=(int *)a;
    int *y=(int *)b;

    if (stdRef[*x]<stdRef[*y])
        return 1;
    else if (stdRef[*x]>stdRef[*y])
        return -1;
    else
        return 0;
    }
    

void stop_exec(int sig)
{
    printf("Current Motif is (%lld",loc1);
    printf(",%lld)",loc2);
    printf(" and the distance is %lf\n",bsf);
    exit(1);
    }


#endif 

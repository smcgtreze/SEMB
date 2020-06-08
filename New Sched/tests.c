#include<stdio.h>
#include"Scheduler.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int responsetimeanalysis(int *C,int *T,int *D,int size, int n,bool block,int *B){

  double RWC1, RWC2=0;
  int i, j, k;

  for(i=0; i<size; i++){

    RWC1=0;
    RWC2=0;
    for(k=0; k<i+1; k++){
      RWC1 = RWC1+C[k];
    }
    //printf("RWC1a = %f\n", RWC1);
    while(RWC1 != RWC2){
        if(RWC2 != 0) RWC1 = RWC2;
      //  printf("RWC1b = %f\n", RWC1);
      //  printf("RWC2c = %f\n", RWC2);
        RWC2 = 0;
        for(j=0; j<i; j++){
          RWC2 = RWC2+ceil(RWC1/T[j])*C[j];
        //  printf("RWC2d = %f\n", RWC2);
        }
        if(block){
          RWC2 = RWC2 + C[i] + B[i];
        }
        else{
          RWC2 = RWC2 + C[i];
        }
        // printf("RWC2e = %f\n", RWC2);
        // printf("i = %d\n",i);
        if(RWC2 > D[i]){
          // printf("RTA: TASK SET %d NOT SCHEDULABLE\n", n);
          return 0;
        }
    }
  }
  if(RWC1 == RWC2){
    // printf("RTA: TASK SET %d IS SCHEDULABLE\n", n);
    return 1;
  }
}

int leastupperband (int *C,int *P,int t) {
  unsigned int i;
  double result=0.0, up;
  double *U = malloc(sizeof(double)*10*t);


  double length = t;

  up = length*(pow(2, 1/length)-1);
  // printf("up= %f\n", up);

  for(i=0; i < length; i++){
      U[i] = C[i]*1.0/P[i];
      result = result + U[i];
      //printf("result[%d] = %f\n", i, result);
  }

  //printf("result= %lf\n", result);

  if(result <= up ){
    return 1;
  }

  else if(result > up && result <= 1.0){
    return -1;
  }

  else{
    return 0;
  }
}

int cpudemand(int *C,int *T,int *D,int size, int n){

    double L1, L2,L0;
    int i, j, k, counter = 0, accumulator=0;

    // FILE* file;
    // file= fopen("cpugraph.csv","w");

    L1=0;
    L2=0;
    L0=0;
    for(k=0; k<size; k++){
        L1 = L1+C[k];
    }
    //printf("RWC1a = %f\n", RWC1);
    while(1){
        if(L2 != 0){
            L0 = L1;
            L1 = L2;
        }
        //  printf("RWC1b = %f\n", RWC1);
        //  printf("RWC2c = %f\n", RWC2);
        L2 = 0;
        // fprintf(file,"%d,%d",accumulator,counter);

      //  printf("\nAccumulator: %d", accumulator);

        counter ++;

        for(j=0; j<size; j++){
          L2 = L2+ceil(L1/T[j])*C[j];
          //printf("L2d = %f\n", L2);
        }

        if(abs(L0-L1) < abs(L2-L1)){
          return 0; //Tá a divergir
        }

        //printf("L2e = %f\n", L2);
        
        // if(L2 > D[size-1]){
        //   //printf("TASK SET NOT SCHEDULABLE\n");
        //   return 0;
        // }

        for(i=0;i<size;i++){
            if(counter % D[i]==0){
                accumulator += C[i]; 
            }
        }

        // printf("i = %d\n",i);
        if(accumulator > counter){
            //printf("CPU Demand: TASK SET %d NOT SCHEDULABLE\n", n);
            return 0;
        }

        if(L1 == L2){
            //printf("\nL1: %lf L2: %lf ", L1, L2);
            //printf("\nRTA: TASK SET %d IS SCHEDULABLE\n", n);
            if(counter >= L2){
              return 1;  
            }
        }
    }
    return 1;
}

int hyperbolic (int *C,int *T,double *u,int t) {

    unsigned int i=0;
    double result=1.0, calc=0.0,calc2=0.0;

    // for(i=0; i < t-1; i++){
    //     calc = (1.0*C[i]/T[i])+1.0;
    //     printf("calc[%d] = %f\n", i, calc);
    //     result = result * calc;
    //     printf("result[%d] = %f\n", i, result);
    // }

     for(i=0; i < t-1; i++){
        calc2 = (1.0*u[i])+1.0;
        //printf("calc2[%d] = %f\n", i, calc2);
        result = result * calc2;
        //printf("result[%d] = %f\n", i, result);
    }

    //printf("result= %f\n", result);

    if(result <=2){
        //printf("Task Set is schedulable\n");
        return 1;
    }

    else{
        //printf("Task Set NOT schedulable\n");
        return 0;
    }
}

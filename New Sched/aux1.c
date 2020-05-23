#include <stdio.h>
#include "Scheduler.h"
#include "tests.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
double avg=0.0;
int count=0;

double average(double *vetor,int size){
	double soma=0.0;
	for(int i=0;i < size;i++){
		soma += vetor[i];
    // printf("Vetor[%d] =%lf\n",i,vetor[i]);
    // printf("Soma:%lf\n",soma);
	}
	return (soma/size);
}

double average_int(int *vetor,int size){
	double soma=0.0;
	for(int i=0;i < size;i++){
		soma += vetor[i];
    // printf("Vetor[%d] =%lf\n",i,vetor[i]);
    // printf("Soma:%lf\n",soma);
	}
	return (soma/size);
}

void Func_init(int task){
    clock_t before = clock();
    for(int i=0;i < 10*CLOCKS_PER_SEC;i++);
    clock_t difference = clock() - before;
    printf("Task %d:execution time:%ld ms\n",task,difference*1000/CLOCKS_PER_SEC);
    avg += difference*1000/CLOCKS_PER_SEC;
    count++;
}


int cmpfunc (const void * a, const void * b) {
   const double *A = a, *B = b;
   return (*A > *B) - (*A < *B);
}

int cmpfunc_int (const void * a, const void * b) {
   const int *A = a, *B = b;
   return (*A > *B) - (*A < *B);
}


void reorder(int *a, int *b, int n) 
{ 
  for (int i = 0; i < n; i++)                     //Loop for ascending ordering
    {
      for (int j = 0; j < n; j++)             //Loop for comparing other values
      {
        if (a[j] > a[i])                //Comparing other array elements
        {
          int tmp = a[i];
          int tmp2 = b[i];      //Using temporary variable for storing last value
          a[i] = a[j];
          b[i] = b[j];                        //replacing value
          a[j] = tmp;
          b[j] = tmp2;              //storing last value
        }  
      }
    }
}

int save(char *filename,double ratio,int size,int t,double UT,char var[]){
  FILE* file;
  file= fopen(filename,"a+");

  if(file == NULL){
    printf("Erro a abrir o ficheiro\n");
  }

  if( (ratio >= 0.0) && (strcmp(var,"UT") == 0) ){
    fprintf(file, "%d,%lf\n",t,ratio);
  }

  if( (ratio >= 0.0) && (strcmp(var,"TSS") == 0) ){
    fprintf(file, "%.2lf,%lf\n",UT,ratio);
  }
  
}

int save_hist(char *filename,int *vec,int size){
  FILE* file;
  file= fopen(filename,"w");

  if(file == NULL){
    printf("Erro a abrir o ficheiro\n");
  }

  if(vec != NULL){
    for(int i=0;i < size;i++){
      fprintf(file, "%d,%lf \n",vec[i], (double)(i*ISize));
    }
  }
}
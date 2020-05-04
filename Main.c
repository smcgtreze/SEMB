#include<stdio.h>
#include"Scheduler.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
float avg=0.0;
int count=0;

int cmpfunc (const void * a, const void * b) {
   const double *A = a, *B = b;
   return (*A > *B) - (*A < *B);
}

int cmpfunc_int (const void * a, const void * b) {
   const int *A = a, *B = b;
   return (*A > *B) - (*A < *B);
}

int save(int *histograma,int size){
  FILE* file;
  file= fopen("call.csv","w+");

  if(file == NULL){
    printf("Erro a abrir o ficheiro\n");
  }

  for (int j = 0; j < size; j++)
  {
      fprintf(file, "%d, %lf, %d\n", j, (2*j+1)/(float)(size*2), histograma[j]);
      //printf("%d, %lf, %d\n", j, (2*j+1)/(float)(size*2), histograma[j]);
  }
}

int leastupperband (double *U) {
  unsigned int i;
  double result=0.0, up;

  double length = TaskSetSize;
//   printf("Length %lf\n",length);

  up = length*(pow(2, 1/length)-1);
//   printf("up= %f\n", up);

  for(i=0; i < length; i++){
      result = result + U[i];
    //  printf("result[%d] = %f\n", i, result);
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


int hyperbolic (int *C,int *T,double *u) {

    unsigned int i=0;
    double result=1.0, calc=0.0,calc2=0.0;

    // for(i=0; i < TaskSetSize-1; i++){
    //     calc = (1.0*C[i]/T[i])+1.0;
    //     printf("calc[%d] = %f\n", i, calc);
    //     result = result * calc;
    //     printf("result[%d] = %f\n", i, result);
    // }

     for(i=0; i < TaskSetSize-1; i++){
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

void Func_init(int task){
    clock_t before = clock();
    for(int i=0;i < 10*CLOCKS_PER_SEC;i++);
    clock_t difference = clock() - before;
    printf("Task %d:execution time:%ld ms\n",task,difference*1000/CLOCKS_PER_SEC);
    avg += difference*1000/CLOCKS_PER_SEC;
    count++;
}

void average(){
    printf("Average time of execution :%.2f ms\n",avg/count);        
}

int main (int argc, char *argv[]){
    Sched_Init();
    int *period = malloc(sizeof(int)*10*TaskSetSize),*C=malloc(sizeof(int)*10*TaskSetSize),*deadline = malloc(sizeof(int)*10*TaskSetSize);
    double *util = malloc(sizeof(double)*10*TaskSetSize);
    int positive =0, positive2 =0;
    srand((unsigned)(time(NULL)));

    for(int j=0;j < Nsets;j++){
        TaskSet_New(j);
        //Rate monotonic
        // for(int i=0;i < TaskSetSize;i++){
        //     period[i]= 5001 + rand()%(5000); // de 5001 a 10000
        //     C[i]= 1 + rand()%(5000); // de 1 a 5000
        // }
        // qsort(period,TaskSetSize,sizeof(double),cmpfunc);

        // for(int i=0;i < TaskSetSize;i++){
        //     util[i] =(double) C[i]*1.0/period[i];
        //     //printf("Utilização %lf\n",util[i]);
        //     Sched_AddT(Func_init, period[i], period[i],i,C[i]);
        //     TaskSet_Add(i,j,Tasks[i]);
        // }

        //Deadline monotonic
        for(int i=0;i < TaskSetSize;i++){
            deadline[i]= 10001 + rand()%(5000); // de 10001 a 15000
            period[i]= 5001 + rand()%(5000); // de 5001 a 10000
            C[i]= 1 + rand()%(5000); // de 1 a 5000
        }

        qsort(deadline,TaskSetSize,sizeof(int),cmpfunc_int);

        for(int i=0;i < TaskSetSize;i++){
            util[i] =(double) C[i]*1.0/period[i];
            Sched_AddT(Func_init,deadline[i],period[i],i,C[i]);
            TaskSet_Add(i,j,Tasks[i]);
        }

        qsort(util,TaskSetSize,sizeof(double),cmpfunc);

        for(int z=0;z < TaskSetSize-1;z++){
            util[z] = util[z+1] - util[z]; 
        }
        // printf("Task set id:%d\n",set[j].task[2].id);

        //HYPERBOLIC BOUND
        if(hyperbolic(C,period,util) == 1){
            printf("\nHyperbolic :Task Set %d is schedulable\n",j);
            positive++;
        }

        else if(hyperbolic(C,period,util) == 0){
            printf("\nHyperbolic :Task Set %d is NOT schedulable\n",j);
        }

        //LEAST UPPER BOUND
        if(leastupperband (util) == 1){
            printf("LUB :Task Set %d is schedulable\n",j);
            positive2++;
        }

        else if(leastupperband (util) == 0){
            printf("LUB :Task Set %d is NOT schedulable\n",j);
        }

        else{
            printf("LUB :Task Set %d is indetermined\n",j);
        }
    }

    printf("\nResultados dos testes \n");
    printf("Hyperbolic Condition :%.4f dos task sets são escalonáveis\n",((float)1.0*positive/Nsets));
    printf("LUB Condition :%.4f dos task sets são escalonáveis\n",((float)1.0*positive2/Nsets));

    // while (1) {
    //      Sched_Schedule();
    //      Sched_Dispatch();
    // }
}


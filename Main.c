#include<stdio.h>
#include"Scheduler.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

double avg=0.0;
int count=0;


int cmpfunc (const void * a, const void * b) {
   const double *A = a, *B = b;
   return (*A > *B) - (*A < *B);
}

int cmpfunc_int (const void * a, const void * b) {
   const int *A = a, *B = b;
   return (*A > *B) - (*A < *B);
}

int save(char *filename,double ratio,double *vec,int size){
  FILE* file;
  file= fopen(filename,"a+");

  if(file == NULL){
    printf("Erro a abrir o ficheiro\n");
  }
  if(ratio != 0.0){
    fprintf(file, "%d,%lf\n",TaskSetSize,ratio);
  }

  if(vec != NULL){
    for(int i=0;i < size;i++){
      fprintf(file, "%lf\n",vec[i]);
    }
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
      fprintf(file, "%d,%.3lf\n",vec[i], (double)i*ISize);
    }
  }
}

int responsetimeanalysis(int *C,int *T,int *D,int size, int n){

  float RWC1, RWC2=0;
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
        RWC2 = RWC2 + C[i];
      //  printf("RWC2e = %f\n", RWC2);
      //  printf("i = %d\n",i);
        if(RWC2 > D[i]){
          printf("RTA: TASK SET %d NOT SCHEDULABLE\n", n);
          return 0;
        }
    }
  }
  if(RWC1 == RWC2){
    printf("RTA: TASK SET %d IS SCHEDULABLE\n", n);
    return 1;
  }
}

int leastupperband (int *C,int *P) {
  unsigned int i;
  double result=0.0, up;
  double *U = malloc(sizeof(double)*10*TaskSetSize);


  double length = TaskSetSize;
//   printf("Length %lf\n",length);

  up = length*(pow(2, 1/length)-1);
//   printf("up= %f\n", up);

  for(i=0; i < length; i++){
      U[i] = C[i]*1.0/P[i];
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

double average(double *vetor,int size){
	double soma=0.0;
	for(int i=0;i < size;i++){
		soma += vetor[i];
    // printf("Vetor[%d] =%lf\n",i,vetor[i]);
    // printf("Soma:%lf\n",soma);
	}
	// printf("Soma:%lf\n",soma);
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

int main (int argc, char *argv[]){
    Sched_Init();
    int *histograma=malloc(sizeof(int)*(50));
    int *period = malloc(sizeof(int)*10*TaskSetSize),*C=malloc(sizeof(int)*10*TaskSetSize),*deadline = malloc(sizeof(int)*10*TaskSetSize);
    double *util = malloc(sizeof(double)*10*TaskSetSize);
    double *aux = malloc(sizeof(double)*10*Nsets);
    double sum=0.0;
    int positive =0, positive2 =0,indetermined = 0, counter=0, rta_counter= 0;
    srand((unsigned)(time(NULL)));
    memset(histograma,0,0);
    memset(aux,0,0);
    char *str= argv[1];

    for(int j=0;j < Nsets;j++){
        TaskSet_New(j);
        //Rate monotonic
        if(strcmp(str,"rm") == 0){
          for(int i=0;i < TaskSetSize;i++){
              period[i]= 5001 + rand()%(5000); // de 5001 a 10000
              C[i]= 1 + rand()%(5000); // de 1 a 5000
          }
          qsort(period,TaskSetSize,sizeof(int),cmpfunc);
          sum=0.0;

          for(int i=0;i < TaskSetSize;i++){
              util[i] =(double) C[i]*1.0/period[i];
              // sum+=util[i];

              // if((sum < UT*TaskSetSize) && (i == (TaskSetSize-1))){
              //   sum=sum-util[i];
              //   util[i] = (UT*TaskSetSize - sum);
              //   sum+=util[i];
              // }

              // else if((sum > UT*TaskSetSize) && (i <= (TaskSetSize-1))){
              //   sum=sum-util[i];
              //   util[i] = (UT*TaskSetSize - sum)/(TaskSetSize - i);
              //   sum+=util[i];
                
              //   while(i < TaskSetSize -1){
              //     ++i;
              //     util[i]=util[i-1];
              //     // printf("Utilização %lf\n",util[i]);
              //     sum+=util[i];
              //     period[i]= 5001 + rand()%(5000); // de 5001 a 10000
              //     C[i]= util[i]*period[i];
              //   }         
              // }
              // Sched_AddT(Func_init, period[i], period[i],i,C[i]);
              // TaskSet_Add(i,j,Tasks[i]);
          }
          // printf("Soma das utilizações depois %lf\n",sum);
        }
        

        //Deadline monotonic
        if(strcmp(str,"dm") == 0){
          for(int i=0;i < TaskSetSize;i++){
              deadline[i]= 10001 + rand()%(5000); // de 10001 a 15000
              period[i]= 5001 + rand()%(5000); // de 5001 a 10000
              C[i]= 1 + rand()%(5000); // de 1 a 5000
          }

          qsort(deadline,TaskSetSize,sizeof(int),cmpfunc);

          for(int i=0;i < TaskSetSize;i++){
              util[i] =(double) C[i]*1.0/period[i];
              Sched_AddT(Func_init,deadline[i],period[i],i,C[i]);
              TaskSet_Add(i,j,Tasks[i]);
          }
        }

        qsort(util,TaskSetSize,sizeof(double),cmpfunc);

        for(int z=0;z < TaskSetSize-1;z++){
            util[z] = util[z+1] - util[z]; 
        }
        
        sum=0.0;
        for(int i=0;i < TaskSetSize;i++){
              sum+=util[i];

              if((sum < UT*TaskSetSize) && (i == (TaskSetSize-1))){
                sum=sum-util[i];
                util[i] = (UT*TaskSetSize - sum);
                sum+=util[i];
                period[i]= 5001 + rand()%(5000); // de 5001 a 10000
                C[i]= util[i]*period[i];
              }

              else if((sum > UT*TaskSetSize) && (i <= (TaskSetSize-1))){
                sum=sum-util[i];
                util[i] = (UT*TaskSetSize - sum)/(TaskSetSize - i);
                sum+=util[i];
                
                while(i < TaskSetSize -1){
                  ++i;
                  util[i]=util[i-1];
                  // printf("Utilização %lf\n",util[i]);
                  sum+=util[i];
                  period[i]= 5001 + rand()%(5000); // de 5001 a 10000
                  C[i]= util[i]*period[i];
                }         
              }
              else{
                  period[i]= 5001 + rand()%(5000); // de 5001 a 10000
                  C[i]= util[i]*period[i];
              }
              Sched_AddT(Func_init, period[i], period[i],i,C[i]);
              TaskSet_Add(i,j,Tasks[i]);
        }

        //HYPERBOLIC BOUND (rm Utilization)
         if(strcmp(str,"rm") == 0){
          if(hyperbolic(C,period,util) == 1){
              printf("\nHyperbolic :Task Set %d is schedulable\n",j);
              positive++;
          }

          else if(hyperbolic(C,period,util) == 0){
              printf("\nHyperbolic :Task Set %d is NOT schedulable\n",j);
          }
        }

        //LEAST UPPER BOUND (rm & dm Utilization)
        if(strcmp(str,"rm") == 0){
          if(leastupperband (C,period) == 1){
              printf("LUB :Task Set %d is schedulable\n",j);
              positive2++;
          }

          else if(leastupperband (C,period) == 0){
              printf("LUB :Task Set %d is NOT schedulable\n",j);
          }

          else{
              printf("LUB :Task Set %d is indetermined\n",j);
              ++indetermined;
          }
        }

        if(strcmp(str,"dm") == 0){
          if(leastupperband (C,deadline) == 1){
              printf("LUB :Task Set %d is schedulable\n",j);
              positive2++;
          }

          else if(leastupperband (C,deadline) == 0){
              printf("LUB :Task Set %d is NOT schedulable\n",j);
          }

          else{
              printf("LUB :Task Set %d is indetermined\n",j);
              ++indetermined;
          }
        }

        //RESPONSE TIME ANALYSIS
        if(strcmp(str,"rm") == 0)
          if(responsetimeanalysis(C, period, deadline, TaskSetSize, j)==1)
            rta_counter++;

        if(strcmp(str,"dm") == 0){
          for (int i=0; i<TaskSetSize; i++)
          {
            if(deadline[i]>=period[i])
            {
              counter++;
            }
          }

          if(counter == TaskSetSize){
            counter = 0;
            if(responsetimeanalysis(C, period, deadline, TaskSetSize, j)==1)
              rta_counter++;
          }

          else
            counter = 0;
        }

    //save("Util_avg.csv",0,util,TaskSetSize);
    //Calculation of the distribution of the utilizations
    for (int j = 0; j < TaskSetSize;j++){
      for (int n = 0;n < Bars;n++){
        if((util[j] >= n*(ISize)) && (util[j] < (n+1)*ISize)){
          histograma[n]++;
        }
      }
    }

    //Average of each task set
    set[j].avgUtil = average(util,TaskSetSize);
    printf("Utilization Average :%.4f\n",set[j].avgUtil);
    // printf("Period Average :%.4f\n",average((double*)period,TaskSetSize));
    // printf("Deadline Average :%.4f\n",average((double*)deadline,TaskSetSize));
  }

    for (int j = 0; j < Nsets;j++){
      aux[j] = (double) set[j].avgUtil;
    }

    if(strcmp(str,"dm") == 0){
      printf("\nDeadline Monotonic Scheduling ");
    }

    if(strcmp(str,"rm") == 0){
      printf("\nRate Monotonic Scheduling ");
    }

    printf("\nResultados dos testes \n");
    printf("Hyperbolic Condition :%.4f dos task sets são escalonáveis\n",((double)1.0*positive/Nsets));
    printf("LUB Condition :%.4f dos task sets são escalonáveis\n",((double)1.0*positive2/Nsets));
    printf("LUB Condition :%.4f dos task sets são indeterminados\n",((double)1.0*indetermined/Nsets));
    printf("RTA Condition :%.4f dos task sets são escalonáveis\n",((double)1.0*rta_counter/Nsets));
    printf("\nUsing All the tasks in the Set\n");
    printf("Number of tasks per Set :%d\n",TaskSetSize);
    printf("Number of Sets :%d\n",Nsets);
    printf("Total Average Utilization :%.4lf\n",average(aux,Nsets));
    // printf("Period Average :%.4f\n",average(period,TaskSetSize*Nsets));
    // printf("Deadline Average :%.4f\n",average(deadline,TaskSetSize*Nsets));

    // save("Hyperbolic.csv",((double)1.0*positive/Nsets),NULL,0);
    // save("LUB.csv",(double)(1.0*positive2/Nsets),NULL,0);
    save_hist("Dist_Util.csv",histograma,Bars);
    // while (1) {
    //      Sched_Schedule();
    //      Sched_Dispatch();
    // }

    free(histograma);
    free(aux);
}


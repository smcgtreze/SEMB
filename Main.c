#include <stdio.h>
#include "Scheduler.h"
#include "tests.h"
#include "PIP.h"
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

void save(char *filename,double ratio,double *vec,int size){
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

void save_hist(char *filename,int *vec,int size){
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

int main (int argc, char *argv[]){
    Sched_Init();
    int *histograma=malloc(sizeof(int)*(50));
    int *period = malloc(sizeof(int)*10*TaskSetSize),*C=malloc(sizeof(int)*10*TaskSetSize),*deadline = malloc(sizeof(int)*10*TaskSetSize);
    double *util = malloc(sizeof(double)*10*TaskSetSize);
    double *aux = malloc(sizeof(double)*10*Nsets);
    double sum=0.0;
    int positive =0, positive2 =0,indetermined = 0, counter=0, rta_counter= 0, rows, n_sem;
    int *B;
    int RandSize = UT*10000;
    srand((unsigned)(time(NULL)));
    memset(histograma,0,0);
    memset(aux,0,0);
    char *str= argv[1];

    for(int j=0;j < Nsets;j++){
        TaskSet_New(j);
        //Rate monotonic
        if(strcmp(str,"rm") == 0){
          for(int i=0;i < TaskSetSize;i++){
              period[i] = 2*(1+rand()%(MAXPERIOD)); // Ti evenly divides Ti+1 
              C[i]= rand()%(period[i]) ; // the computation time C i uniform in [0,Ti ]
          }
          qsort(period,TaskSetSize,sizeof(int),cmpfunc_int);

          for(int i=0;i < TaskSetSize;i++){
              util[i] =(double) C[i]*1.0/period[i];
          }
        }
        

        //Deadline monotonic
        if(strcmp(str,"dm") == 0){
          for(int i=0;i < TaskSetSize;i++){
              period[i]= 2*(1+rand()%(MAXPERIOD)); 
              deadline[i]= rand()%(period[i]); // D > P
              C[i]= rand()%(period[i]); 
          }

          qsort(deadline,TaskSetSize,sizeof(int),cmpfunc_int);

          for(int i=0;i < TaskSetSize;i++){
              util[i] =(double) C[i]*1.0/period[i];
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
                period[i]= 2*(1+rand()%(MAXPERIOD)); 
                if(strcmp(str,"dm") == 0){
                  deadline[i]= rand()%(period[i]); // D > P
                }
              }

              else if((sum > UT*TaskSetSize) && (i <= (TaskSetSize-1))){
                sum=sum-util[i];
                util[i] = (UT*TaskSetSize - sum)/(TaskSetSize - i);
                sum+=util[i];
                period[i]= 2*(1+rand()%(MAXPERIOD));
                if(strcmp(str,"dm") == 0){
                  deadline[i]= rand()%(period[i]); // D > P
                } 
                
                while(i < TaskSetSize -1){
                  ++i;
                  util[i]=util[i-1];
                  sum+=util[i];
                  period[i]= 2*(1+rand()%(MAXPERIOD));
                  if(strcmp(str,"dm") == 0){
                    deadline[i]= rand()%(period[i]); // D > P
                  } 
                }         
              }
              else{
                  period[i]= 1+ 2*(rand()%(MAXPERIOD));
                  if(strcmp(str,"dm") == 0){
                    deadline[i]= rand()%(period[i]); // D > P
                  } 
              }
        }
      
      if(strcmp(str,"rm") == 0)
      {qsort(period,TaskSetSize,sizeof(int),cmpfunc_int);}
      if(strcmp(str,"dm") == 0){
        //qsort(deadline,TaskSetSize,sizeof(int),cmpfunc_int);
        reorder(deadline,period,TaskSetSize);
      } 

      for(int i=0;i < TaskSetSize;i++){
            C[i]= util[i]*period[i];
            if(strcmp(str,"rm") == 0){
              deadline[i]=period[i];
            }
            Sched_AddT(Func_init, deadline[i], period[i],i,C[i]);
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
        if(strcmp(str,"rm") == 0){
          if(responsetimeanalysis(C, period, deadline, TaskSetSize, j)==1)
            rta_counter++;
        }
        if(strcmp(str,"dm") == 0){
          for (int i=0; i<TaskSetSize; i++)
          {
            if(deadline[i]<=period[i])
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

        rows= TaskSetSize;

        n_sem = rand() % (rows-1) + 1;


        B = (int *)malloc(rows * sizeof(int));



        printf("\n\n------------------------------------\n");

        B = calc_b(period, rows, n_sem);

    //Calculation of the distribution of the utilizations
    for (int z = 0; z < TaskSetSize;z++){
        printf("\nSet %d/%d\n",j,Nsets-1);
        printf("Task %d/%d\n",z,TaskSetSize-1);
        printf("Period %d\n",period[z]);
        printf("Execution time %d\n",C[z]);
        printf("Deadline %d\n",deadline[z]);
        printf("Utilization %lf\n",util[z]);
        printf("B %d\n",B[z]);
      for (int n = 0;n < Bars;n++){
        if((util[z] >= n*(ISize)) && (util[z] < (n+1)*ISize)){
          histograma[n]++;
        }
      }
    }

    //Average of each task set
    set[j].avgUtil = average(util,TaskSetSize);
    // printf("Utilization Average :%.4f\n",set[j].avgUtil);
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
    // save("RTA.csv",(double)(1.0*rta_counter/Nsets),NULL,0);
    save_hist("Dist_Util.csv",histograma,Bars);

    // while (1) {
    //      Sched_Schedule();
    //      Sched_Dispatch();
    // }

    free(histograma);
    free(C);
    free(period);
    free(deadline);
    free(util);
    free(aux);
}
#include <stdio.h>
#include "Scheduler.h"
#include "aux1.h"
#include "tests.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char *argv[]){
  char var[20];
  double UT_aux;
  int TSS=0;
  printf("Escolha uma variável para manter constante UT ou TSS (Task Set Size) \n");
  scanf("%s",&var);
  if(strcmp(var,"UT") == 0){
    printf("Escolha um valor para UT \n");
    scanf("%lf",&UT_aux);
    UT_MIN=UT_aux; // Limite inferior da Utilização média de cada task set
    UT_MAX=UT_aux; // Limite superior da Utilização média de cada task set
    printf("Escolha o valor para TSS max \n");
    scanf("%d",&TSS);
    TaskSetSize=TSS; // Limite superior do Task Set Size
    printf("Escolha o valor para TSS min \n");
    scanf("%d",&TSS);
    SetSizeMin =TSS; // Limite inferior do Task Set Size
  }
  if(strcmp(var,"TSS") == 0){
    printf("Escolha o valor para UT max \n");
    scanf("%lf",&UT_aux);
    UT_MAX=UT_aux; // Limite superior da Utilização média de cada task set
    printf("Escolha o valor para UT min \n");
    scanf("%lf",&UT_aux);
    UT_MIN=UT_aux; // Limite inferior da Utilização média de cada task set
    printf("Escolha um valor para TSS \n");
    scanf("%d",&TSS);
    TaskSetSize=TSS; // Limite superior do Task Set Size
    SetSizeMin =TSS; // Limite inferior do Task Set Size
  }
  // TaskSetSize=5; // Limite superior do Task Set Size
  // SetSizeMin =5; // Limite inferior do Task Set Size
  // UT_MIN=0.05; // Limite inferior da Utilização média de cada task set
  // UT_MAX=0.50; // Limite superior da Utilização média de cada task set
  remove("Hyperbolic.csv"); //remover os ficheiros resultantes da última execução
  remove("LUB.csv");
  remove("RTA.csv");   

  for(double UT=UT_MIN;UT <= UT_MAX;UT+= UT_int){
  for(int t=SetSizeMin;t < TaskSetSize+1;t++){
    Sched_Init(t);
    int *histograma=malloc(sizeof(int)*(50));
    int *period = malloc(sizeof(int)*10*t),*C=malloc(sizeof(int)*10*t),*deadline = malloc(sizeof(int)*10*t);
    double *util = malloc(sizeof(double)*10*t);
    double *aux = malloc(sizeof(double)*10*Nsets);
    double sum=0.0;
    int positive =0, positive2 =0,indetermined = 0, counter=0, rta_counter= 0;
    int RandSize = UT*10000;
    srand((unsigned)(time(NULL)));
    memset(histograma,0,0);
    memset(aux,0,0);
    memset(C,0,0);
    memset(deadline,0,0);
    memset(util,0,0);
    memset(period,0,0);
    char *str= argv[1];

    for(int j=0;j < Nsets;j++){
        TaskSet_New(j);
        //Rate monotonic
        if(strcmp(str,"rm") == 0){
          for(int i=0;i < t;i++){
              period[i] = 2*(1+rand()%(MAXPERIOD)); // Ti evenly divides Ti+1 
              C[i]= rand()%(period[i]) ; // the computation time C i uniform in [0,Ti ]
          }
          qsort(period,t,sizeof(int),cmpfunc_int);

          for(int i=0;i < t;i++){
              util[i] =(double) C[i]*1.0/period[i];
          }
        }
        

        //Deadline monotonic
        if(strcmp(str,"dm") == 0){
          for(int i=0;i < t;i++){
              period[i]= 2*(1+rand()%(MAXPERIOD)); 
              deadline[i]= rand()%(period[i]); // D > P
              C[i]= rand()%(period[i]); 
          }

          qsort(deadline,t,sizeof(int),cmpfunc_int);

          for(int i=0;i < t;i++){
              util[i] =(double) C[i]*1.0/period[i];
          }
        }

        qsort(util,t,sizeof(double),cmpfunc);

        for(int z=0;z < t-1;z++){
            util[z] = util[z+1] - util[z]; 
        }
        
        sum=0.0;
        for(int i=0;i < t;i++){
              sum+=util[i];

              if((sum < UT*t) && (i == (t-1))){
                sum=sum-util[i];
                util[i] = (UT*t - sum);
                sum+=util[i];
                period[i]= 2*(1+rand()%(MAXPERIOD)); 
                if(strcmp(str,"dm") == 0){
                  deadline[i]= rand()%(period[i]); // D > P
                }
              }

              else if((sum > UT*t) && (i <= (t-1))){
                sum=sum-util[i];
                util[i] = (UT*t - sum)/(t - i);
                sum+=util[i];
                period[i]= 2*(1+rand()%(MAXPERIOD));
                if(strcmp(str,"dm") == 0){
                  deadline[i]= rand()%(period[i]); // D > P
                } 
                
                while(i < t -1){
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
      {qsort(period,t,sizeof(int),cmpfunc_int);}
      if(strcmp(str,"dm") == 0){
        //qsort(deadline,t,sizeof(int),cmpfunc_int);
        reorder(deadline,period,t);
      } 

      for(int i=0;i < t;i++){
            C[i]= util[i]*period[i];
            if(strcmp(str,"rm") == 0){
              deadline[i]=period[i];
            }
            Sched_AddT(Func_init, deadline[i], period[i],i,C[i]);
            TaskSet_Add(i,j,Tasks[i]);                     
      }

        //HYPERBOLIC BOUND (rm Utilization)
        if(strcmp(str,"rm") == 0){
          if(hyperbolic(C,period,util,t) == 1){
              // printf("\nHyperbolic :Task Set %d is schedulable\n",j);
              positive++;
          }

          else if(hyperbolic(C,period,util,t) == 0){
              // printf("\nHyperbolic :Task Set %d is NOT schedulable\n",j);
          }
        }

        //LEAST UPPER BOUND (rm & dm Utilization)
        if(strcmp(str,"rm") == 0){
          if(leastupperband (C,period,t) == 1){
              // printf("LUB :Task Set %d is schedulable\n",j);
              positive2++;
          }

          else if(leastupperband (C,period,t) == 0){
              // printf("LUB :Task Set %d is NOT schedulable\n",j);
          }

          else{
              // printf("LUB :Task Set %d is indetermined\n",j);
              ++indetermined;
          }
        }

        if(strcmp(str,"dm") == 0){
          if(leastupperband (C,deadline,t) == 1){
              // printf("LUB :Task Set %d is schedulable\n",j);
              positive2++;
          }

          else if(leastupperband (C,deadline,t) == 0){
              // printf("LUB :Task Set %d is NOT schedulable\n",j);
          }

          else{
              // printf("LUB :Task Set %d is indetermined\n",j);
              ++indetermined;
          }
        }

        //RESPONSE TIME ANALYSIS
        if(strcmp(str,"rm") == 0){
          if(responsetimeanalysis(C, period, deadline, t, j)==1)
            rta_counter++;
        }
        if(strcmp(str,"dm") == 0){
          for (int i=0; i<t; i++)
          {
            if(deadline[i]<=period[i])
            {
              counter++;
            }
          }

          if(counter == t){
            counter = 0;
            if(responsetimeanalysis(C, period, deadline, t, j)==1)
              rta_counter++;
          }

          else
            counter = 0;
        }

    //Calculation of the distribution of the utilizations
    for (int z = 0; z < t;z++){
        // printf("\nSet %d/%d\n",j,Nsets-1);
        // printf("Task %d/%d\n",z,t-1);
        // printf("Period %d\n",period[z]);
        // printf("Execution time %d\n",C[z]);
        // printf("Deadline %d\n",deadline[z]);
        // printf("Utilization %lf\n",util[z]);
      for (int n = 0;n < Bars;n++){
        if((util[z] >= n*(ISize)) && (util[z] < (n+1)*ISize)){
          histograma[n]++;
        }
      }
    }

    //Average of each task set
    set[j].avgUtil = average(util,t);
    // printf("Utilization Average :%.4f\n",set[j].avgUtil);
    // printf("Period Average :%.4f\n",average((double*)period,t));
    // printf("Deadline Average :%.4f\n",average((double*)deadline,t));
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
    printf("Number of tasks per Set :%d\n",t);
    printf("Number of Sets :%d\n",Nsets);
    printf("Total Average Utilization :%.4lf\n",average(aux,Nsets));
    // printf("Period Average :%.4f\n",average(period,t*Nsets));
    // printf("Deadline Average :%.4f\n",average(deadline,t*Nsets));

    save("Hyperbolic.csv",((double)1.0*positive/Nsets),0,t,UT,var);
    save("LUB.csv",(double)(1.0*positive2/Nsets),0,t,UT,var);
    save("RTA.csv",(double)(1.0*rta_counter/Nsets),0,t,UT,var);
    // if(t == TaskSetSize)
    //   save_hist("Dist_Util.csv",histograma,Bars);

    free(histograma);
    free(C);
    free(period);
    free(deadline);
    free(util);
    free(aux);
  }
  }
  
    // while (1) {
    //      Sched_Schedule(TaskSetSize);
    //      Sched_Dispatch();
    // }

}
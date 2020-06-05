#include <stdio.h>
#include "Scheduler.h"
#include "aux1.h"
#include "tests.h"
#include "PIP.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int main (int argc, char *argv[]){
  char var[20];
  double UT_aux;
  int TSS=0;
  int harmonic,blocking;
  printf("Escolha uma variável para manter constante UT ou TSS (Task Set Size) \n");
  scanf("%s",var);
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
    if(TSS > TaskSetSize){
      printf("O mínimo escolhido é maior que o máximo\n");
      return 0;
    }
    SetSizeMin =TSS; // Limite inferior do Task Set Size
  }
  else if(strcmp(var,"TSS") == 0){
    printf("Escolha o valor para UT max \n");
    scanf("%lf",&UT_aux);
    UT_MAX=UT_aux; // Limite superior da Utilização média de cada task set
    printf("Escolha o valor para UT min \n");
    scanf("%lf",&UT_aux);
    if(UT_aux > UT_MAX){
      printf("O mínimo escolhido é maior que o máximo\n");
      return 0;
    }
    UT_MIN=UT_aux; // Limite inferior da Utilização média de cada task set
    printf("Escolha um valor para TSS \n");
    scanf("%d",&TSS);
    TaskSetSize=TSS; // Limite superior do Task Set Size
    SetSizeMin =TSS; // Limite inferior do Task Set Size
  }

  else{
    printf("Variável não reconhecida\n");
    return 0;
  }

  printf("Periodos harmónicos (1/0)?\n");
  scanf("%d",&harmonic);
  printf("Cálculo do RTA com blocking times(1/0)?\n");
  scanf("%d",&blocking);
  //strcpy(var,"TSS");
  //strcpy(var,"UT");

  //harmonic=true;
  // TaskSetSize=15; // Limite superior do Task Set Size
  // SetSizeMin =1; // Limite inferior do Task Set Size
  // UT_MIN=0.15; // Limite inferior da Utilização média de cada task set
  // UT_MAX=0.15; // Limite superior da Utilização média de cada task set

  remove("Hyperbolic.csv"); //remover os ficheiros resultantes da última execução
  remove("LUB.csv");
  remove("RTA.csv");
  remove("Dist_Util.csv");
  remove("Priority Inheritances.txt");     

  for(double UT=UT_MIN;UT < UT_MAX+UT_int;UT+= UT_int){
  for(int t=SetSizeMin;t < TaskSetSize+1;t++){
    Sched_Init(t);

    //Tamanho dos dados
    int size,*B; 
    if(strcmp(var,"TSS") == 0){
      size = UT_MAX/UT_MIN;
    }
    else{
      size=10;
    }

    //Inicialização das variáveis
    int *histograma=malloc(sizeof(int)*(Nsets)*size*(TaskSetSize-SetSizeMin+1)*Bars);
    int *period = malloc(sizeof(int)*size*Nsets),*C=malloc(sizeof(int)*size*Nsets),*deadline = malloc(sizeof(int)*size*Nsets);
    double *util = malloc(sizeof(double)*size*Nsets);
    double *aux0 = malloc(sizeof(double)*size*Nsets);
    double *aux1 = malloc(sizeof(double)*size*Nsets);
    double *aux2 = malloc(sizeof(double)*size*Nsets);
    double *aux3 = malloc(sizeof(double)*size*Nsets);
    double *aux4 = malloc(sizeof(double)*size*Nsets);
    double sum=0.0;
    int positive =0, positive2 =0,indetermined = 0, counter=0, rta_counter= 0, rows, i, n_sem;

    srand((unsigned)(time(NULL)*(unsigned int)(UT*100*t))); // dá reset à seed do Rand a cada iteração
    memset(histograma,0,0);
    memset(aux0,0,0);
    memset(aux1,0,0);
    memset(aux2,0,0);
    memset(aux3,0,0);
    memset(aux4,0,0);
    memset(C,0,0);
    memset(deadline,0,0);
    memset(util,0,0);
    memset(period,0,0);
    char *sched_type= argv[1]; // the type of scheduling is passed as a argument in the terminal
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for(int j=0;j < Nsets;j++){
        //Task Generation
        TaskSet_New(j);

        //Rate monotonic
        if(strcmp(sched_type,"rm") == 0){
          for(int i=0;i < t;i++){
              if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
              else{period[i]= 2*(1+rand()%(MAXPERIOD));}// Ti evenly divides Ti+1
              C[i]= rand()%(period[i]) ; // the computation time C i uniform in [0,Ti]
          }
          qsort(period,t,sizeof(int),cmpfunc_int);

          for(int i=0;i < t;i++){
              util[i] =(double) C[i]*1.0/period[i];
          }
        }

        //Deadline monotonic
        if(strcmp(sched_type,"dm") == 0){
          for(int i=0;i < t;i++){
              if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
              else{period[i]= 2*(1+rand()%(MAXPERIOD));}
              deadline[i]= rand()%(period[i]); // D <= P
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
                if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
                else{period[i]= 2*(1+rand()%(MAXPERIOD));}
                if(strcmp(sched_type,"dm") == 0){
                  deadline[i]= rand()%(period[i]); // D <= P
                }
              }

              else if((sum > UT*t) && (i <= (t-1))){
                sum=sum-util[i];
                util[i] = (UT*t - sum)/(t - i);
                sum+=util[i];
                if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
                else{period[i]= 2*(1+rand()%(MAXPERIOD));}
                if(strcmp(sched_type,"dm") == 0){
                  deadline[i]= rand()%(period[i]); // D <= P
                } 
                
                while(i < t -1){
                  ++i;
                  util[i]=util[i-1];
                  sum+=util[i];
                  if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
                  else{period[i]= 2*(1+rand()%(MAXPERIOD));}
                  if(strcmp(sched_type,"dm") == 0){
                    deadline[i]= rand()%(period[i]); // D <= P
                  } 
                }         
              }
              else{
                  if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
                  else{period[i]= 2*(1+rand()%(MAXPERIOD));}
                  if(strcmp(sched_type,"dm") == 0){
                    deadline[i]= rand()%(period[i]); // D <= P
                  } 
              }
        }
      
      if(strcmp(sched_type,"rm") == 0){
        qsort(period,t,sizeof(int),cmpfunc_int);
      }

      if(strcmp(sched_type,"dm") == 0){
        reorder(deadline,period,t);
      } 

      for(int i=0;i < t;i++){
            C[i]= util[i]*period[i];
            if(strcmp(sched_type,"rm") == 0){
              deadline[i]=period[i];
            }
            Sched_AddT(deadline[i],period[i],i,C[i]);
            TaskSet_Add(i,j,Tasks[i]);                     
      }

        //HYPERBOLIC BOUND (rm Utilization)
        if(strcmp(sched_type,"rm") == 0){
          if(hyperbolic(deadline,period,util,t) == 1){
              // printf("\nHyperbolic :Task Set %d is schedulable\n",j);
              positive++;
          }

          else if(hyperbolic(C,period,util,t) == 0){
              // printf("\nHyperbolic :Task Set %d is NOT schedulable\n",j);
          }
        }

        //LEAST UPPER BOUND (rm & dm Utilization)
        if(strcmp(sched_type,"rm") == 0){
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

        if(strcmp(sched_type,"dm") == 0){
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

        //Priority Inheritance Study
        rows= t;

        B = malloc(sizeof(int)*rows);

        if(rows > 1){
          n_sem = rand() % (rows-1) + 1;

          // printf("\n\n------------------------------------\n");

          B = calc_b(period, rows, n_sem, j, UT); //array that contains the blocking times
        }
        
        //RESPONSE TIME ANALYSIS
        if(strcmp(sched_type,"rm") == 0){
          if(responsetimeanalysis(C, period, deadline, t, j,blocking,B)==1)
            rta_counter++;
        }
        if(strcmp(sched_type,"dm") == 0){
          for (int i=0; i<t; i++)
          {
            if(deadline[i]<=period[i])
            {
              counter++;
            }
          }

          if(counter == t){
            counter = 0;
            if(responsetimeanalysis(C, period, deadline, t, j,blocking,B)==1)
              rta_counter++;
          }

          else
            counter = 0;
        }

    //Calculation of the distribution of the utilizations
    for(int z = 0; z < t;z++){
        //  printf("\nSet %d/%d\n",j,Nsets-1);
        //  printf("Task %d/%d\n",z,t-1);
        //  printf("Period %d\n",period[z]);
        //  printf("Execution time %d\n",C[z]);
        //  printf("Deadline %d\n",deadline[z]);
        //  printf("Utilization %lf\n",util[z]);
        //  printf("Blocking time %d\n",B[z]);
      for (int n = 0;n < Bars ;n++){
        if((util[z] >= n*(ISize)) && (util[z] < (n+1)*ISize)){
          histograma[n]++;
        }
      }
    }

    //Calculate the average of each task set
    set[j].avgUtil = average(util,t);
    set[j].avgPeriod = average_int(period,t);
    set[j].avgDeadline = average_int(deadline,t);
    set[j].avgExecutingtime = average_int(C,t);
    set[j].avgBlockingtime = average_int(B,t);
   }
  

    for (int j = 0; j < Nsets;j++){
      aux0[j] = (double) set[j].avgUtil;
      aux1[j] = (double) set[j].avgPeriod;
      aux2[j] = (double) set[j].avgDeadline;
      aux3[j] = (double) set[j].avgExecutingtime;
      aux4[j] = (double) set[j].avgBlockingtime;
    }

    if(strcmp(sched_type,"dm") == 0){
      printf("\nDeadline Monotonic Scheduling ");
    }

    if(strcmp(sched_type,"rm") == 0){
      printf("\nRate Monotonic Scheduling ");
    }

    printf("\nResultados dos testes \n");
    printf("Hyperbolic Condition :%.4f dos task sets são escalonáveis\n",((double)1.0*positive/Nsets));
    printf("LUB Condition :%.4f dos task sets são escalonáveis\n",((double)1.0*positive2/Nsets));
    printf("LUB Condition :%.4f dos task sets são indeterminados\n",((double)1.0*indetermined/Nsets));
    printf("RTA Condition :%.4f dos task sets são escalonáveis\n",((double)1.0*rta_counter/Nsets));
    printf("Number of tasks per Set :%d\n",t);
    printf("Number of Sets :%d\n",Nsets);
    printf("Total Average Utilization :%.4lf\n",average(aux0,Nsets));
    printf("Total Period Average :%.4f\n",average(aux1,Nsets));
    printf("Total Deadline Average :%.4f\n",average(aux2,Nsets));
    printf("Total Execution time Average :%.4f\n",average(aux3,Nsets));
    printf("Total Blocking time Average :%.4f\n",average(aux4,Nsets));

    save("Hyperbolic.csv",((double)1.0*positive/Nsets),0,t,UT,var);
    save("LUB.csv",(double)(1.0*positive2/Nsets),0,t,UT,var);
    save("RTA.csv",(double)(1.0*rta_counter/Nsets),0,t,UT,var);
    if(t == TaskSetSize)
      save_hist("Dist_Util.csv",histograma,Bars);

    free(histograma);
    free(C);
    free(period);
    free(deadline);
    free(util);
    free(aux0);
    free(aux1);
    free(aux2);
    free(aux3);
    free(aux4);
  }
  }

}
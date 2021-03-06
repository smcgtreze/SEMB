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
  char sched_type[20];
  double UT_aux;
  int TSS=0;
  int harmonic,blocking,save_priority,save_taskinfo,CPU_EDF;

  printf("Escolha o scheduling rm(Rate monotonic) ou dm(Deadline monotonic) \n");
  scanf("%s",sched_type);
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

  // printf("Periodos harmónicos (1/0)?\n");
  // scanf("%d",&harmonic);
  printf("Guardar as priority inheritances num ficheiro(1/0)?\n");
  scanf("%d",&save_priority);
  printf("Cálculo do RTA com blocking times(1/0)?\n");
  scanf("%d",&blocking);
  printf("Guardar as parâmetros relativos às tasks num ficheiro(1/0)?\n");
  scanf("%d",&save_taskinfo);
  printf("Cálculos do CPU demand para EDF(1/0)?\n");
  scanf("%d",&CPU_EDF);
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
  remove("CPU_Demand.csv");
  remove("info.txt");
  remove("task_info.txt");   

  FILE * task_info;
  task_info = fopen("task_info.txt","w");

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
    int positive =0, positive2 =0,positive3=0 ,indetermined = 0, counter=0, rta_counter= 0, rows, i, n_sem;

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
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for(int j=0;j < Nsets;j++){
        //Task Generation
        TaskSet_New(j);

        //Rate monotonic
        if(strcmp(sched_type,"rm") == 0){
          for(int i=0;i < t;i++){
              //if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
              period[i]= 2*(1+rand()%(MAXPERIOD));// Ti evenly divides Ti+1
               C[i]= 1+ rand()%(period[i]); // the computation time C i uniform in [0,Ti]
          }
          qsort(period,t,sizeof(int),cmpfunc_int);

          for(int i=0;i < t;i++){
              util[i] =(double) C[i]*1.0/period[i];
          }
        }

        //Deadline monotonic
        if(strcmp(sched_type,"dm") == 0){
          for(int i=0;i < t;i++){
             //if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
              period[i]= 2*(1+rand()%(MAXPERIOD));
              deadline[i]= 1 + rand()%(period[i]);
              C[i]= 1+ rand()%(deadline[i]); //tempo de execução sempre menor que o deadline
          }

          qsort(deadline,t,sizeof(int),cmpfunc_int);

          for(int i=0;i < t;i++){
              util[i] =(double) C[i]*1.0/period[i];
          }
        }

        //Característico do UUnisort
        qsort(util,t,sizeof(double),cmpfunc);

        for(int z=0;z < t-1;z++){
            util[z] = util[z+1] - util[z]; 
        }
        
        sum=0.0;
        for(int i=0;i < t;i++){
              sum+=util[i];

              if((sum < UT*t) && (i == t-1)){ //apenas é necessário alterar a utilização da última task
                sum=sum-util[i];
                util[i] = (UT*t - sum); //A utilização atribuída é a diferença entre a soma de utilização necessária e a soma atual
                sum+=util[i];
                //if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
                period[i]= 2*(1+rand()%(MAXPERIOD)); //recalcular os períodos
                if(strcmp(sched_type,"dm") == 0){
                 deadline[i]= 1 + rand()%(period[i]);
                }
              }

              if((sum > UT*t) && (i < t)){ //é necessário alterar a utilização desde a task em que a soma de utilizações foi ultrapassada
                sum=sum-util[i];
                util[i] = (UT*t - sum)/(t - i);//A utilização atribuída é a diferença entre a soma de utilização necessária e a soma atual a dividir pelas tasks que faltam
                sum+=util[i];
                //if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
                period[i]= 2*(1+rand()%(MAXPERIOD));
                if(strcmp(sched_type,"dm") == 0){
                 deadline[i]= 1 + rand()%(period[i]);
                } 
                
                while(i < t ){
                  ++i;
                  util[i]=util[i-1]; // a utilização da task é igual á utilização da anterior, isto é a diferença de somas/nºde tasks no set
                  sum+=util[i];
                  //if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
                  period[i]= 2*(1+rand()%(MAXPERIOD));
                  if(strcmp(sched_type,"dm") == 0){
                   deadline[i]= 1 + rand()%(period[i]);
                  } 
                }         
              }
              else{ // casos em que nenhum deles se verifica
                  //if(harmonic){period[i] = pow(2,(rand()%(MAXN)));}
                  period[i]= 2*(1+rand()%(MAXPERIOD));
                  if(strcmp(sched_type,"dm") == 0){
                   deadline[i]= 1 + rand()%(period[i]);
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
              //printf("\nHyperbolic :Task Set %d is schedulable\n",j);
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

          B = calc_b(period, rows, n_sem, j, UT,save_priority); //array that contains the blocking times
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


      //CPU DEMAND ANALYSIS
      if(CPU_EDF){
        if(cpudemand(C,period,deadline,t,j)==1){
        positive3++;
      }
    }

    //Calculation of the distribution of the utilizations
    for(int z = 0; z < t;z++){
        if(save_taskinfo){
         fprintf(task_info,"\nSet %d/%d\n",j,Nsets-1);
         fprintf(task_info,"Task %d/%d\n",z,t-1);
         fprintf(task_info,"Period %d\n",period[z]);
         fprintf(task_info,"Execution time %d\n",C[z]);
         fprintf(task_info,"Deadline %d\n",deadline[z]);
         fprintf(task_info,"Utilization %lf\n",util[z]);
         fprintf(task_info,"Blocking time %d\n",B[z]);
        }
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
    printf("EDF CPU Demand Analysis :%.4f dos task sets são escalonáveis\n",((double)1.0*positive3/Nsets));
    printf("Number of tasks per Set :%d\n",t);
    printf("Number of Sets :%d\n",Nsets);
    printf("Total Average Utilization per task of a set:%.4lf\n",average(aux0,Nsets));
    printf("Total Period Average :%.4f\n",average(aux1,Nsets)/t);
    printf("Total Deadline Average :%.4f\n",average(aux2,Nsets)/t);
    printf("Total Execution time Average :%.4f\n",average(aux3,Nsets)/t);
    printf("Total Blocking time Average :%.4f\n",average(aux4,Nsets)/t);

    save("Hyperbolic.csv",((double)1.0*positive/Nsets),0,t,UT,var);
    save("LUB.csv",(double)(1.0*positive2/Nsets),0,t,UT,var);
    save("RTA.csv",(double)(1.0*rta_counter/Nsets),0,t,UT,var);
    save("CPU_Demand.csv",(double)(1.0*positive3/Nsets),0,t,UT,var);
    
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
FILE* file;
file= fopen("info.txt","w");

if(file == NULL){
  printf("Erro a abrir o ficheiro\n");
}
fprintf(file,"%s\n",sched_type);
fprintf(file,"%s\n",var);
if(strcmp(var,"UT") == 0){fprintf(file,"%.2lf\n",UT_aux);}
if(strcmp(var,"TSS") == 0){fprintf(file,"%d\n",TSS);}
// fprintf(file,"%d\n",harmonic);
fprintf(file,"%d\n",blocking);

}
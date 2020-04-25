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

int main (int argc, char *argv[]) {
    Sched_Init();
    int NTasks= atoi(argv[1]);
    int period;

    Sched_AddT(average, 50, 10*CLOCKS_PER_SEC,0,0);
    srand((unsigned)(time(NULL)));

    for(int i=1;i < NTasks+1;i++){
        period=rand()%(10 - 0 +1); // de 0 a 10
        //printf("Periodo %d\n",period);
        Sched_AddT(Func_init, 50, period*CLOCKS_PER_SEC,i,100);
    }

    while (1) {
         Sched_Schedule();
         Sched_Dispatch();
    }
}

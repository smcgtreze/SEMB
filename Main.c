#include<stdio.h>
#include"Scheduler.h"
#define s 1000000

void FuncX_init(){
    printf("FuncX running\n");
}

void FuncY_init(){
    printf("FuncY running\n");  
}

int main (void) {
    Sched_Init();

    /* periodic task */
    //FuncX_init();
    Sched_AddT(FuncX_init, 60, 4*s,1);

    /* one-shot task */
    //FuncY_init();
    Sched_AddT(FuncY_init, 50, 0,0);

    while (1) {
        Sched_Schedule();
        Sched_Dispatch();
    }
}

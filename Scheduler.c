#include<stdio.h>
#include"Scheduler.h"

int Sched_Init(void){
    for(int x=0; x<20; x++)
        Tasks[x].func = 0;
    /*
    * Also configures
    * interrupt that
    * periodically calls
    * Sched_Schedule().
    * now calls int_handler()
    */
}

int Sched_AddT(void (*f)(void),int d, int p,int pri){
    //for(int x=0; x<20; x++)
        if (!Tasks[pri].func) {
            Tasks[pri].period = p;
            Tasks[pri].delay = d;
            Tasks[pri].exec = 0;
            Tasks[pri].func = f;
            return pri;
        }
    return -1;
}

void Sched_Schedule(void) {
    for(int x=0; x<20; x++) {
        if(Tasks[x].func){
            if(Tasks[x].delay){
                Tasks[x].delay--;
            }   
            else {
                /* Schedule Task */
                Tasks[x].exec++;
                Tasks[x].delay = Tasks[x].period-1; //why?
            }
        }
    }
}

void Sched_Dispatch(void) {
    for(int x=0; x<20; x++) {
        if((Tasks[x].func)&&(Tasks[x].exec)){
            Tasks[x].exec--;
            Tasks[x].func();
            
            // Delete task if one-shot
            if(!Tasks[x].period)
                Tasks[x].func = 0;
        return;
        }
    }
}
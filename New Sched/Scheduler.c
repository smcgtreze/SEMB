#include<stdio.h>
#include"Scheduler.h"
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

int cur_task=MaxTaskSetSize; // 0-> High 19->Low


int Sched_AddT(int d, int p,int pri,int c){
    //for(int x=0; x<20; x++)
            Tasks[pri].period = p;
            Tasks[pri].deadline = d;
            Tasks[pri].exec = c;
            Tasks[pri].id  = pri;
            return pri;
}

void TaskSet_New(int id){
    set[id].id = id;
}

void TaskSet_Add(int taskid,int setid, Sched_Task_t newtask){
    set[setid].task[taskid] = newtask;
}

// void Sched_Schedule(int TaskSetSize) {
//     for(int x=0; x< TaskSetSize ; x++) {
//         if(Tasks[x].func){
//             if (!Tasks[x].func)
//                 continue;
//             if(Tasks[x].delay){
//                 Tasks[x].delay--;
//             }   
//             else {
//                 /* Schedule Task */
//                 //printf("Task  %d scheduled\n",x);
//                 Tasks[x].exec++;
//                 Tasks[x].delay = Tasks[x].period-1; //why?
//             }
//         }
//     }
// }

// void Sched_Dispatch(void) {
//     int prev_task = cur_task;
//     for(int x=0; x<cur_task; x++) {
//         if((Tasks[x].func)&&(Tasks[x].exec)){
//             Tasks[x].exec--;
//             cur_task = x;
//             Tasks[x].func(x);
//             cur_task = prev_task;
            
//             // Delete task if one-shot
//             if(!Tasks[x].period)
//                 Tasks[x].func = 0;
//         //return;
//         }
//     }
// }
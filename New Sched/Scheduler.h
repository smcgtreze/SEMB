#define MaxTaskSetSize 20
#define Nsets 1000
#define Bars 20
#define ISize 0.050
#define UT_int 0.05
#define MAXPERIOD 5000

 /*task periods are more
deterministic, since are defined by the user and then enforced by the operating system
On the other hand, at an early stage of evaluating a guarantee test without any a priori
knowledge about the environment where it is going to be used, assuming some probability
density for the period is something we cannot avoid.*/

typedef struct {
/* period in ticks */
int period;
/* ticks until next activation */
int delay;
/* function pointer */
void (*func)(int);
/* activation counter */
int exec;
int wce;
int id;
} Sched_Task_t;

typedef struct {
Sched_Task_t task[MaxTaskSetSize];
int id;
double avgUtil;
} Task_set;

Sched_Task_t Tasks[MaxTaskSetSize]; //array off structures for all tasks
Task_set set[Nsets];
int TaskSetSize;
int SetSizeMin;
double UT_MAX;
double UT_MIN;
// int cur_task = 20; // 0-> High 19->Low

int Sched_Init(int TaskSetSize);
/* - Initialise data
* structures.
*
* - Configure interrupt
* that periodically
* calls int_handler()
* Sched_Schedule().
*/

void Sched_Schedule(int TaskSetSize);
/* Verifies if any
* task needs to be
* activated, and if so,
* increments by 1 the
* task's pending
* activation counter.
*/


void Sched_Dispatch(void); 
/* Verifies if any task
* with higher priority than the currently
* executing task,
* has an activation
* counter > 0,
* and if so, calls that
* task.
*/

int Sched_AddT(void (*f)(int),int d, int p,int pri,int wce);
    //adds a task to the schedule

void TaskSet_New(int id);

void TaskSet_Add(int taskid,int setid, Sched_Task_t newtask);

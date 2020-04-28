#define TaskSetSize 5
#define Nsets 1000

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
Sched_Task_t task[TaskSetSize];
int id;
} Task_set;

Sched_Task_t Tasks[TaskSetSize]; //array off structures for all tasks
Task_set set[Nsets];
// int cur_task = 20; // 0-> High 19->Low

int Sched_Init(void);
/* - Initialise data
* structures.
*
* - Configure interrupt
* that periodically
* calls int_handler()
* Sched_Schedule().
*/

void Sched_Schedule(void);
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


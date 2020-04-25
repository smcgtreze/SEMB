typedef struct {
/* period in ticks */
int period;
/* ticks until next activation */
int delay;
/* function pointer */
void (*func)(void);
/* activation counter */
int exec;
} Sched_Task_t;

Sched_Task_t Tasks[20]; //array off structures for all tasks

int Sched_Init(void);
/* - Initialise data
* structures.
*
* - Configure interrupt
* that periodically
* calls
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
* has an activation
* counter > 0,
* and if so, calls that
* task.
*/

int Sched_AddT(void (*f)(void),int d, int p,int pri);
    //adds a task to the schedule

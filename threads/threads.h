#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <malloc.h>
#include <err.h>

/*constants declarations*/
#define MAX_THR 32
#define STACK_SIZE 64*1024

/*structs declarations*/
typedef enum {
	Free, 
	Started,
	Died,
}status;
enum 
{
	MILISEC=200
};
typedef struct _mythread{
 	ucontext_t ucontext;
	int thid;
	void *stack;
	int stack_size;
	int state; /*0->Free, 1->Started, 2->Died*/
	time_t t_ini;
}_mythread;

  /*Array of _mythreads*/
_mythread thread_table[MAX_THR];

/*functions declarations*/
void initthreads(void);
int createthread(void (*mainf)(void*), void *arg, int stacksize);
void exitsthread(void);
void yieldthread(void);
int curidthread(void);

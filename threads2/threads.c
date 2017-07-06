/*
	gcc -o threads -Wall -Wshadow threads.c && threads
 */
#include "threads.h"
#include <stdio.h>
#include <time.h>
static int first=0;
int now=0;
void 
inittable(_mythread table[]){
	int i=1;
	for(i = 1;i <MAX_THR;i++){
    	table[i].state=Free;
    	free(table[i].stack);
	}
}

void 
cleantable( _mythread table[] ){
	int i=0;
	for(i = 0;i <MAX_THR;i++){
		if(table[i].state==Died){
			table[i].stack_size=0;
    		table[i].state=Free;
    		free(table[i].stack);
   		}
	}
}
int 
checksize(int stacksize){

	if(stacksize!=0){
		return stacksize;
	}else{
		return STACK_SIZE;
	}		
}
void
printstate(){ 
    for(int i=0; i < MAX_THR; i++){
        	fprintf(stderr,"thread %d.state-> %d\n",i,thread_table[i].state);
   	
    }
}
time_t 
static settime(){
	struct timeval aux;
	gettimeofday(&aux, NULL);
	time_t total = aux.tv_sec*1000 + aux.tv_usec/1000;
	return total;
}
/*to use it in the scheduler,hacer que retorne un valor*/
int 
awakethread(int thid){
	time_t rightNow = settime();
	time_t time = thread_table[thid].t_slept;
	if( ((long long)time-(long long)rightNow>0) && (thread_table[thid].state==Sleep)){
		thread_table[thid].state=Waiting;
		fprintf(stderr, "Waking up to thread %d \n", thid);
		return 1;
	}

	return -1;
}

int 
static freedom(_mythread table[]){
	int i;
	int position=-1;
	for (i = 0; i < MAX_THR; i++) {
		if (table[i].state == Died){
 			free(table[i].stack);
 			table[i].stack_size=0;
 			table[i].state = Free;
 			position = i;
			break;
		}else if(table[i].state==Free){
			position=i;
			break;
		}
	}
	return position;
}
int
static scheduler(){
	int next;
	int alive=0;//dormido
	int suspend=0;

	next= (now+1)%MAX_THR;

	while(next>=0){
		awakethread(next);
		if(thread_table[next].state==Sleep){
			alive=1;
			return next;
		}else if(thread_table[next].state==Waiting){
			return next;
		}else if(thread_table[next].state==Suspended){
			suspend=1;
		}
		if(now==next){
			awakethread(now);
			if(thread_table[next].state==Started){
				return next;
			}else if(alive==0 && thread_table[next].state==Died && suspend==0){
				fprintf(stderr, "The program has ended fine, see you soon!!\n" );
				printstate();
				cleantable(thread_table);
				exit(1);
			}else if(thread_table[next].state==Waiting){
				fprintf(stderr, "err: This was the last thread, and it is sleept\n");
				exit(1);
			}else if(thread_table[next].state==Suspended){
				fprintf(stderr, "err: This was the last thread, and it is suspended\n");
				exit(1);
			}else if(thread_table[next].state==Sleep){
				fprintf(stderr, "err: This was the last thread, and it wants sleep\n");
				exit(1);
			}else if (thread_table[next].state==Died && suspend>0){
				fprintf(stderr, "err: This was the last thread, and there are threads still suspended\n");
				printstate();
				cleantable(thread_table);
				exit(1);
			}
		}
		next=(next+1)%MAX_THR;
	}
	return next;
}

int 
createthread(void (*mainf)(void*), void *arg, int stacksize){

	int tid=freedom(thread_table);
	if(tid < 0){
		return -1;
	}
	/*position must be 31 or less*/
	if(getcontext(&thread_table[tid].ucontext)!=-1){
		void * stack;
		stack=malloc(checksize(stacksize));
		thread_table[tid].ucontext.uc_stack.ss_size=(checksize(stacksize));
    	thread_table[tid].thid=tid;
    	thread_table[tid].state=Waiting;
    	thread_table[tid].ucontext.uc_link = NULL;
		thread_table[tid].ucontext.uc_stack.ss_sp = stack;
		thread_table[tid].t_slept=0;
		makecontext(&thread_table[tid].ucontext, (void(*)(void))mainf, 1, arg);
		return tid;
	}else{
		err(1, "Not possible to make getcontext in the %d thread\n",tid);
		return -1;
	}
}

void
initthreads(void){

	if (first==0){
			first=1;
    	if(getcontext(&thread_table[0].ucontext)!=-1){
    		thread_table[0].stack_size=0;
    		thread_table[0].thid=0;
    		thread_table[0].state=Started;
    		thread_table[0].t_ini=settime();
    		inittable(thread_table);
    	}else{
    		fprintf(stderr, "Not possible to make getcontext in the first one thread\n");
    	}

	}else{
		fprintf(stderr,"You can only use this once!\n");
		exit(1); 
	}

}

int 
static checktime(time_t t_ini){
	time_t rightNow = settime();

	if( (long long)rightNow-(long long)t_ini >(long long)MILISEC){
		return 1;
	}
	return -1;
}

int 
static checkstatus(){

	if((thread_table[now].state==Died) || (thread_table[now].state==Sleep) || (thread_table[now].state==Suspended))
	return -1;

	return 1; 
}
/*First at all, change the state, so other new thread can use this position*/
void
exitsthread(void){
    //th_count--;
    thread_table[now].state=Died;
    yieldthread();
}
void 
yieldthread(void){
	int next=0;

	next=scheduler();

	if(next!=-1){// Si existe otro thread, compruebo el tiempo que llevo aquí
		int time = checktime(thread_table[now].t_ini);
		if((time>0) || checkstatus()){
			int now2=now;	
			thread_table[next].state=Started;
			if(thread_table[now].state==Started){
				thread_table[now].state=Waiting;
			}
			now=next;
			thread_table[next].t_ini=settime();
				if(next==MAX_THR){
					next=0;
				}
			swapcontext(&thread_table[now2].ucontext, &thread_table[next].ucontext);
		}
		
	}else{ //if we get -1 i wanna die too
		exit(0);
	}
}
/*This is the current position in the table, means this is the thread doing job*/
int 
curidthread(void){
    return (thread_table[now].thid);
}
/******************************************************************************************/
int 
resumethread(int thid){

	if(thread_table[thid].state == Suspended){
		fprintf(stderr, "Resumen of thread %d\n",thid );
		thread_table[thid].state = Waiting;/*Free*/
		return 0;
	}
	return -1;
}

/*This thread will be suspended*/
void 
suspendthread(void){
	thread_table[now].state = Suspended;
	yieldthread();
}

int 
killthread(int thid){
	if (thread_table[thid].state == Died || thread_table[thid].state == Free || thread_table[thid].state == Started || thread_table[thid].state==Sleep){
		fprintf(stderr, "Impossible to kill thread %d, state %d\n",thid,thread_table[thid].state );
		return -1;
	}
	thread_table[thid].state = Died;
	fprintf(stderr, "Murdered thread %d, state %d\n",thid,thread_table[thid].state );
	return thid;
}

void
sleepthread(int msec){
	if(thread_table[now].state==Sleep){
		fprintf(stderr, "New time to sleep asinged \n" );
	}
	thread_table[now].t_slept = settime()+ msec;
	thread_table[now].state = Sleep;
	yieldthread();
}

int 
suspendedthreads(int **list){
	
	int * aux;
	int i=0;
	int j = 0;

	aux = (int*)malloc(MAX_THR*sizeof(int));
	for(i = 0; i < MAX_THR; i++){
		if (thread_table[i].state == Suspended){
			aux[j] = i;
			j++;
		}
	}
	*list = aux;
	
	return j;
}
/*********************************************************************************************************/
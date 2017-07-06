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
	int i=0;
	for(i = 0;i <MAX_THR;i++){
		table[i].stack_size=0;
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

	int next= (now+1)%MAX_THR;
	int count=0;
	while(next>=0){

		if(thread_table[next].state==Started){
			return next;
		}
		if(thread_table[next].state==Free){
			next= (now+1)%MAX_THR;
			continue;
		}
		if(thread_table[next].state==Died && (now!=next)){
			now++;
			next= (now+1)%MAX_THR;
			count++;
			if(count==31){
				printstate();
			}
			fprintf(stderr, "contador %d\n",count );
		}
		if(now==next){

			if(thread_table[next].state==Started){
				//thread_table[next].t_ini=settime();
				return next;
			}else if(thread_table[next].state==Died){
				fprintf(stderr, "The program has ended fine, see you soon!!\n" );
				printstate();
				cleantable(thread_table);
				exit(1);
			}

		}
		next= (now+1)%MAX_THR;
	}
	return -1;
}

int 
createthread(void (*mainf)(void*), void *arg, int stacksize){

	int tid=freedom(thread_table);
	
	if(tid < 0){
		return -1;
	}else{

		if(getcontext(&thread_table[tid].ucontext)!=-1){
			void * stack;
			stack=malloc(checksize(stacksize));
			thread_table[tid].ucontext.uc_stack.ss_size=(checksize(stacksize));
	    	thread_table[tid].thid=tid;
	    	thread_table[tid].state=Started;
	    	thread_table[tid].ucontext.uc_link = NULL;
			thread_table[tid].ucontext.uc_stack.ss_sp = stack;
			thread_table[tid].t_ini=settime();
			makecontext(&thread_table[tid].ucontext, (void(*)(void))mainf, 1, arg);
			return tid;
		}else{
			err(1, "Not possible to make getcontext for this thread\n");
		return -1;
		}
	}
}

void
initthreads(void){
	inittable(thread_table);

	if (first == 0){
    	if(getcontext(&thread_table[0].ucontext)!=-1){
    		thread_table[0].stack_size=0;
    		thread_table[0].thid=0;
    		thread_table[0].state=Started;
    		thread_table[0].t_ini=settime();
    		first=1;
    	}else{
    		err(1, "Not possible to make getcontext in the first one thread\n");
    	}

	}else{
		fprintf(stderr,"You can only use this once!\n");
		exit(1); 
	}
}

int 
static awakethread(time_t t_ini){
	time_t rightNow = settime();

	if( (long long)rightNow-(long long)t_ini >(long long)MILISEC){
		return 1;
	}
	return -1;
}
/*First at all, change the state, so other new thread can use this position*/
void
exitsthread(void){
    thread_table[now].state=Died;
    //fprintf(stderr, "Muriendo thread _________%d\n",curidthread() );
    yieldthread();
}
void 
yieldthread(void){
	int next=-1;

	next=scheduler();
	if(next!=-1){// Si existe otro thread, compruebo el tiempo que llevo aquí
		int time = awakethread(thread_table[now].t_ini);
		if(time>0){
			int now2=now;	
			thread_table[next].state=Started;
			thread_table[next].t_ini=settime();
			now=next;
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
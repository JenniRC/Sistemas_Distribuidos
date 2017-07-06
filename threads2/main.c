/*gcc -o main -Wall -Wshadow threads.c main.c && ./main*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "threads.h"    
#include <malloc.h>
#include <time.h>

static int * list;
void
imprimirLista(int* lit, int n){
 
    int i;
    if (n > 0) fprintf(stderr, "%d Suspended threads: ",n);
    for (i = 0; i < n; i++){
        fprintf(stderr, "%d,", lit[i]);
    }
    fprintf(stderr,"\n");
    free(lit);
}
void
sayhello(void *p){ 
    int i;

    for(i=0; i < 20; i++){
        fprintf(stderr,"Hello from %d\n",curidthread());
        yieldthread();
        usleep(1000);
    }
    exitsthread();
}
void
goodnight(void *p){ 
    int i;

    for(i=0; i < 20; i++){
        if( i==5 || i==12 || i==15) {
            fprintf(stderr,"Saying goodnight %d\n",curidthread());
            sleepthread(2000);
            yieldthread();
        }
    }
    exitsthread();
}
void
inactive(void *p){ 
    int i;

    for(i=0; i < 20; i++){
        if( i==1 ) {
            fprintf(stderr,"Suspending thread %d\n",curidthread());
            suspendthread();
            yieldthread();
        }
    }
    exitsthread();
}
void
active(void *p){ 
    int i;

    for(i=0; i < 20; i++){
        if(thread_table[i].state==Suspended) {
            fprintf(stderr,"Unsuspendig thread %d\n",curidthread());
            resumethread(i);
            yieldthread();
        }
    }
    exitsthread();
}
void
inactiveprint(void *p){ 
    int n = suspendedthreads(&list);
    imprimirLista(list, n);
    int i;
    for(i=0; i < 20; i++){
        yieldthread();
    }
    exitsthread();
    free(list);
}

void
printstates(void *p){ 
    for(int i=0; i < MAX_THR; i++){
        fprintf(stderr,"thread main %d.state-> %d\n",i,thread_table[i].state);
        if(i==15){
            fprintf(stderr,"Saying goodnight %d\n",curidthread());
            sleepthread(1500); /*To test if this thread come back where and when it should be*/
            yieldthread();
        }
    }
    yieldthread();
    exitsthread();
}

int
main(int argc, char* argv[]){
    list=NULL;
 
    initthreads();
 
    int arg =1;
    fprintf(stderr, "First hello from the main thread %d \n",curidthread() );
    for (int i = 0; i < MAX_THR  ; ++i){
        usleep(500);
        if (createthread(sayhello, &arg , STACK_SIZE) == -1)
            fprintf(stderr, "error al crear el thread_main\n");
        yieldthread();

        usleep(500);
        if (createthread(goodnight, &arg , STACK_SIZE) == -1)
            fprintf(stderr, "error al crear el thread_main\n"); 
        yieldthread();

        if (createthread(inactive, &arg , STACK_SIZE) == -1)
            fprintf(stderr, "error al crear el thread_main\n"); 
        yieldthread(); 

        if (createthread(inactiveprint, &arg , STACK_SIZE) == -1)
            fprintf(stderr, "error al crear el thread_main\n"); 
        yieldthread();
        
        if(i==3){
            usleep(200);
            if(killthread(8)<0)
                fprintf(stderr, "to kill thread %d is not possible\n",8 );
            if(killthread(7)<0)
                fprintf(stderr, "to kill thread %d is not possible\n",7 );
            if (createthread(printstates, &arg , STACK_SIZE) == -1)
                fprintf(stderr, "error al crear el thread_main\n"); 
            yieldthread();
            resumethread(3);
            resumethread(10);
            resumethread(13);/*-> Comment this line to get a bad ending*/
            if(killthread(7)<0)/*to test if to kill works fine*/
                fprintf(stderr, "to kill thread %d is not possible\n",7 );
            exitsthread();
            if (createthread(printstates, &arg , STACK_SIZE) == -1)
                fprintf(stderr, "error al crear el thread_main\n"); 
            yieldthread();
       }
    }
}
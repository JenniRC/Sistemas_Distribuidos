/*
	gcc -o main -Wall -Wshadow threads.c main.c && ./main

	valgrind --leak-check=full ./main
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "threads.h"	
#include <malloc.h>
#include <time.h>

void
sayhello(void *p){ 
	int i;

	for(;;){

		for(i=0; i < 32; i++){
			fprintf(stderr,"Hello from %d\n",curidthread());
						usleep(250000);
			yieldthread();
						usleep(250000);

			fprintf(stderr,"Bye from %d\n",curidthread() );
			yieldthread();
			exitsthread();

			usleep(250000);
		}
		//exitsthread();
	}
}

void
tester(void *p){

	int i;
	int arg = 1;
	static int once = 1;
	for(;;){
		if (once){
			for (i = 0; i<32; i++){
				if (createthread(sayhello, &arg, STACK_SIZE) == -1)
					fprintf(stderr, "error al crear el thread\n");
					yieldthread();
				}
			once = 0;
		}
		usleep(2500);
		yieldthread();
		exitsthread();
	}
}

int
main(int argc, char* argv[]){
		
	initthreads();
	int arg =1;
	if (createthread(tester, &arg , STACK_SIZE) == -1){
		fprintf(stderr, "error al crear el thread\n");
	}
			yieldthread();
		if (createthread(tester, &arg , STACK_SIZE) == -1){
		fprintf(stderr, "error al crear el thread\n");
	}
			yieldthread();
			exitsthread();
/*			if (createthread(tester, &arg , STACK_SIZE) == -1){
			fprintf(stderr, "error al crear el thread\n");
		}
					yieldthread();
			if (createthread(tester, &arg , STACK_SIZE) == -1){
			fprintf(stderr, "error al crear el thread\n");
		}*/
	exitsthread();
	for(;;){
		usleep(5000);
		//yieldthread();
		fprintf(stderr,"First hello from %d\n",curidthread());
		exitsthread();
	}
	exit(0);

}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

static inline unsigned long long time_ns() {
  struct timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts)) {
    exit(1);
  }
  return ((unsigned long long)ts.tv_sec) * 1000000000LLU +
         (unsigned long long)ts.tv_nsec;
}

const int NUM_ITERATIONS = 100000;

sem_t binary_semaphore;
long long unsigned t1=0,t2=0,t3=0,t4=0;
int flag = -1;

void* threadfunc_1(void* p)
{
	// printf("Hello from threadfunc_1\n");
	sem_wait(&binary_semaphore);
	t1 = time_ns(); 	
	sem_post(&binary_semaphore);
	t2 = time_ns(); 	
	flag = 2;
	// printf("value of flag from threadfunc_1 %d\n", flag);
  	return NULL;
}

void* threadfunc_2(void* p)
{
	// printf("Hello from threadfunc_2\n");
	sem_wait(&binary_semaphore);
	t3 = time_ns();
	sem_post(&binary_semaphore); 	
  	t4 = time_ns();
  	flag = 1;
	// printf("value of flag from threadfunc_2 %d\n", flag);
  	return NULL;
}

void semaphore_shuffle_time(){
	
}

int main(int argc, char** argv)
{
	printf("Running for %d iterations\n", NUM_ITERATIONS);
	
	sem_init(&binary_semaphore, 0, 1);
	pthread_t childt_1, childt_2;
  	pthread_create(&childt_1, NULL, threadfunc_1, NULL);
  	pthread_create(&childt_2, NULL, threadfunc_2, NULL);
 	
  	pthread_join(childt_1, 0);
  	pthread_join(childt_2, 0);

  	long long unsigned elapsed = 0;

 	if(flag==1){
 		// threadfun_1 executed first
 		printf("threadfun_1 executed first\n");
 		elapsed = t3-t2;
 	}else if(flag==2){
 		// threadfun_2 executed first
 		printf("threadfun_2 executed first\n");
 		elapsed = t1-t4;
 	}else {
 		printf("flag value remained -1\n");
 		exit(1);
 	}
  	
  	sem_destroy(&binary_semaphore);
	
	// printf("t1 %llu ns\n", t1);
	// printf("t2 %llu ns\n", t2);
	// printf("t3 %llu ns\n", t3);
	// printf("t4 %llu ns\n", t4);
  	printf("Semaphore Shuffling time %llu ns\n", elapsed);
	return 0;
}

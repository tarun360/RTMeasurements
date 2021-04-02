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

const int NUM_ITERATIONS = 1000;

sem_t binary_semaphore;
long long unsigned t1=0,t2=0,t1_prev=0;
int flag = -1;

void* threadfunc_1(void* p)
{
	while(1){
		if(flag==1)
			break;
		t1_prev = t1;
		t1 = time_ns(); 	
	}
	return NULL;
}

void* threadfunc_2(void* p)
{
	t2 = time_ns();
	flag = 1;
	return NULL;
}

long long unsigned semaphore_shuffle_time(){

	pthread_t childt_1, childt_2;
  	pthread_create(&childt_1, NULL, threadfunc_1, NULL);
  	sleep(0.001);
  	pthread_create(&childt_2, NULL, threadfunc_2, NULL);
 	
  	pthread_join(childt_1, 0);
  	pthread_join(childt_2, 0);

  	long long unsigned elapsed = 0;

 	if(flag==1){
 		if(t2 > t1)
 			elapsed = t2-t1;
 		else
 			elapsed = t2-t1_prev;
 	}else{
 		printf("flag value remained -1\n");
 		exit(1);
 	}
  	printf("%llu\n", elapsed);
  	return elapsed;
}

int main(int argc, char** argv)
{
	printf("Running for %d iterations\n", NUM_ITERATIONS);
	
	long long unsigned total_elapsed=0;
	for(int i=0;i<NUM_ITERATIONS;i++){
		total_elapsed += semaphore_shuffle_time();
	}

	double avg_task_switching_time = (double)total_elapsed/NUM_ITERATIONS;

	printf("%.1f\n", avg_task_switching_time);
	return 0;
}

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

int fd[2];//File descriptor for creating a pipe
long long unsigned tw[100000], te[100000];
//This function continously reads fd[0] for any input data byte
//If available, prints

void *reader()
{
    for(int i=0; i<100000; i++){
        long long unsigned twi, tr;
        char ch;
        int     result;

        result = read (fd[0],&ch,1);
        tr = time_ns();
        if (result != 1) {
            perror("read");
            exit(3);
        }
        long long unsigned t_elapsed = tr-tw[i];
        te[i] = t_elapsed;
        // printf ("Reader: %llu, Writer: %llu, Time elapsed: %llu\n", tr, tw[i], t_elapsed);   }
    }
    pthread_exit(reader);
}

//This function continously writes Alphabet into fd[1]
//Waits if no more space is available

void *writer()
{
    int     result;
    long long unsigned twi;
    char ch = 'A';
    // sleep(0.001);
    for(int i=0; i<100000; i++){
        twi = time_ns();
        tw[i] = twi;
        result = write (fd[1], &ch,1);
        if (result != 1){
            perror ("write");
            exit (2);
        }
        // printf ("Writer: %llu\n", twi);
    }
    pthread_exit(writer);
}

int main()
{
   pthread_t       tid1,tid2;
   int             result;

   result = pipe (fd);
   if (result < 0){
       perror("pipe ");
       exit(1);
   }

   pthread_create(&tid1,NULL,reader,NULL);
   pthread_create(&tid2,NULL,writer,NULL);

   pthread_join(tid1,NULL);
   pthread_join(tid2,NULL);
   long long unsigned total = 0;
    for(int i=0; i<100000; i++){
    //    printf("%llu\n", te[i]);
        total += te[i];
    }
    double avg = (double)total/100000;
    printf("average %.1f\n", avg);
}

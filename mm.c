#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include "barrier.h"
#define NUM_PROC 4

void randwait(int secs) {
  int x = (int) (secs*drand48()+ 1);
  usleep(x);
}

int main() {
  
  Barrier * barr = (Barrier*) malloc(sizeof(Barrier));
  char semName[6] = "BarSe";
  char mutexName[7] = "BarMut"; 
  

  strcpy(barr->semName, semName);
  barr->size = NUM_PROC;
  barr->shmKey = 2000;
	strcpy(barr->mutexName,mutexName);
  initBarrier(barr);
  
  pid_t child_pid, wpid;
  int status = 0;

  for (int id=0; id<NUM_PROC; id++) {
    if ((child_pid = fork()) == 0) {
        printf("The process %d begins\n", id);
        randwait(50); // multiplicacion
        // llegué a la cima
        printf("Process %d reached the checkpoint\n", id);
        waitBarrier(barr);
        printf("I am free \n");
        // después de la barrera
        exit(0);
    }
  }
  
  while ((wpid = wait(&status)) > 0);
  printf("All processes end\n");
  
  destroyBarrier(barr);
  return 0;
}
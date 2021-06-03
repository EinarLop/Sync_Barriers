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
  char semName[6] = "Einar";
  char mutexName[7] = "EinarM";

  strcpy(barr->semName, semName);
  barr->size = NUM_PROC;
  barr->shmKey = 2000;
	strcpy(barr->mutexName,mutexName);
  initBarrier(barr);
  
  pid_t child_pid, wpid;
  int status = 0;

  for (int id=0; id<NUM_PROC; id++) {
    if ((child_pid = fork()) == 0) {
        printf("COMIENZA proceso %d\n", id);
        randwait(50); // multiplicacion
        // llegué a la cima
        printf("LLEGÓ a la cima proceso %d\n", id);
        waitBarrier(barr);
        printf("CONTINÚA proceso %d\n", id);
        // después de la barrera
        exit(0);
    }
  }
  
  while ((wpid = wait(&status)) > 0);
  printf("Terminaron todos los procesos. Padre existe\n");
  
  destroyBarrier(barr);
  return 0;
}
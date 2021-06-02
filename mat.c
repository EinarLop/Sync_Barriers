#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "mc.h"
#include "barrier.h"

#define M_LEN 2

void randwait(int secs) {
  int x = (int) (secs*drand48()+ 1);
  usleep(x);
}

void writeToResult(int key, int value, int pos);
int dotProduct(int rowVector[2], int colVector[2]);

int main() {
  int KEY = 3000;     // para memoria compartida
  int vector[2] = {5, -1};
  
  int matrix[2][2] = {{2,4},{7,-1}};
  
  pid_t child_pid, wpid;
  int status = 0;
  char semName[6] = "Einar";
  char mutexName[7] = "EinarM";

  Barrier* barr = malloc(sizeof(Barrier));
  
  strcpy(barr->semName, semName);
  barr->size = M_LEN;
  
  barr->shmKey = 2000;
  initBarrier(barr);

  for (int id=0; id<M_LEN; id++) {
    if ((child_pid = fork()) == 0) {
        printf("Renglón %d\n", id);
        randwait(50); // multiplicacion
        // llegué a la cima
        printf("Terminó multi %d\n", id);
        int val = dotProduct(matrix[id], vector);
        writeToResult(KEY, val, id);
        waitBarrier(barr);
        // print Resultante
        exit(0);
    }
  }

  while ((wpid = wait(&status)) > 0);
  printf("All processes done. Parent exits\n");
  destroyBarrier(barr);
  return 0; 
}


int dotProduct(int rowVector[2], int colVector[2]){
  int result = rowVector[0]*colVector[0] + rowVector[1]*colVector[1];
  return result;
}

void writeToResult(int key, int value, int pos){
	int shmid1;
  if ((shmget(key, sizeof(resultVector), IPC_CREAT | S_IRWXU)) < 0 ) {
    perror("Error en shmget\n");
    return;
  }
  resultVector  *rv = (resultVector*) shmat(shmid1, 0, 0);
  if (rv == (void *) -1) {
    perror("Error en shmat\n");
    return;
  }
  rv->vector[pos] = value;

  printf("Termine de escribir en la pos %d\n", pos);
}



int cleanMemory(int key){
   
  int shmid;
  if (( shmid = shmget(key,  sizeof(resultVector), S_IRWXU)) < 0 ) {
    perror("Error en el shmget 1");
    return 1;
  }
  struct shmid_ds ds;
  if (shmctl(shmid, IPC_RMID, &ds) < 0) {
    perror("Error al eliminar el segmento de memoria compartida 1");
    return 1;
  }
  
  return 0;
  
}
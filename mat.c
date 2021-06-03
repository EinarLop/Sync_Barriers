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

#define M_LEN 3

void randwait(int secs) {
  int x = (int) (secs*drand48()+ 1);
  usleep(x);
}

void writeToResult(int key, int value, int pos);
int dotProduct(int rowVector[2], int colVector[2]);
int cleanMemory(int key);

int main() {
  int KEY = 3000;     // para memoria compartida
  int vector[3] = {5, -1, 1};
  
  int matrix[3][3] = {{2,4,8},{7,-1,5},{9,3,-6}};
  
  pid_t child_pid, wpid;
  int status = 0;
  char semName[6] = "Semaf";
  char mutexName[7] = "SemafB";

  int shmid1;
  if ( (shmid1 = shmget(KEY, sizeof(resultVector), IPC_CREAT | S_IRWXU)) < 0 ) {
    perror("Error en creacion al_ shmget");
    return 1;
  }
 
  resultVector *rv = (resultVector*) shmat(shmid1, 0, 0);
  if (rv == (void *) -1) {
    perror("error en shmat1");
    return 1;
  }

  rv->vector[0]= 0;
  rv->vector[1]= 0;
  rv->vector[2]= 0;

  printf("Initial vector is <%d %d %d>\n", rv->vector[0], rv->vector[1], rv->vector[2]);
  Barrier* barr = malloc(sizeof(Barrier));
  
  strcpy(barr->semName, semName);
	strcpy(barr->mutexName, mutexName);
  barr->size = M_LEN;
  
  barr->shmKey = 2000;
  initBarrier(barr);

  for (int id=0; id<M_LEN; id++) {
    if ((child_pid = fork()) == 0) {
        printf("Renglón %d\n", id);
        int val = dotProduct(matrix[id], vector);
        printf("dotproduct:%d\n", val);
        writeToResult(KEY, val, id);
        printf("Terminó multi %d\n", id);
        waitBarrier(barr);
        printf("RESULTADO \n");
        exit(0);
    }
  }
  
  while ((wpid = wait(&status)) > 0);
  printf("Todos los proceso acabaron. Proces padre sale.\n");
	printf("Final vector is <%d %d %d>\n", rv->vector[0], rv->vector[1], rv->vector[2]);
  destroyBarrier(barr);
  cleanMemory(KEY);
  return 0; 
}


int dotProduct(int rowVector[M_LEN], int colVector[M_LEN]){
  int sum=0;

  for (int x=0; x<M_LEN; x++) {
    sum += rowVector[x] * colVector[x];
  }

  // int result = rowVector[0]*colVector[0] + rowVector[1]*colVector[1] + rowVector[2]*colVector[3];

	return sum;
}

void writeToResult(int key, int value, int pos){
	int shmid1;
  if ((shmid1 = shmget(key, sizeof(resultVector), IPC_CREAT | S_IRWXU)) < 0 ) {
    perror("Error en shmget de resultVector\n");
    return;
  }
  resultVector  *rv = (resultVector*) shmat(shmid1, 0, 0);
  if (rv == (void *) -1) {
    perror("Error en shmat de resultVector\n");
    return;
  }
  
  rv->vector[pos] = value;

  printf("Termine de escribir en la pos %d\n", pos);

}


int cleanMemory(int key){
  printf("Cleaning memory...\n");
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


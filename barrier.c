#include "barrier.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>         
#include <sys/stat.h>       
#include <semaphore.h>
#include <stdlib.h>

/*
INTEGRANTES:
*Bryan Alexis Monroy Álvarez A01026848   
*Eric Jardón Chao A01376748 
*Einar López Altamirano A01656259
*Georgina Alejandra Gámez Melgar A01656818
*/

int initBarrier(Barrier *barrier){
  printf("\tInicializando barrier tamaño %d\n", barrier->size);
  sem_unlink(barrier->semName);
  //sem_unlink(barrier->mutexName);

  int KEY = barrier->shmKey; 
  sem_t *sem;   // para hacer esperar a los procesos
  sem_t *mutex; // para count

  // Crear memoria compartida e inicializar su valor (count).
  int shmid1;
  if ( (shmid1 = shmget(KEY, sizeof(compartir), IPC_CREAT | S_IRWXU)) < 0 ) {
    perror("Error en shmget");
    return 1;
  }

  compartir *cmp = (compartir*) shmat(shmid1, 0, 0);
  if (cmp == (void *) -1) {
    perror("Error en shmat");
    return 1;
  }

  cmp->count = barrier->size;
  // inicializar los semáforos. 
  
  sem = sem_open(barrier->semName, O_CREAT, S_IRWXU, 0);
  if (sem==SEM_FAILED) {
    printf("\tSemaphore fallo semaforo\n");
    exit(EXIT_FAILURE);
  }

  /* mutex = sem_open(barrier->mutexName, O_CREAT, S_IRWXU, 1);
  if (mutex ==SEM_FAILED) {
    printf("\tMutex fallo mutex\n");
    exit(EXIT_FAILURE);
  } */
  
  sem_close(sem);
  //sem_close(mutex);
	return 0;
}

int waitBarrier(Barrier* barrier){
   
  sem_t *sem;
  //sem_t *mutex;

  sem = sem_open(barrier->semName, 0);
	int KEY = barrier->shmKey;
  
  // abrir memoria compartida y aumentar el contador
  int shmid1;
  if ( (shmid1 = shmget(KEY, sizeof(compartir), IPC_CREAT | S_IRWXU)) < 0 ) {
    perror("Error en shmget");
    return 1;
  }
  
  compartir *cmp = (compartir*) shmat(shmid1, 0, 0);
  if (cmp == (void *) -1) {
    perror("Error en shmat");
    return 1;
  }

  (cmp->count)--;
  printf("\tFaltan de llegar %d\n", cmp->count);

	if(cmp->count==0){
    // printf("\tAll processes arrived. Continue\n");
    for(int i =0;i<barrier->size;i++){
			sem_post(sem); //Libera semaforo a todos
  	}  
	} else {
    sem_wait(sem);
    // liberó la barrera
	}
  sem_close(sem);
  //sem_close(mutex);
	return 0;
}


int destroyBarrier(Barrier* barrier){
  // Limpiar memoria compartida
  // Borrar semáforos
  int KEY = barrier->shmKey; 
  int shmid;
  if (( shmid = shmget(KEY,  sizeof(compartir), S_IRWXU)) < 0 ) {
    perror("Error en el shmget 1");
    return 1;
  }

  struct shmid_ds ds;
  
  if (shmctl(shmid, IPC_RMID, &ds) < 0) {
    perror("Error al eliminar el segmento de memoria compartida 1");
    return 1;
  }

  sem_unlink(barrier->semName);
  //sem_unlink(barrier->mutexName);

	return 0;
}

/*
int pthread_barrier_init(pthread_barrier_t * barrier, const pthread_barrierattr_t * attr, unsigned count);

// El proceso llamante es bloqueado hasta que `count` procesos hayan llamado a wait.
int pthread_barrier_wait(pthread_barrier_t * barrier);

// Liberar todos los recursos utilizados por la barrera
int pthread_barrier(pthread_barrier_t* barrier);
*/

/*
Semaforos -> numero entero, puede ser +  -
wait -> decrementa , si <0, proceso SE BLOQUEA, si no, continúa sin pausar.
-N
count (memoria compartida) == barrier.size
post -> incrementa, 

sem_post N veces, hasta llegar a positivo, -> semaforo regrese a positivo, y se liberan todos

*/
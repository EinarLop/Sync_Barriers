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



// Creacion del semaforo
// #define SEM_1 "procesos"
// sem_t *semaforo1;
// semaforo1 = sem_open(SEM_1,O_CREAT,S_IWGRP,num_procesos);
// sem_close(semaforo1);
// sem_unlink(semaforo1);
// sem_wait(semaforo2);
// sem_post(semaforo2);

/*
pthread_mutex_trylock(&mutex);
counter++;
pthread_mutex_unlock(&mutex);
*/

int initBarrier(Barrier *barrier){
  sem_unlink(barrier->semName);
  sem_unlink(barrier->mutexName);

  printf("\tInside initBarrier\n");
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
  printf("\tInit barr count: %d\n", cmp->count);
  // inicializar los semáforos. 
  
  sem = sem_open(barrier->semName, O_CREAT, S_IRWXU, 0);
  if (sem==SEM_FAILED) {
    printf("Semaphore Chinga tu madrísima ERICANA\n");
    exit(EXIT_FAILURE);
  }

  mutex = sem_open(barrier->mutexName, O_CREAT, S_IRWXU, 1);
  if (mutex ==SEM_FAILED) {
    printf("Mutex Chinga tu madrísima ERICANA\n");
    exit(EXIT_FAILURE);
  }
  
  printf("\tSe crearon el semaforo y el mutex exitosamente\n");
  sem_close(sem);
  sem_close(mutex);
	return 0;
}

int waitBarrier(Barrier* barrier){
   
  sem_t *sem;
  sem_t *mutex;

  sem = sem_open(barrier->semName, 0);
  mutex = sem_open(barrier->mutexName, 0);

  // int currSem;
  // int currMutex;
  // sem_getvalue(sem, &currSem);
  // sem_getvalue(mutex, &currMutex);
  // printf("\tthis is sem %d\n", currSem);
  // printf("\tthis is mutex %d\n", currMutex);
  
  printf("\tSe abrieron correctamente el semaforo y mutex\n");
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

  printf("count is %d",cmp->count);
  --(cmp->count);
  printf("\tDecrement shared count %d\n", cmp->count);

	if(cmp->count==0){
    printf("\tAll processes arrived. Continue\n");
		printf("Barrier size %d\n ",barrier->size);
    for(int i =0;i<barrier->size;i++){
			printf("%d\n",i);
			sem_post(sem); //Libera semaforo a todos
			int currSem;
			int currMutex;
			sem_getvalue(sem, &currSem);
			sem_getvalue(mutex, &currMutex);
			printf("\tthis is sem %d\n", currSem);
			printf("\tthis is mutex %d\n", currMutex);
	  }
	} else {
		printf("\tLlegó un proceso\n");
    sem_wait(sem);	
    // espero a los demás
    printf("Terminé de esperar\n");
	}
  sem_close(sem);
  sem_close(mutex);
	printf("SOY LIBREEEE\n");
 
	return 0;
}


int clearBarrier(Barrier* barrier){
  sem_t *sem;
  sem_t *mutex;
  sem = sem_open( barrier->semName, O_CREAT, S_IWGRP,0);
  mutex = sem_open(barrier->mutexName, O_CREAT, S_IWGRP,1);
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
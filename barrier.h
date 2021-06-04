#ifndef BARRIER_H
#define BARRIER_H

typedef struct Barrier {
  int size; // procesos
  int shmKey; // llave identificadora a la ShM
  char semName[20];  // semaphore name
	char mutexName[21];
} Barrier;

typedef struct compartir {
  int count; // protegido por mutex, pase de lista
} compartir;

int waitBarrier(Barrier* barrier);

int initBarrier(Barrier* barrier);

int destroyBarrier(Barrier* barrier); // libera recursos y  limpia memoria compartida


#endif
#include <stdio.h>
#include <string.h>
#include "mc.h"

/* para memmoria compartida */
#include <sys/shm.h>
#include <sys/stat.h>

int main(int argc, const char * argv[]) {
  /* uso programa palabra_buscar, lista de palabras separadas por espacios
  ./crea hola hola adios adios adios hola
  char* palabras[] = {"hola", "hola", "adios", "adios", "adios", "hola"}; */

  /* crea memoria compartida */
    int shmid1, shmid2;
  if ( (shmid1 = shmget(KEY1, sizeof(compartir), IPC_CREAT | S_IRWXU)) < 0 ) {
    perror("Error en creacion al_ shmget");
    return 1;
  }
 
  
  compartir *cmp = (compartir*) shmat(shmid1, 0, 0);
  if (cmp == (void *) -1) {
    perror("error en shmat1");
    return 1;
  }
  
  // Inicializar valores
  cmp->inicio = 0;
  cmp->fin = argc-2; //sizeof(palabras)/sizeof(char*);
  cmp->mitad = cmp->fin/2;
  strcpy(cmp->palabra, argv[1]);

  // Asignar espacio de shmid2: número de palabras * 50 chars
  if ( (shmid2 = shmget(KEY2, cmp->fin*50, IPC_CREAT | S_IRWXU)) < 0 ) {
    perror("Error en creacion al_ shmget");
    return 1;
  }

  // Obten apuntador a shmid2
  char *mem2 = (char*) shmat(shmid2, 0, 0);
  if (mem2 == (void*) -1) {
    perror("error en shmat2");
    return 1;
  }

  // Copia las palabras del arreglo a buscar a shmid2
  for(int i = 0; i < cmp->fin; i++) {
    strcpy(mem2, argv[i+2]);
    mem2 += 50;
  }

  return 0;
}

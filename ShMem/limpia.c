#include <stdio.h>
#include "mc.h"

/* para memmoria compartida */
#include <sys/shm.h>
#include <sys/stat.h>

int main(int argc, const char * argv[]) {
  /* eliminar memoria compartida */
  int shmid;
  if (( shmid = shmget(KEY1,  sizeof(compartir), S_IRWXU)) < 0 ) {
    perror("Error en el shmget 1");
    return 1;
  }
  struct shmid_ds ds;
  if (shmctl(shmid, IPC_RMID, &ds) < 0) {
    perror("Error en padre al ejecutar shmctl al eliminar el segmento de memoria compartida 1");
    return 1;
  }
  
  if (( shmid = shmget(KEY2,  sizeof(char*), S_IRWXU)) < 0 ) {
    perror("Error en el shmget 2");
    return 1;
  }
  if (shmctl(shmid, IPC_RMID, &ds) < 0) {
    perror("Error en padre al ejecutar shmctl al eliminar el segmento de memoria compartida 2");
    return 1;
  }
  
  return 0;
}

#include <stdio.h>
#include <string.h>
#include "mc.h"

/* para memmoria compartida */
#include <sys/shm.h>
#include <sys/stat.h> 

int main(int argc, const char * argv[]) {
  int shmid1;
  if ( (shmid1 = shmget(KEY1, sizeof(compartir), S_IRWXU)) < 0 ) {
    perror("Error en el shmget1");
    return 1;
  }
  
  compartir *cmp = shmat(shmid1, 0, 0);
  if (cmp == (void *) -1) {
    perror("error en shmat1");
    return 1;
  }

printf("Total = %d\n", cmp->contador1 + cmp->contador2);
}

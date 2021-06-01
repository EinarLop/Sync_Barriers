#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mc.h"

/* para memmoria compartida */
#include <sys/shm.h>
#include <sys/stat.h>

int busca(char* palabras, char* palabra, int inicio, int fin, char* quien) {
  int resultado = 0;
  for(int i = inicio; i < fin; i++) {
    usleep(10);
    if (strcmp(palabra, palabras + (i * 50)) == 0) {
      printf("encontrada %d, %s\n", i, quien);
      resultado++;
    }
  }
  return resultado;
}

int main(int argc, const char * argv[]) {
  int shmid1, shmid2;
  if ( (shmid1 = shmget(KEY1, sizeof(compartir), S_IRWXU)) < 0 ) {
    perror("Error en el shmget1");
    return 1;
  }
  compartir *cmp = shmat(shmid1, 0, 0);
  if (((int) cmp) == -1) {
    perror("error en shmat1");
    return 1;
  }

  if ( (shmid2 = shmget(KEY2, sizeof(char*)*cmp->fin, S_IRWXU)) < 0 ) {
    perror("Error en el shmget2");
    return 1;
  }

  char *palabras = shmat(shmid2, NULL, 0);
  
  if (((int) palabras) == -1) {
    perror("error en shmat2");
    return 1;
  }

  printf("La palabra a buscar es %s, y hay %d palabras \n", cmp->palabra, cmp->fin);
  for(int i = 0; i < cmp->fin; i++) {
    printf("%s\n", palabras + (i * 50));
  }
  
  if(strcmp(argv[1],"0") == 0) {
    cmp->contador1 = busca(palabras, cmp->palabra,cmp->inicio,cmp->mitad,"inf");
  } 
  else {
    cmp->contador2 = busca(palabras, cmp->palabra,cmp->mitad,cmp->fin,"sup");
  }
  return 0;
}

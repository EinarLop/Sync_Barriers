#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, const char * argv[]) {
  
  pid_t pid1, pid2;
  if ((pid1= fork()) == 0) {
    execlp("./cuenta1","./cuenta1", "0", NULL); 
  }

  else if ((pid2 = fork()) == 0) {
    execlp("./cuenta1","./cuenta1", "1", NULL); 
  }

  int status;
  waitpid(pid1, &status, 0);
  waitpid(pid2, &status, 0);
  execlp("./lee","./lee", NULL); 
  
}
 

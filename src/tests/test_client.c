#include"../client.h"

int main(int argc, char** argv){
  char buf[8];
  int sfd[3];
  
  for(int i = 0; i < 3; i++){ 
    sfd[i] = connection("127.0.0.1");
    printf("%d\n", sfd[i]);
  }
  
  recv(sfd[0], buf, sizeof(buf), 0);

  /* PRINT(buf); */

  return EXIT_SUCCESS;
}

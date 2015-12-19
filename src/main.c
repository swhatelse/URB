#include<stdio.h>
#include<stdlib.h>

#include"common.h"

struct Group{
  struct sock_addr_in* connected_fella;
};

int main(int argc, char *argv[]){
  if(argc < 2){
    perror("arguments missing");
    return EXIT_FAILURE;
  }
  
  join("host.txt");
  return EXIT_SUCCESS;
}

#include"../server.h"

int main(int argc, char** argv){
  connexion_init();
  connexion_handler();
  return EXIT_SUCCESS;
}

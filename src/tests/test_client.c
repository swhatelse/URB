#include<getopt.h>
#include"../client.h"

int main(int argc, char** argv){
    int opt;
    char* config;
    char buf[8];
    int sfd[3];
  
    if(argc < 2){
        perror("Argument missing");
        exit(EXIT_FAILURE);
    }
  
    while( (opt = getopt(argc, argv, "f:") ) != -1){
        switch(opt){
        case 'f':
            config = optarg;
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    join(config);
  
    recv(sfd[0], buf, sizeof(buf), 0);

    /* PRINT(buf); */

    return EXIT_SUCCESS;
}

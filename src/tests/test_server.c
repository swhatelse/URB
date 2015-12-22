#include<getopt.h>
#include"../server.h"

int main(int argc, char** argv){
    int opt;
    int port;
    if(argc < 2){
        perror("Argument missing");
        exit(EXIT_FAILURE);
    }
  
    while( (opt = getopt(argc, argv, "p:") ) != -1){
        switch(opt){
        case 'p':
            port = atoi(optarg);
            break;
        default:
            exit(EXIT_FAILURE);
        }
    }

    connexion_init(port);
    connexion_handler();
    return EXIT_SUCCESS;
}

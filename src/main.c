#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<getopt.h>

#include"server.h"
#include"common.h"

/* struct Group{
  struct sock_addr_in* connected_fella;
  };*/

int main(int argc, char *argv[]){
    int opt;
    int port;
    /* pthread_t tsid, tcid; */
    pthread_t tsid;
    /* char* config; */

    if(argc < 3){
        fprintf(stderr,"arguments missing");
        return EXIT_FAILURE;
    }

    while( (opt = getopt(argc, argv, "p:f:") ) != -1){
        switch(opt){
        case 'f':
            /* config = optarg; */
            break;
        case 'p':
            port = atoi(optarg);
            break;
        default:
            return EXIT_FAILURE;
        }
    }
  
    connexion_init(port);
    pthread_create(&tsid,NULL,&connexion_handler, NULL);
    return EXIT_SUCCESS;
}

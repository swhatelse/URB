#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<getopt.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include"listener.h"
#include"group.h"
#include"common.h"
#include"communication.h"

int main(int argc, char *argv[]){
    int opt;
    /* pthread_t tsid, tcid; */
    pthread_t tsid, csid;
    char* hostfile = NULL;
    int port = 0;

    if(argc < 3){
        fprintf(stderr,"arguments missing");
        return EXIT_FAILURE;
    }

    while( (opt = getopt(argc, argv, "hp:f:") ) != -1){
        switch(opt){
		case 'h':
		  fprintf(stderr,"Usage: ./main -f [hostfile] -p [port]");
            break;
        case 'f':
            hostfile = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    // TODO: make it proper
    init(hostfile, "127.0.0.1", port);
    listener_init();
    pthread_create(&tsid,NULL,&listener_run, NULL);
    sleep(1);
    pthread_create(&csid,NULL,&message_handler, NULL);
    pthread_join(csid, NULL);
    message_t msg;
    msg.type = 'M';
    beb(msg);
    pthread_join(tsid, NULL);

    return EXIT_SUCCESS;
}

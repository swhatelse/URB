#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<getopt.h>
#include<string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include"server.h"
#include"common.h"

/** Initialize the structure of the group
 * @param file Name of the file containing the hosts. Host format must be in A.B.C.D:port
 */
int init(char *file, char *my_addr, int my_port){
    int group_count = 0;
    FILE *fd;
    char buf[16];
    int i = 0;
    char *addr;
    int port;
    char sep = ':';

    fd = fopen(file, "r");
    
    while(fgets(buf, sizeof(buf), fd) != NULL){
        group_count++;
    }

    send_sockets.count = group_count - 1;
    receive_sockets.count = group_count - 1;
    send_sockets.nodes = calloc(send_sockets.count, sizeof(node_t));
    receive_sockets.nodes = calloc(receive_sockets.count, sizeof(node_t)); 

    rewind(fd);

    while(fgets(buf, sizeof(buf), fd) != NULL){
        addr = strtok(buf, &sep);
        port = atoi(strtok(NULL, &sep));
        if( port != my_port && strcmp(addr,my_addr) != 0){
            (send_sockets.nodes[i]).infos.sin_family = AF_INET;
            (send_sockets.nodes[i]).infos.sin_port = htons(port);
            (send_sockets.nodes[i]).infos.sin_addr.s_addr = inet_addr(addr);
            i++;
        }
    }

    fclose(fd);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
    int opt;
    int port;
    /* pthread_t tsid, tcid; */
    pthread_t tsid;
    char* hostfile;

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
    init(hostfile,"127.0.0.1", port);
    connexion_init(port);
    pthread_create(&tsid,NULL,&connexion_handler, NULL);
    
    pthread_join(tsid, NULL);

    return EXIT_SUCCESS;
}

#include"common.h"

#define NODE_COORDINATE_SIZE 32

unsigned short get_my_port(){
    return ntohs(my_addr.sin_port);
}

char* get_my_addr(){
    return inet_ntoa(my_addr.sin_addr);
}

int get_my_id(){
    return my_id;
}

void set_my_port(unsigned short port){
    my_addr.sin_port = htons((port));
}

/* void set_my_addr(int addr){ */
/*     my_addr.sin_addr.s_addr = ; */
/* } */


void set_my_id(int id){
    my_id = id;
}

/** Initialize the structures of the group
 * @param file Name of the file containing the hosts. Host format must be in A.B.C.D:port
 */
int init(char *file, char *my_addr, int port){
    int group_count = 0;
    FILE *fd;
    char buf[NODE_COORDINATE_SIZE];
    int i = 0;
    char *addr;
    int remote_port;
    int node_id;
    char sep = ':';

    assert(file != NULL);
    assert(strcmp(file, "") != 0);
    assert(my_addr != NULL);
    assert(port != 0);
    assert(strcmp(my_addr, "") != 0);

    set_my_port(port);

    DEBUG("Begin of initialization\n");
    DEBUG("Address : %s\n", my_addr);
    DEBUG("Listening port : %d\n", port);
    
    pthread_mutex_init(&send_sockets_mtx, NULL);
    pthread_mutex_init(&receive_sockets_mtx, NULL);
    
    fd = fopen(file, "r");
    
    while(fgets(buf, sizeof(buf), fd) != NULL){
        group_count++;
    }

    send_sockets.count = group_count - 1;
    receive_sockets.count = group_count - 1;
    send_sockets.nodes = calloc(send_sockets.count, sizeof(node_t*));
    receive_sockets.nodes = calloc(receive_sockets.count, sizeof(node_t*));

    // Just initialize the groups
    for(int i = 0; i < group_count - 1; i++){
        send_sockets.nodes[i] = NULL;
        receive_sockets.nodes[i] = NULL;
    }

    rewind(fd);

    while(fgets(buf, sizeof(buf), fd) != NULL){
        addr = strtok(buf, &sep);
        remote_port = atoi(strtok(NULL, &sep));
        node_id = atoi( strtok(NULL, &sep) );
        if( remote_port != port || strcmp(addr,my_addr) != 0){
            // Fill the connecting sockets
            send_sockets.nodes[i] = malloc(sizeof(node_t));
            send_sockets.nodes[i]->connexion = malloc(sizeof(connexion_t));
            send_sockets.nodes[i]->connexion->infos.sin_family = AF_INET;
            send_sockets.nodes[i]->connexion->infos.sin_port = htons(remote_port);
            send_sockets.nodes[i]->connexion->infos.sin_addr.s_addr = inet_addr(addr);
            send_sockets.nodes[i]->id = node_id;
            send_sockets.nodes[i]->active = false;

            // Pre-fill the listening sockets
            receive_sockets.nodes[i] = malloc(sizeof(node_t));
            receive_sockets.nodes[i]->id = node_id;
            i++;
        }
        else{
            DEBUG("Node id : %d\n", node_id);
            set_my_id(node_id);
        }
    }

    fclose(fd);

    return EXIT_SUCCESS;
}

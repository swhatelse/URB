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

void set_my_id(int id){
    my_id = id;
}

/** Initialize the structures of the group
 * @param file Name of the file containing the hosts. Host format must be in A.B.C.D:port
 */
int init(char *file, char *my_addr, int port){
    assert(file != NULL);
    assert(strcmp(file, "") != 0);
    assert(my_addr != NULL);
    assert(port != 0);
    assert(strcmp(my_addr, "") != 0);

    int group_count = 0;
    FILE *fd;
    char buf[NODE_COORDINATE_SIZE];
    int i = 0;
    char *addr;
    int remote_port;
    int node_id;
    char sep = ':';

    set_my_port(port);

    DEBUG("Begin of initialization\n");
    DEBUG("Address : %s\n", my_addr);
    DEBUG("Listening port : %d\n", port);
    
    pthread_mutex_init(&send_sockets_mtx, NULL);
    pthread_mutex_init(&receive_sockets_mtx, NULL);
    
    fd = fopen(file, "r");

    // Get the size of the group by couting line of the hostfile.
    while(fgets(buf, sizeof(buf), fd) != NULL){
        group_count++;
    }

    /* send_sockets.count = group_count - 1; */
    /* receive_sockets.count = group_count - 1; */
    /* send_sockets.nodes = calloc(send_sockets.count, sizeof(node_t*)); */
    /* receive_sockets.nodes = calloc(receive_sockets.count, sizeof(node_t*)); */

    // New
    group = g_hash_table_new(g_int_hash, g_int_equal);
    
    // Just initialize the groups
    /* for(int i = 0; i < group_count - 1; i++){ */
    /*     send_sockets.nodes[i] = NULL; */
    /*     receive_sockets.nodes[i] = NULL; */
    /* } */

    rewind(fd);

    while(fgets(buf, sizeof(buf), fd) != NULL){
        addr = strtok(buf, &sep);
        remote_port = atoi(strtok(NULL, &sep));
        node_id = atoi( strtok(NULL, &sep) );

        if( remote_port != port || strcmp(addr,my_addr) != 0){
            node_t* peer = node_create(NULL);
            peer->id = node_id ;
            peer->outbox = connexion_create(addr,remote_port);
            g_hash_table_insert(group, &peer->id, peer);
            /* // Fill the connecting sockets */
            /* send_sockets.nodes[i] = node_create(NULL); */
            /* send_sockets.nodes[i]->id = node_id; */
            /* send_sockets.nodes[i]->connexion = connexion_create(addr,remote_port); */
            /* // Pre-fill the listening sockets */
            /* receive_sockets.nodes[i] = node_create(NULL); */
            /* receive_sockets.nodes[i]->id = node_id; */
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

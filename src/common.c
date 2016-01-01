#include"common.h"
#include"group.h"

/** Initialize the structures of the group
 * @param file Name of the file containing the hosts. Host format must be in A.B.C.D:port
 */
int init(char *file, char *my_addr, int port){
    int group_count = 0;
    FILE *fd;
    char buf[16];
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

    my_port = port;

    PRINT("Begin of initialization");
    
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
        node_id = atoi(strtok(NULL, &sep));
        if( remote_port != my_port || strcmp(addr,my_addr) != 0){
            send_sockets.nodes[i] = malloc(sizeof(node_t));
            (send_sockets.nodes[i])->connexion.infos.sin_family = AF_INET;
            (send_sockets.nodes[i])->connexion.infos.sin_port = htons(remote_port);
            (send_sockets.nodes[i])->connexion.infos.sin_addr.s_addr = inet_addr(addr);
            send_sockets.nodes[i]->id = node_id;
            i++;
        }
    }

    fclose(fd);

    return EXIT_SUCCESS;
}



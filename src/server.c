#include"server.h"

#define BACKLOG 50
#define PRINT(x)                                \
    printf("Server : %s\n", x)

int sfd; // listening socket for connexion
group_t reception_sockets;
node_t *first_node = NULL;

void connexion_init(int port){
    struct sockaddr_in my_addr;
	// TODO here just for the moment
	reception_sockets.nodes = NULL;

    sfd = socket(AF_INET, SOCK_STREAM,0);
    if(sfd < 0){
        perror("Failed to attribute the socket\n");
        exit(EXIT_FAILURE);
    }
  
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sfd, (struct sockaddr*) &my_addr, sizeof(my_addr)) < 0 ){
        perror("Failed to name the socket\n");
        exit(EXIT_FAILURE);    
    }

    if(listen(sfd, BACKLOG) < 0){
        perror("Failed to listen\n");
        exit(EXIT_FAILURE);    
    }
}

void* connexion_handler(){
	while(1){
        connexion_accept();
    }
    return NULL;
}

int insert_node(const int fd, const struct sockaddr_in peer_addr){
    node_t* n = malloc(sizeof(node_t));
    node_t* current;
  
    n->fd = fd;
    n->infos = peer_addr;

    if(first_node == NULL){
        first_node = n;
        n->next = NULL;
        n->prev = NULL;
        return EXIT_SUCCESS;
    }
    else{
        current = first_node;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = n;
        n->prev = current;
        n->next = NULL;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

/** Add a node to the receiving list
 * @param fd File descriptor associated to the addr
 * @param addr Address information of the incomming connexion
 * @return 0 if ok 1 if fails
 */
int add_node(const int fd, struct sockaddr_in addr){
    for(int i = 0; i < receive_sockets.count; i++){
        if(receive_sockets.nodes[i].infos.sin_addr.s_addr == 0){
            receive_sockets.nodes[i].infos = addr;
            receive_sockets.nodes[i].fd = fd;
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

int remove_node(int fd){
    node_t* current;
    current = first_node;
    while(current != NULL){
        if(current->fd == fd){
            if(current->prev != NULL){
                current->prev->next = current->next;
            }
            if(current->next != NULL){
                current->next->prev = current->prev;
            }
            return EXIT_SUCCESS;
        }
        current = first_node->next;
    }
    
    return EXIT_FAILURE;
}

int connexion_accept(){
    int cfd;
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size;
  
    cfd = accept(sfd, (struct sockaddr*) &peer_addr, &peer_addr_size);
    if(cfd < 0){
        perror("Failed to accept\n");
        exit(EXIT_FAILURE);    
    }
    
    add_node(cfd, peer_addr);
    PRINT("Client connected");

    return cfd;
}


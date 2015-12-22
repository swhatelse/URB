#include"server.h"

#define BACKLOG 50
#define PRINT(x)                                \
  printf("Server : %s\n", x)

int sfd;
// TODO used for test
int nb_nodes = 3;
group_t connected_clients;

void connexion_init(int port){
    struct sockaddr_in my_addr;
  
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

void connexion_handler(){
    int i = 0;
    connected_clients.nodes = malloc(3*sizeof(node_t));
    connected_clients.count = nb_nodes;
  
    while(1){
        connected_clients.nodes[i].fd = connexion_accept();
        i++;
    }
}

static int connexion_accept(){
    int cfd;
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size;
  
    cfd = accept(sfd, (struct sockaddr*) &peer_addr, &peer_addr_size);
    if(cfd < 0){
        perror("Failed to accept\n");
        exit(EXIT_FAILURE);    
    }
  
    PRINT("Client connected");

    return cfd;
}

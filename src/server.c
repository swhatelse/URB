#include<assert.h>
#include<sys/select.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/time.h>

#include"server.h"

/********************************************
 *
 *             Macros
 *
 *******************************************/
#define BACKLOG 50

/********************************************
 *
 *             Global vars
 *
 *******************************************/
int listening_fd; // listening socket for connexion
group_t reception_sockets; // Use for incomming messages
fd_set reception_fd_set; // File descriptor to watch
/********************************************
 *
 *             Functions
 *
 *******************************************/
/** Prepare the socket for waiting connexion
 *
 */
void connexion_init(){
    struct sockaddr_in my_addr;
    // TODO here just for the moment
    /* reception_sockets.nodes = NULL; */

    PRINT("Initialization of the listener");
    
    listening_fd = socket(AF_INET, SOCK_STREAM,0);
    if(listening_fd < 0){
        perror("Failed to attribute the socket\n");
        exit(EXIT_FAILURE);
    }
  
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(my_port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listening_fd, (struct sockaddr*) &my_addr, sizeof(my_addr)) < 0 ){
        perror("Failed to name the socket\n");
        exit(EXIT_FAILURE);    
    }

    if(listen(listening_fd, BACKLOG) < 0){
        perror("Failed to listen\n");
        exit(EXIT_FAILURE);    
    }
}

void* connexion_handler(){
    struct timeval timeout;
    int event = 0;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    
    PRINT("Start to listen");
    while(1){
        // Need to reinitialize the set each time
        FD_ZERO(&reception_fd_set);
        // Re-fill the set
        FD_SET(listening_fd, &reception_fd_set);
        for(int i = 0; i < receive_sockets.count; i++){
            if(receive_sockets.nodes[i].fd != 0){
                FD_SET(receive_sockets.nodes[i].fd, &reception_fd_set);
            }
        }

        
        event = select(FD_SETSIZE, &reception_fd_set, NULL, NULL, &timeout);
        if(event == -1){
            
        }
        else if(event){
            if(FD_ISSET(listening_fd, &reception_fd_set)){
                connexion_accept();
            }
            
            for(int i = 0; i < receive_sockets.count; i++){
                if(FD_ISSET(receive_sockets.nodes[i].fd, &reception_fd_set)){

                }
            }
        }
        else{

        }
    }
    return NULL;
}

/** Add a node to the receiving list
 * @param fd File descriptor associated to the addr
 * @param addr Address information of the incomming connexion
 * @return 0 if ok 1 if fails
 */
int add_node(const int fd, const struct sockaddr_in addr){
    assert(fd > 2); // To be sure the fd is valid
    
    for(int i = 0; i < receive_sockets.count; i++){
        if(receive_sockets.nodes[i].infos.sin_addr.s_addr == 0){
            receive_sockets.nodes[i].infos = addr;
            receive_sockets.nodes[i].fd = fd;
            FD_SET(fd, &reception_fd_set);
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

int connexion_accept(){
    int cfd;
    char buf[64];
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size;
  
    cfd = accept(listening_fd, (struct sockaddr*) &peer_addr, &peer_addr_size);
    if(cfd < 0){
        perror("Failed to accept\n");
        exit(EXIT_FAILURE);    
    }

    // TODO: Need to detect if the group is full before adding
    add_node(cfd, peer_addr);
    
    sprintf(buf, "client from %d %d is connected", peer_addr.sin_addr.s_addr, peer_addr.sin_port);
    PRINT(buf);
    
    return cfd;
}


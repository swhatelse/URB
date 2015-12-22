#include<string.h>
#include"client.h"

#define PRINT(x)                                \
    printf("Client : %s\n", x)

int connection(const char* addr, const int port){
    int sfd;
    struct sockaddr_in srv_addr;

    sfd = socket(AF_INET, SOCK_STREAM,0);
    if(sfd < 0){
        perror("Failed to attribute the socket");
        return EXIT_FAILURE;
    }
  
    // Retrieve server info
    /* struct hostent *server = malloc(sizeof(struct hostent)); */
    /* server = gethostbyname("127.0.0.1"); */
    /* printf("%lx\n", (long unsigned int) server); */

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    srv_addr.sin_addr.s_addr = inet_addr(addr);

    if(connect(sfd, (struct sockaddr*) &srv_addr, sizeof(srv_addr)) < 0){
        perror("Failed to name the socket");
        return -1;    
    }
  
    /* close(sfd); */
    return sfd;
}

int join(char* fname){
    char buf[16];
    char** group = NULL;
    int group_count = 0;
    FILE *fd;
  
    fd = fopen(fname, "r");
  
    while(fgets(buf, sizeof(buf), fd) != NULL){
        group = realloc( (void*) group, group_count + 1 );
        group[group_count] = malloc(16);
        memcpy(group[group_count], buf, strlen(buf));
        group_count++;
    }

    // Debug to remove
    for(int i = 0; i < group_count; i++){    
        printf("%s", group[i]);
    }

    for(int i = 0; i < group_count; i++){
        char *addr;
        int port;
        char sep = ':';
        
        addr = strtok(group[i], &sep);
        port = atoi(strtok(NULL, &sep));
        printf("%d\n", port);
        connection(addr, port);
    }
  
    for(int i = 0; i < group_count; i++){
        free(group[i]);
    }
    free(group);
  
    fclose(fd);
    
    return EXIT_SUCCESS;
}

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<sys/select.h>

#include"common.h"
#include"group.h"

#ifndef LISTENER_H
#define LISTENER_H

// Globals
int             listening_fd;       // listening socket for connexion
group_t         reception_sockets;  // Use for incomming messages
fd_set          reception_fd_set;   // File descriptor to watch

// Functions
void listener_init();
void* listener_run();
int connexion_accept();
#endif

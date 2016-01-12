#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<sys/select.h>

#include"common.h"
#include"group.h"
#include"node.h"
#include"list.h"

#ifndef LISTENER_H
#define LISTENER_H

// Types
/* typedef struct connexions_pending_t{
    connexion_t* connexion;
    struct connexions_pending_t* prev;
    struct connexions_pending_t* next;
    }connexions_pending_t; */

// Globals
int             listening_fd;       // listening socket for connexion
fd_set          reception_fd_set;   // File descriptor to watch

// Functions
void listener_init();
void* listener_run();
int connexion_accept();
#endif

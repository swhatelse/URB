#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>

#include"common.h"

#ifndef SERVER_H
#define SERVER_H

void listener_init();
void* connexion_handler();
int connexion_accept();
#endif

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>

#include"common.h"

#ifndef SERVER_H
#define SERVER_H

void connexion_init(int port);
static int connexion_accept();
void connexion_handler();

#endif

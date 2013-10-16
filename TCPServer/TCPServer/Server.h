
#ifndef __Server_H_INCLUDED__   
#define __Server_H_INCLUDED__   

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_BUFLEN 1024
#define PORT_BUFLEN 5
#define MAX_CONNECTIONS 10000

class Server;

#endif 




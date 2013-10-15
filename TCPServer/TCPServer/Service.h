

#ifndef __Client_H_INCLUDED__   
#define __Client_H_INCLUDED__   

#include "server.h"

enum SERVICE_PORT {HELLO_SERVICE=2810, ECHO_SERVICE=2811};

class Client {	

	int port;

	SOCKET socket;

	Server * server;

	FD_SET readSet;			

	int canReed;

public:
	Client(SOCKET, Server *, int);	

	Client(SOCKET, Server *, char *);		

	Client();	

	~Client();	

	void resetReadSet();

	bool isInReadSet();

	void makeNonBlocking();

	void disconect();

	bool isValid();

	int getService();

	void write(const char *);

	void write(const char *, int);

	int read(char *, int);

	bool canRead();
};

class Services {		
		static void hello(Client *);

		static void echo(Client *);

		static void process(int readPerCall, char * buff, Client * client, BOOL & read);

	public:
		static void serve(LPVOID lpParam);
};

#endif 


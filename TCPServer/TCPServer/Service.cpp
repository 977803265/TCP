
#include "service.h"

void Services::serve(LPVOID lpParam){
	// each thread has its own client
	Client * c = (Client *)lpParam;		
	switch (c->getService()) {
	  case HELLO_SERVICE:
		Services::hello(c);
		break;
	  case ECHO_SERVICE:
		Services::echo(c);
		break;
	  default:
		printf("Unknown service requested by client.\n Disconect the client.");
		c->disconect();
		return;
	}
	// anyway disconect the client
	if(c!= NULL && c!= 0 && c->isValid()){
		c->disconect();
		return;
	}
	// intentionally assing 0 to pointer in order to prevet 
	// calling to object desctructor which already called
	c = 0;
}


void Services::echo(Client * client){			
	client->makeNonBlocking();	
	int bytesRead;	
	char readBuf[DEFAULT_BUFLEN];		
		
	BOOL read = true;
	
	// speak cycle
	do {		
		client->resetReadSet();
		//read only when socketObj is ready
		if (client->canRead()) {
			if(client->isInReadSet()){
				bytesRead = client->read(readBuf, DEFAULT_BUFLEN);				
				if (bytesRead > 0) {

				    if(bytesRead == 1){
						client->write("\rTelnet is not supported.\n\rPlease use the normal client.\n\r");
						client->write("Good by.\n\r");								
						read = false;						
					} else {
						readBuf[bytesRead] = '\0';
						process(bytesRead, readBuf, client, read);					
					}
					
				}								
			}
		}		
	} while (read); 
}




void Services::process(int bytesRead, char * cmd, Client * client, BOOL & read){		

	if(strncmp(cmd, "QUIT", 4) == 0){
		// Send good by								
		client->write("Good by.\n");								
		read = false;
	} else {
		// Send echo				
		client->write("SERVER:");
		client->write(cmd, bytesRead);		
		return;
	}
}




void Services::hello(Client * c){	
	c->write("Hello client. I am an TCP Server.\n");	
}

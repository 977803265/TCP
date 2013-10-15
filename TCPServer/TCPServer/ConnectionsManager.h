
#ifndef __ConnectionsManager_H_INCLUDED__   
#define __ConnectionsManager_H_INCLUDED__   

#include "service.h"

class ConnectionsManager {
	
public:	
	static int disconNum;

	static int disconIndexes[MAX_CONNECTIONS];

	static bool suspend;

	static Client * connections[MAX_CONNECTIONS];

	static int connIndex;	

	static void disconnectClient(Client *);

	static void disconnectAllClients();

	static bool canConnect();	

	static void addConnection(Client *);	

};

#endif 


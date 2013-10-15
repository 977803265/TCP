
	#include "ConnectionsManager.h"	

	Client * ConnectionsManager::connections[MAX_CONNECTIONS];

	int ConnectionsManager::connIndex = 0;	

	bool ConnectionsManager::suspend = false;

	int ConnectionsManager::disconNum = 0;

	int ConnectionsManager::disconIndexes[MAX_CONNECTIONS];

	void ConnectionsManager::disconnectClient(Client * pClient){	
		for(int i = 0; i < MAX_CONNECTIONS; i++){
			if(connections[i] == pClient){				
				delete connections[i];
				connections[i] = 0;

				// add index to disconnected
				disconIndexes[disconNum] = i;
				disconNum++;
				printf("Client disconected.\n");
			}
		}
	}

	bool ConnectionsManager::canConnect(){		
		return ((connIndex + 1) >= MAX_CONNECTIONS) ? false : true;
	}	

	void ConnectionsManager::addConnection(Client * c){
		if(canConnect() == false){
			printf("Connections limit has been reached.\nDiscard connection.\n");
			c->disconect();
			return;		
		} else {			
			int discIndex = disconIndexes[disconNum];			
			if(connections[discIndex] == 0){
				// use free index				
				connections[discIndex] = c;						
				disconNum--;
			} else {
				// use new index
				connections[connIndex] = c;
				connIndex += 1;
			}			
			printf("New connection.\n");
		}					
	}

	void ConnectionsManager::disconnectAllClients(){
		for(int i = 0; i < MAX_CONNECTIONS; i++){
			if(connections[i] != NULL && connections[i] != 0){				
				delete connections[i];				
			}
		}	
	}

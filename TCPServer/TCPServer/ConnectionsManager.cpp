
	#include "ConnectionsManager.h"	

	Client * ConnectionsManager::connections[MAX_CONNECTIONS];

	int ConnectionsManager::connIndex = 0;	

	bool ConnectionsManager::suspend = false;

	int ConnectionsManager::disconNum = 0;

	int ConnectionsManager::disconIndexes[MAX_CONNECTIONS];

	Client * ConnectionsManager::get(int index){
		if(index >= 0 && index < MAX_CONNECTIONS){
			return connections[index];
		}
		return 0;
	}

	void ConnectionsManager::disconnectClient(int index){					
		if(index >= 0 && index < MAX_CONNECTIONS){
			if(connections[index] != 0){				
				delete connections[index];
				connections[index] = 0;

				// add index to disconnected
				disconIndexes[disconNum] = index;
				disconNum++;
				printf("Client disconected.\n");
			}
		}		
	}

	bool ConnectionsManager::canConnect(){		
		return ((connIndex + 1) >= MAX_CONNECTIONS) ? false : true;
	}	

	int ConnectionsManager::addConnection(SOCKET sock, Server * serv, char * p){
		if(canConnect() == false){
			printf("Connections limit has been reached.\nDiscard connection.\n");			
			return -1;		
		} else {		
			int ret = -2;
			int discIndex = disconIndexes[disconNum];			
			if(connections[discIndex] == 0){
				// use free index				
				connections[discIndex] = new Client(sock, serv, p, discIndex);						
				disconNum--;
				ret = discIndex;
			} else {
				// use new index
				connections[connIndex] = new Client(sock, serv, p, connIndex);	
				ret = connIndex;
				connIndex += 1;
			}			
			printf("New connection.\n");
			return ret;
		}
		return -3;
	}
	

	void ConnectionsManager::disconnectAllClients(){
		for(int i = 0; i < MAX_CONNECTIONS; i++){
			disconnectClient(i);
		}	
	}

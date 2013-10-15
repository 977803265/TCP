

#ifndef __Server_H_INCLUDED__   
#define __Server_H_INCLUDED__   

#include "service.h"

class Connections {

	Client * connections[MAX_CONNECTIONS];

public:
	Connections();

	int add();

	int insert();

	Client * get();

};

#endif 
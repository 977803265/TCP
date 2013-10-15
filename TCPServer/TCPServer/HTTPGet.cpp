
#include "server.h"

// make http request
int getPage(const char * host, 
			const char * file, 
			const char * ip, 
			char * response = NULL, 
			int respLen = 10240)
{	
	struct addrinfo hints;
	struct addrinfo * result = NULL;
	struct addrinfo * ptr = NULL;
	                
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	char  * httpPort = "80";
	
	int iResult;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, httpPort, &hints, &result);
	if (iResult != 0) {
		//resolve failed
		lastNetError = WSAGetLastError();
		return RESOLVE_ERROR;
	}
	
	SOCKET socketObj = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	socketObj = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (socketObj == INVALID_SOCKET) {
		// try to use another addrinfo in list
		while(ptr = ptr->ai_next){
			// Create a SOCKET for connecting to server
			socketObj = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (socketObj == INVALID_SOCKET) {
				continue;
			} else {
				break;
			}
		}		
	}

	if (socketObj == INVALID_SOCKET) {
		freeaddrinfo(result);		
		lastNetError = WSAGetLastError();
		return SOCKET_CREATE_ERROR;
	}
	
	const int bufSize = 1024*4;
	char sendBuffer [bufSize];

	string request;
	request += "GET ";
	request += file; // string class support concatenation with char type (C strings)
	request += " HTTP/1.1\r\n";
	request += "Host: ";
	request += host; // string class support concatenation with char type (C strings)
	request += "\r\n";
	request += "Connection: close\r\n";
	request += "Accept-Language: uk-UA,uk;q=0.8,ru;q=0.6,en-US;q=0.4,en;q=0.2\r\n";
	request += "Connection: keep-alive\r\n";
	request += "Referer: http://www.google.com\r\n";
	request += "User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.19 (KHTML, like Gecko)";
	request += "Chrome/18.0.1025.168 Safari/535.19\r\n";
	request += "\r\n";

	strncpy_s(sendBuffer, bufSize, request.c_str(), request.size());

	// Connect unconnected socketObj to server.	
	int connResult = connect(socketObj, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (connResult == SOCKET_ERROR) {
		closesocket(socketObj);	
		lastNetError = WSAGetLastError();
		return SOCKET_CONNECT_ERROR;
	}

	//Переводимо сокет клієнта в неблокуючий режим
	unsigned long ulMode = 1;	
	ioctlsocket(socketObj, FIONBIO,  &ulMode);

	// Send an initial buffer
	int sendResult = send(socketObj, sendBuffer, (int) strlen(sendBuffer), 0);
	if (sendResult == SOCKET_ERROR) {		
		closesocket(socketObj);
		lastNetError = WSAGetLastError();
		return SEND_REQUEST_ERROR;
	}
		

	// Receive data 
	const int partrialRespLen = 1024*2;
	const int totalRespLen = HTTP_RESPONSE_LEN;
	char partrialResp [partrialRespLen];
	char totalResp[totalRespLen];
	
	int bytesRead;		
	int totalRead = 0;
	
	FD_SET readSet;	
	FD_ZERO(&readSet);
	FD_SET(socketObj, &readSet);
	
	int canReed;
	BOOL read = true;

	do {
		//read only when socketObj is ready
		if ((canReed = select(0, &readSet, NULL, NULL, NULL)) != SOCKET_ERROR) {
			if(FD_ISSET(socketObj, &readSet)){
				bytesRead = recv(socketObj, partrialResp, partrialRespLen, 0);
				if (bytesRead > 0) {					
					for(int i = 0; i<bytesRead; i++, totalRead++){
						// copy part of response
						totalResp[totalRead] = partrialResp[i];
					}					
				} else if (bytesRead == 0) {					
					read = false;
				} else {					
					read = false;
				}
			}
		}
		// chek if we can to read more
		if(totalRead+(partrialRespLen*2) > totalRespLen 
			|| totalRead+(partrialRespLen*2) > respLen){
			read = false;
		}
	} while (read);
	
	if(totalRead > 0 && response != NULL){
		//response
		totalResp[totalRead] = '\0';
		strncpy_s(response, respLen, totalResp, totalRead);		
	} 
	closesocket(socketObj);
	return SUCCSESS;
}

#pragma once
#include "RakNetTypes.h"
#include "MessageTypes.h"

static const int MAX_CLIENTS = 10;
static const int SERVER_PORT = 60000;
//static const char* strIPAddress = "127.0.0.1";

class Network
{
private:
	bool isServer; 
	RakPeerInterface *peer; 
	Packet *packet;
	char message[128];
	static int msgType;
	std::string ipAddress;

public:
	Network(void);
	~Network(void);

	void SetIPAddress(std::string ip) {this->ipAddress = ip;};
	void StartServer();
	void StartClient();

	int GetLastMessageType(){ return msgType; };

	char *Update();

	void SendNetMessage( const NetMessage *message, int sizeInBytes);

	//void SendChatMessage ( const ChatMessage *message );
	//void SendPositionMessage ( const PositionMessage *message );
};

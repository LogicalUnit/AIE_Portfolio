#pragma once
#include "RakNetTypes.h"
#include "MessageTypes.h"

static const int MAX_CLIENTS = 10;
static const int SERVER_PORT = 60000;
static const char* strIPAddress = "127.0.0.1";

class Network
{
private:
	bool isServer; 
	RakPeerInterface *peer; 
	Packet *packet;
	char message[128];

public:
	Network(void);
	~Network(void);

	void StartServer();
	void StartClient();

	char *Update();

	void SendChatMessage ( const ChatMessage *message );
};

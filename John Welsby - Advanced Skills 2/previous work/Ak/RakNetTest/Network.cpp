#include "StdAfx.h"
#include "Network.h"

Network::Network(void)
{
	isServer = true; 
	peer = RakNetworkFactory::GetRakPeerInterface();
	memset(message, 0, 128);
}

Network::~Network(void)
{
	RakNetworkFactory::DestroyRakPeerInterface (peer);
}

void Network::StartClient()
{
	isServer = false; 
	peer->Startup( 1, 30, &SocketDescriptor(), 1 );

	//this currently uses the hard coded address 
	// it should be an option for the client to enter somehow 
	peer->Connect( strIPAddress, SERVER_PORT, 0 ,0 );
}

void Network::StartServer()
{
	isServer = true; 
	peer->Startup(MAX_CLIENTS, 30, &SocketDescriptor( SERVER_PORT, 0), 1 );

	// We need to let the server accept incoming connections from the clients 
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);
}

char *Network::Update()
{
	packet=peer->Receive();
	if (packet)
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			sprintf_s(message, 128, "Another client has disconnected.\n");
			break;
		case ID_REMOTE_CONNECTION_LOST:
			sprintf_s(message, 128,"Another client has lost the connection.\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			sprintf_s(message, 128,"Another client has connected.\n");
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			sprintf_s(message, 128,"Our connection request has been accepted.\n");
			break;
		case ID_NEW_INCOMING_CONNECTION:
			sprintf_s(message, 128,"A connection is incoming.\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			sprintf_s(message, 128,"The server is full.\n");
		break;
		case ID_DISCONNECTION_NOTIFICATION:
			if (isServer)
			{
				sprintf_s(message, 128,"A client has disconnected.\n");
			} 
			else
			{
				sprintf_s(message, 128,"We have been disconnected.\n");
			}
			break;
		case ID_CONNECTION_LOST:
			if (isServer)
			{
				sprintf_s(message, 128,"A client lost the connection.\n");
			} 
			else
			{
				sprintf_s(message, 128,"Connection lost.\n");
			}
			break;
			case ID_CHAT:
			{
				ChatMessage *msg;
				msg = (ChatMessage*)packet->data;
				sprintf_s(message, 128, "A chat message arrived: %s \n", msg->message );
			} 
			break;
		default:
			sprintf_s(message, 128,"Message with identifier %i has arrived.\n", packet->data[0]);
			break;
		}
		peer->DeallocatePacket(packet);
	}
	
	return message;
}
	
void Network::SendChatMessage ( const ChatMessage *message )
{
	// raknet wants the number of bits not the number of bytes - so * 8
	peer->Send((char*)message, sizeof(ChatMessage)*8, HIGH_PRIORITY,
	RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}
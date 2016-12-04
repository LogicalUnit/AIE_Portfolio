#include "StdAfx.h"
#include "Network.h"
#include "RakNetHeader.h"
#include "NetworkInterface.h"


int Network::msgType;

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
	peer->Connect( ipAddress.c_str(), SERVER_PORT, 0 ,0 );
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
	memset(message, 0, sizeof(message));
	packet=peer->Receive();
	if (packet)
	{
		switch (msgType = packet->data[0])
		{
		case ID_CONNECTION_ATTEMPT_FAILED:
			sprintf_s(message, 128, "Connection attempt failed.\n");
			break;
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

			if(!isServer)
				OnConnect();

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

				if (isServer)			
					SendNetMessage(msg, sizeof(ChatMessage));
			}
			break;
			case ID_POSITION:
			{
				PositionMessage *msg;
				msg = (PositionMessage*)packet->data;

				if (isServer)
				{
					sprintf_s(message, 128, "Name: %s Position: %f %f %f\n", msg->name, msg->position.x, msg->position.y, msg->position.z);
					PositionMessage pos(msg->position, msg->name);
					SendNetMessage(&pos, sizeof(pos));
				}
				else
				{
					UpdatePosition(msg->name, msg->position);
				}

			}
			break;
			case ID_ATTACK:
			{
				AttackMessage *msg = (AttackMessage*)packet->data;
				if(isServer)
				{
					sprintf_s(message, 128, "%s attacked\n", msg->attacker.c_str() );
					AttackMessage relay;
					relay.attacker = msg->attacker;
					relay.target = msg->target;
					SendNetMessage(&relay, sizeof(relay));
				}
				else
				{
					ResolveAttack(msg->attacker, msg->target);
				}
			}
			break;
			case ID_DYING:
			{
				DyingMessage *msg = (DyingMessage*)packet->data;
				sprintf_s(message, 128, "%s got owned by %s\n", msg->name.c_str(), msg->attacker );
				if(isServer)
				{
					DyingMessage relay;
					relay.name = msg->name;
					strcpy_s(relay.attacker, 64, msg->attacker);
					SendNetMessage(&relay, sizeof(relay));
				}
				else
				{
					KillPlayer(msg->name);
				}
			}
			break;
			case ID_RESPAWN:
			{
				RespawnMessage *msg = (RespawnMessage*)packet->data;
				sprintf_s(message, 128, "%s is respawning\n", msg->name.c_str() );
				if (isServer)
				{
					RespawnMessage relay;
					relay.name = msg->name;
					SendNetMessage(&relay, sizeof(relay));
				}
				else
				{
					RespawnPlayer(msg->name);
				}
			}
			break;
			case ID_MODEL_INFO:
			{
				ModelMessage *msg = (ModelMessage*)packet->data;
				sprintf_s(message, 128, "%s is using model %s\n", msg->name.c_str(), msg->modelPath );
				if (isServer)
				{
					ModelMessage relay;
					relay.name = msg->name;
					strcpy_s(relay.modelPath, 64, msg->modelPath);
					SendNetMessage(&relay, sizeof(relay));
				}
				else
				{
					SetPlayerModel(msg->name, msg->modelPath);
				}
			}
			break;
			case ID_DISCONNECT:
			{
				Disconnect *msg = (Disconnect*)packet->data;
				sprintf_s(message, 128, "%s disconnected\n", msg->playerName);
				if (isServer)
				{
					Disconnect relay;
					strcpy_s(relay.playerName, 64, msg->playerName);
					SendNetMessage(&relay, sizeof(relay));
				}
				else
				{
					PlayerDisconnect(msg->playerName);
				}
			}
			break;
			case ID_CONNECT:
			{
				Connect *msg = (Connect*)packet->data;
				sprintf_s(message, 128, "%s connected\n", msg->playerName);
				if (isServer)
				{
					Connect relay;
					strcpy_s(relay.playerName, 64, msg->playerName);
					SendNetMessage(&relay, sizeof(relay));
				}
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

void Network::SendNetMessage( const NetMessage *message, int sizeInBytes)
{
	peer->Send((char*)message, sizeInBytes, HIGH_PRIORITY,
	RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}
	
//void Network::SendChatMessage ( const ChatMessage *message )
//{
//	// raknet wants the number of bits not the number of bytes - so * 8
//	peer->Send((char*)message, sizeof(ChatMessage), HIGH_PRIORITY,
//	RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
//}
//
//void Network::SendPositionMessage ( const PositionMessage *message )
//{
//	peer->Send((char*)message, sizeof(PositionMessage), HIGH_PRIORITY,
//	RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
//}
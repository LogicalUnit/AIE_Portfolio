/// \file
/// \brief A simple TCP based server allowing sends and receives.  Can be connected to by a telnet client.
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.jenkinssoftware.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#include "TCPInterface.h"
#ifdef _WIN32
//#include <Shlwapi.h>
#include <process.h>
#else
#include <sys/time.h>
#define closesocket close
#include <unistd.h>
#include <pthread.h>
#endif
#include <string.h>
#include <assert.h>
#include <stdio.h>

#ifdef _WIN32
unsigned __stdcall UpdateTCPInterfaceLoop( LPVOID arguments );
#else
#define closesocket close
void* UpdateTCPInterfaceLoop( void* arguments );
#endif

#include "RakSleep.h"

#ifdef _DO_PRINTF
#endif


#ifdef _MSC_VER
#pragma warning( push )
#endif

TCPInterface::TCPInterface()
{
	isStarted=false;
	threadRunning=false;
	listenSocket=INVALID_SOCKET;

#ifdef _WIN32

	WSADATA winsockInfo;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &winsockInfo ) != 0 )
	{
#if defined(_WIN32) && !defined(_CONSOLE_1) && defined(_DEBUG)
		DWORD dwIOError = GetLastError();
		LPVOID messageBuffer;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwIOError, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),  // Default language
			( LPTSTR ) & messageBuffer, 0, NULL );
		// something has gone wrong here...
		printf( "WSAStartup failed:Error code - %d\n%s", dwIOError, messageBuffer );
		//Free the buffer.
		LocalFree( messageBuffer );
#endif
	}

#endif
}
TCPInterface::~TCPInterface()
{
	Stop();
}
bool TCPInterface::Start(unsigned short port, unsigned short maxIncomingConnections)
{
	if (isStarted)
		return false;

	isStarted=true;

	if (maxIncomingConnections>0)
	{
		listenSocket = socket(AF_INET, SOCK_STREAM, 0);
		if ((int)listenSocket ==INVALID_SOCKET)
			return false;

		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		serverAddress.sin_port = htons(port);

		if (bind(listenSocket,(struct sockaddr *) &serverAddress,sizeof(serverAddress)) < 0)
			return false;

		listen(listenSocket, maxIncomingConnections);
	}

	// Start the update thread
#ifdef _WIN32
	unsigned threadId = 0;
	threadHandle = ( HANDLE ) _beginthreadex( NULL, 0, UpdateTCPInterfaceLoop, this, 0, &threadId );

	//SetThreadPriority(threadHandle, THREAD_PRIORITY_BELOW_NORMAL);

	if ( threadHandle == 0 )
		return false;
	CloseHandle( threadHandle );
	threadHandle = 0;
#else
	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );

//	sched_param sp;
//	sp.sched_priority = sched_get_priority_min(SCHED_OTHER);
//	pthread_attr_setschedparam(&attr, &sp);

	int error;
	error = pthread_create( &threadHandle, &attr, &UpdateTCPInterfaceLoop, this );
	if ( error )
		return false;
#endif

	return true;
}
void TCPInterface::Stop(void)
{
	if (isStarted==false)
		return;

	isStarted=false;

	if (listenSocket!=INVALID_SOCKET)
	{
		closesocket(listenSocket);
		listenSocket=INVALID_SOCKET;
	}

	// Wait for the thread to stop
	while ( threadRunning )
		RakSleep(15);

	// Stuff from here on to the end of the function is not threadsafe
	unsigned i;
	for (i=0; i < remoteClients.Size(); i++)
	{
		closesocket(remoteClients[i]->socket);
		delete remoteClients[i];
	}
	remoteClients.Clear();

	outgoingMessages.Clear();
	incomingMessages.Clear();
	newConnections.Clear();
	newRemoteClients.Clear();
	lostConnections.Clear();
	requestedCloseConnections.Clear();
}
SystemAddress TCPInterface::Connect(const char* host, unsigned short remotePort)
{
	sockaddr_in serverAddress;

#if !defined(_CONSOLE_1)
	struct hostent * server;
	server = gethostbyname(host);
	if (server == NULL)
		return UNASSIGNED_SYSTEM_ADDRESS;
#endif

	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		return UNASSIGNED_SYSTEM_ADDRESS;

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons( remotePort );
	

#if !defined(_CONSOLE_1)
	memcpy((char *)&serverAddress.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
#else
	serverAddress.sin_addr.s_addr = inet_addr( host );
#endif

	// This is blocking but whatever.  If I need it non-blocking I will make it so later.
	if ( connect( sockfd, ( struct sockaddr * ) &serverAddress, sizeof( struct sockaddr ) ) != 0 )
	{
		closesocket(sockfd);
		return UNASSIGNED_SYSTEM_ADDRESS;
	}

	waitForClient=true;
	RemoteClient *remoteClient;
	remoteClient = new RemoteClient;
	remoteClient->socket=sockfd;
	remoteClient->systemAddress.binaryAddress=inet_addr(host);
	remoteClient->systemAddress.port=remotePort;
	RemoteClient **temp = newRemoteClients.WriteLock();
	*temp=remoteClient;
	newRemoteClients.WriteUnlock();
	while (waitForClient)
	{
		RakSleep(30);
	}
	return remoteClient->systemAddress;
}
void TCPInterface::Send( const char *data, unsigned length, SystemAddress systemAddress )
{
	if (isStarted==false)
		return;
	if (remoteClients.Size()==0)
		return;
	if (data==0)
		return;
	Packet *p=outgoingMessages.WriteLock();
	p->length=length;
	p->data = (unsigned char*) rakMalloc( BITS_TO_BYTES(p->length ) );
	memcpy(p->data, data, p->length);
	p->systemAddress=systemAddress;
	outgoingMessages.WriteUnlock();
}
Packet* TCPInterface::Receive( void )
{
	if (isStarted==false)
		return 0;
	return incomingMessages.ReadLock();
}
void TCPInterface::CloseConnection( SystemAddress systemAddress )
{
	if (isStarted==false)
		return;
	if (systemAddress==UNASSIGNED_SYSTEM_ADDRESS)
		return;
	SystemAddress *id = requestedCloseConnections.WriteLock();
	*id=systemAddress;
	requestedCloseConnections.WriteUnlock();
}
void TCPInterface::DeallocatePacket( Packet *packet )
{
	assert(incomingMessages.CheckReadUnlockOrder(packet));
	rakFree(packet->data);
	incomingMessages.ReadUnlock();
}
SystemAddress TCPInterface::HasNewConnection(void)
{
	SystemAddress *out;
	out = newConnections.ReadLock();
	if (out)
	{
		newConnections.ReadUnlock();
		return *out;
	}
	else
	{
		return UNASSIGNED_SYSTEM_ADDRESS;
	}
}
SystemAddress TCPInterface::HasLostConnection(void)
{
	SystemAddress *out;
	out = lostConnections.ReadLock();
	if (out)
	{
		lostConnections.ReadUnlock();
		return *out;
	}
	else
	{
		return UNASSIGNED_SYSTEM_ADDRESS;
	}
}
#ifdef _MSC_VER
#pragma warning( disable : 4100 ) // warning C4100: <variable name> : unreferenced formal parameter
#endif
void TCPInterface::DeleteRemoteClient(RemoteClient *remoteClient, fd_set *exceptionFD)
{
//	FD_CLR(remoteClient->socket, exceptionFD);
	closesocket(remoteClient->socket);
	//shutdown(remoteClient->socket, SD_SEND);
	delete remoteClient;
}

#ifdef _WIN32
unsigned __stdcall UpdateTCPInterfaceLoop( LPVOID arguments )
#else
void* UpdateTCPInterfaceLoop( void* arguments )
#endif
{
	TCPInterface * sts = ( TCPInterface * ) arguments;
	RemoteClient *remoteClient;
	RemoteClient **otherThreadClient;
	const int BUFF_SIZE=8096;
	char data[ BUFF_SIZE ];
	Packet *p;
	SystemAddress *systemAddress;
	fd_set      readFD, exceptionFD;
	sts->threadRunning=true;

	sockaddr_in sockAddr;
	int sockAddrSize = sizeof(sockAddr);

	unsigned i;
	int len;
	SOCKET newSock;
	timeval tv;
	int selectResult;
	tv.tv_sec=0;
	tv.tv_usec=25000;

	while (sts->isStarted)
	{
		p=sts->outgoingMessages.ReadLock();
		while (p)
		{
			if (p->systemAddress==UNASSIGNED_SYSTEM_ADDRESS)
			{
				// Send to all
				for (i=0; i < sts->remoteClients.Size(); i++)
					send(sts->remoteClients[i]->socket, (const char*)p->data, p->length, 0);
			}
			else
			{
				// Send to this player
				for (i=0; i < sts->remoteClients.Size(); i++)
					if (sts->remoteClients[i]->systemAddress==p->systemAddress )
						send(sts->remoteClients[i]->socket, (const char*)p->data, p->length, 0);
			}

			rakFree(p->data);
			sts->outgoingMessages.ReadUnlock();
			p=sts->outgoingMessages.ReadLock();
		}

		otherThreadClient=sts->newRemoteClients.ReadLock();
		if (otherThreadClient)
		{
			sts->remoteClients.Insert(*otherThreadClient);
			sts->newRemoteClients.ReadUnlock();
			sts->waitForClient=false;
		}

		systemAddress=sts->requestedCloseConnections.ReadLock();
		if (systemAddress)
		{
			for (i=0; i < sts->remoteClients.Size(); i++)
			{
				if (sts->remoteClients[i]->systemAddress==*systemAddress )
				{
					systemAddress=sts->lostConnections.WriteLock();
					*systemAddress=sts->remoteClients[i]->systemAddress;
					sts->lostConnections.WriteUnlock();

					sts->DeleteRemoteClient(sts->remoteClients[i], &exceptionFD);
					sts->remoteClients.RemoveAtIndex(i);


					/*
					systemAddress=sts->lostConnections.WriteLock();
					*systemAddress=sts->remoteClients[i]->systemAddress;
					sts->lostConnections.WriteUnlock();
					sts->remoteClients.Del(i);
					*/
					break;
				}
			}

			sts->requestedCloseConnections.ReadUnlock();
		}

		SOCKET largestDescriptor=0; // see select()'s first parameter's documentation under linux

		// Reset readFD and exceptionFD since select seems to clear it
		FD_ZERO(&readFD);
		FD_ZERO(&exceptionFD);
#ifdef _MSC_VER
#pragma warning( disable : 4127 ) // warning C4127: conditional expression is constant
#endif
		if (sts->listenSocket!=INVALID_SOCKET)
		{
			FD_SET(sts->listenSocket, &readFD);
			FD_SET(sts->listenSocket, &exceptionFD);
			largestDescriptor = sts->listenSocket; // @see largestDescriptor def
		}
		
		for (i=0; i < sts->remoteClients.Size(); i++)
		{
			FD_SET(sts->remoteClients[i]->socket, &readFD);
			FD_SET(sts->remoteClients[i]->socket, &exceptionFD);
			if(sts->remoteClients[i]->socket > largestDescriptor) // @see largestDescriptorDef
				largestDescriptor = sts->remoteClients[i]->socket;
		}


		// Linux' select() implementation changes the timeout
		tv.tv_sec=0;
		tv.tv_usec=25000;

#ifdef _MSC_VER
#pragma warning( disable : 4244 ) // warning C4127: conditional expression is constant
#endif
#if defined(_CONSOLE_2)
		selectResult=socketselect(largestDescriptor+1, &readFD, 0, &exceptionFD, &tv);
#else
		selectResult=(int) select(largestDescriptor+1, &readFD, 0, &exceptionFD, &tv);		
#endif
		//selectResult=select(largestDescriptor+1, &readFD, 0, &exceptionFD, &tv);

		//selectResult=select(0, &readFD, 0, &exceptionFD, &tv);

		if (selectResult > 0)
		{
			if (sts->listenSocket!=INVALID_SOCKET && FD_ISSET(sts->listenSocket, &readFD))
			{
				newSock = accept(sts->listenSocket, (sockaddr*)&sockAddr, (socklen_t*)&sockAddrSize);

				if (newSock != INVALID_SOCKET)
				{
					remoteClient = new RemoteClient;
					remoteClient->socket=newSock;
					remoteClient->systemAddress.binaryAddress=sockAddr.sin_addr.s_addr;
					remoteClient->systemAddress.port=ntohs( sockAddr.sin_port);
					sts->remoteClients.Insert(remoteClient);
					systemAddress=sts->newConnections.WriteLock();
					*systemAddress=remoteClient->systemAddress;
					sts->newConnections.WriteUnlock();

					FD_SET(newSock, &readFD);
					FD_SET(newSock, &exceptionFD);
				}
				else
				{
#ifdef _DO_PRINTF
					printf("Error: connection failed\n");
#endif
				}
			}
			else if (sts->listenSocket!=INVALID_SOCKET && FD_ISSET(sts->listenSocket, &exceptionFD))
			{
#ifdef _DO_PRINTF
				int err;
				int errlen = sizeof(err);
				getsockopt(sts->listenSocket, SOL_SOCKET, SO_ERROR,(char*)&err, &errlen);
				printf("Socket error %s on listening socket\n", err);
#endif
			}
			else
			{
				i=0;
				while (i < sts->remoteClients.Size())
				{
					if (FD_ISSET(sts->remoteClients[i]->socket, &exceptionFD))
					{
#ifdef _DO_PRINTF
						if (sts->listenSocket!=INVALID_SOCKET)
						{
							int err;
							int errlen = sizeof(err);
							getsockopt(sts->listenSocket, SOL_SOCKET, SO_ERROR,(char*)&err, &errlen);
							in_addr in;
							in.s_addr = sts->remoteClients[i]->systemAddress.binaryAddress;
							printf("Socket error %i on %s:%i\n", err,inet_ntoa( in ), sts->remoteClients[i]->systemAddress.port );
						}
						
#endif
						// Connection lost abruptly
						systemAddress=sts->lostConnections.WriteLock();
						*systemAddress=sts->remoteClients[i]->systemAddress;
						sts->lostConnections.WriteUnlock();
						sts->DeleteRemoteClient(sts->remoteClients[i], &exceptionFD);
						sts->remoteClients.RemoveAtIndex(i);
					}
					else
					{
						if (FD_ISSET(sts->remoteClients[i]->socket, &readFD))
						{
							// if recv returns 0 this was a graceful close
							len=recv(sts->remoteClients[i]->socket, data, BUFF_SIZE, 0);
							if (len>0)
							{
								p=sts->incomingMessages.WriteLock();
								p->data = (unsigned char*) rakMalloc( BITS_TO_BYTES(len+1 ) );
								memcpy(p->data, data, len);
								p->data[len]=0; // Null terminate this so we can print it out as regular strings.  This is different from RakNet which does not do this.
								p->length=len;
								p->systemAddress=sts->remoteClients[i]->systemAddress;
								sts->incomingMessages.WriteUnlock();
								i++; // Nothing deleted so increment the index
							}
							else
							{
								// Connection lost gracefully
								systemAddress=sts->lostConnections.WriteLock();
								*systemAddress=sts->remoteClients[i]->systemAddress;
								sts->lostConnections.WriteUnlock();
								sts->DeleteRemoteClient(sts->remoteClients[i], &exceptionFD);
								sts->remoteClients.RemoveAtIndex(i);
							}
						}
						else
							i++; // Nothing deleted so increment the index
					}
				}
			}
		}
		else if (selectResult==0)
		{
			// No input - sleep for a while
			RakSleep(50);
		}
		else
		{
			// FD_CLOSE? closesocket(remoteClient->socket);

#if defined(_DO_PRINTF) && defined(_WIN32)
			DWORD dwIOError = WSAGetLastError();
			printf("Socket error %i\n", dwIOError);
#endif
		}



	}
	sts->threadRunning=false;

	return 0;
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

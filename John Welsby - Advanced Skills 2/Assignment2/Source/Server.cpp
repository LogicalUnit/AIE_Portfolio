#include <iostream>
#include <conio.h>
#include "Network.h"

int main()
{
	//ten line server!!
	Network net;

	net.StartServer();

	std::cout<<"Server Online\n";

	char *msg = net.Update();

	while (!_kbhit())
	{
		if (msg[0])
			std::cout<<msg<<std::endl;

		msg = net.Update();
	}

	std::cout<<"Server Offline\n";

}
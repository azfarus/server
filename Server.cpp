#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include "ServerUtils.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;





void main()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	// Create a socket
	
	SOCKET clientSocket = serverCreate();




	

	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(), Trying to connect again" << endl;

			clientSocket = serverCreate();
			continue;
			
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;

			clientSocket = serverCreate();
			continue;
			
		}

		if (!strncmp(buf, "faculty", 6))
		{
			//send(clientSocket,(char *)&siuuu,sizeof(siuuu), 0);
			sendFaculty(clientSocket);
		}
		else send(clientSocket, 0 , 0, 0);

	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}

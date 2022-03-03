#include <iostream>
#include <WS2tcpip.h> //Contains definitions introduced in winsock 2 and functions to receive IPs
					 //Also access network sockets

#include <string>
#include "ServerUtils.h"

#pragma comment (lib, "ws2_32.lib") //Linking libraries

using namespace std;





void main()
{
	//initialize Winsock
	WSADATA wsData; //Contains information about the windows sockets implementation
	WORD ver = MAKEWORD(2, 2); //The MAKEWORD(2,2) parameter of WSAStartup makes a request for
							   //version 2.2 of Winsock on the system, and sets the passed version as the highest version of Windows Sockets support that the caller can use
	int wsok = WSAStartup(ver, &wsData); //allows an application or DLL to specify the version of Windows Sockets required and retrieve details of the specific Windows Sockets implementation
	if (wsok != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	// Create a socket
	
	SOCKET clientSocket = serverCreate();




	
	//Features handling loop//
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
			sendFaculty(clientSocket);
		}
		else if (!strncmp(buf, "searchfaculty", 13))
		{
			searchFaculty(clientSocket);
		}
		else if (!strncmp(buf, "help", 4)) //Changed here for sending emergency services
		{
			sendHelp_vect(clientSocket);
		}
		else if (!strncmp(buf, "Portal", 6))
		{
			sendPortal_vect(clientSocket);
		}
		else send(clientSocket, 0 , 0, 0);

	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}

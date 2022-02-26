#include"ServerUtils.h"


SOCKET serverCreate()
{
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return (SOCKET)-1;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ....

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}
	closesocket(listening);
	return clientSocket;
	// Close listening socket

}

void sendFaculty(SOCKET sock)
{
	FILE* fp = fopen(facultylist, "r+");
	info faculty;
	fseek(fp, 0, 0);

	while (!feof(fp))
	{
		ZeroMemory(&faculty, sizeof(faculty));
		fread(&faculty, sizeof(faculty), 1, fp);
		int send_success = send(sock, (char*)&faculty, sizeof(faculty), 0);
		if (send_success == SOCKET_ERROR)
		{
			cerr << "Client disconnected, try to connect again\n";
			return;
		}
	}

	fclose(fp);
	return;


}

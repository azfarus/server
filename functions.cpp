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

bool facultyDeptComp(const info a , const info b)
{
	int c = strcmp(a.department, b.department);
	if (c < 0) return TRUE;
	else return FALSE;
}

vector<string> substringer(char* str)
{
	vector<string> parts;
	char subs[5];
	

	for (int i = 0; i < strlen(str)-1; i++)
	{
		ZeroMemory(subs, 5);
		strncpy(subs, str + i, 2);
		string cpp_sub(subs);
		parts.push_back(cpp_sub);
	}

	return parts;

}
	

int facultyNameComp( char * match,  char * toMatch)
{
	string name(match);
	string substring(toMatch);
	vector<string> parts;
	
	
	transform(name.begin(), name.end(), name.begin() , ::tolower);
	transform(substring.begin(), substring.end(),substring.begin(), ::tolower);

	parts = substringer(toMatch);

	for (unsigned int i = 0; i < parts.size(); i++)
	{
		//cout << parts[i] << endl;
		size_t f = name.find(parts[i]);
		if (f != string::npos)
		{
			
			return TRUE;
		}
	}
	return FALSE;


	

	





	
}

void sendFaculty(SOCKET sock)
{
	FILE* fp = fopen(facultylist, "r+");
	info faculty;
	vector<info> facultyShomuho;
	fseek(fp, 0, 0);

	while (!feof(fp))
	{
		ZeroMemory(&faculty, sizeof(faculty));
		fread(&faculty, sizeof(faculty), 1, fp);
		if (faculty.phone_no == 0) break;
		facultyShomuho.push_back(faculty);
		
	}

	
	std::sort(facultyShomuho.begin(), facultyShomuho.end(), facultyDeptComp );

	ZeroMemory(&faculty, sizeof(faculty));
	facultyShomuho.push_back(faculty);


	cout << "Faculties shown :" << facultyShomuho.size() << endl;
	for (unsigned int i = 0; i < facultyShomuho.size(); i++)
	{
		int sendbytes = send(sock, (char*)( &facultyShomuho[i]), sizeof(faculty), 0);
		if (sendbytes == 0)
		{
			cerr << "Send failed . Client disconnected\n";
			return;
		}
	}

	fclose(fp);
	return;


}

void searchFaculty(SOCKET sock)
{
	FILE* fp = fopen(facultylist, "r+");
	info faculty;
	vector<info> facultyShomuho , foundFaculties;
	char stringToFindBuf[128];
	bool notfoundflag = TRUE;
	int matchCountThreshold;
	map<int, info> foundFacs;

	ZeroMemory(stringToFindBuf, 128);

	int bytesrecv = recv(sock, stringToFindBuf, 128, 0);
	if (bytesrecv == 0)
	{
		cerr << "Client disconnected";
		return;
	}

	fseek(fp, 0, 0);

	while (!feof(fp))
	{
		ZeroMemory(&faculty, sizeof(faculty));
		fread(&faculty, sizeof(faculty), 1, fp);
		if (faculty.phone_no == 0) break;
		facultyShomuho.push_back(faculty);

	}

	
		for (unsigned int i = 0; i < facultyShomuho.size(); i++)
		{
			if (facultyNameComp(facultyShomuho[i].name, stringToFindBuf) > 0)
			{
				foundFaculties.push_back(facultyShomuho[i]);
			}
			
		}
		
		
	ZeroMemory(&faculty, sizeof(faculty));
	foundFaculties.push_back(faculty);

	cout << "Faculties shown :" << foundFaculties.size() << endl;
	for (unsigned int i = 0; i < foundFaculties.size(); i++)
	{
		int sendbytes = send(sock, (char*)(&foundFaculties[i]), sizeof(faculty), 0);
		if (sendbytes == 0)
		{
			cerr << "Send failed . Client disconnected\n";
			return;
		}
	}

}

void sendHelp(SOCKET sock)
{
	FILE* fp = fopen(emergencylist, "r+");
	EmergencyServices service;
	fseek(fp, 0, 0);

	while (!feof(fp))
	{
		ZeroMemory(&service, sizeof(service));
		fread(&service, sizeof(service), 1, fp);
		int send_success = send(sock, (char*)&service, sizeof(service), 0);
		if (send_success == SOCKET_ERROR)
		{
			cerr << "Client disconnected, try to connect again\n";
			return;
		}
	}

	fclose(fp);
	return;


}

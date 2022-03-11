#include"ServerUtils.h"


SOCKET serverCreate()
{
	//Create a sock
	/*
		Socket just plugs a port and an IP
	*/
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return (SOCKET)-1;
	}


	//Bind the socket to an ip address and port
	/*
		Port and ip to a socket
	*/
	// Bind the ip address and port to a socket
	sockaddr_in hint;//Specifies transport and port for the AF_INET
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);//Host to Network short
	hint.sin_addr.S_un.S_addr = 0; //ALternative = inet_pton

	bind(listening, (sockaddr*)&hint, sizeof(hint)); //Binding done

	// Tell Winsock the socket is for listening
	listen(listening, SOMAXCONN); //SOMAXCONN is the maximum number in "listen"

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
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);  //Address to string translation
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	closesocket(listening);


	return clientSocket;

}

bool facultyDeptComp(const info a, const info b)
{
	int c = strcmp(a.department, b.department);
	if (c < 0) return TRUE;
	else return FALSE;
}

vector<string> substringer(char* str)
{
	vector<string> parts;
	char subs[5];

	if (strlen(str) < 3)
	{
		ZeroMemory(subs, 5);
		strncpy(subs, str, 2);
		string cpp_sub(subs);
		parts.push_back(cpp_sub);

	}
	else
	{
		for (int i = 0; i < strlen(str) - 2; i++)
		{
			ZeroMemory(subs, 5);
			strncpy(subs, str + i, 3);
			string cpp_sub(subs);
			parts.push_back(cpp_sub);
		}
	}
	return parts;

}


int facultyNameComp(char* match, char* toMatch)
{
	string name(match);
	string substring(toMatch);
	vector<string> parts;


	transform(name.begin(), name.end(), name.begin(), ::tolower);
	transform(substring.begin(), substring.end(), substring.begin(), ::tolower);

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


	std::sort(facultyShomuho.begin(), facultyShomuho.end(), facultyDeptComp);

	ZeroMemory(&faculty, sizeof(faculty));
	facultyShomuho.push_back(faculty);


	cout << "Faculties shown :" << facultyShomuho.size() << endl;
	for (unsigned int i = 0; i < facultyShomuho.size(); i++)
	{
		int sendbytes = send(sock, (char*)(&facultyShomuho[i]), sizeof(faculty), 0);
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
	vector<info> facultyShomuho, foundFaculties;
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
void sendHelp_vect(SOCKET sock)
{
	FILE* fp = fopen(emergencylist, "r+");
	EmergencyServices service;
	vector<EmergencyServices> serviceShomuho;
	fseek(fp, 0, 0);

	while (!feof(fp))
	{
		//Enters all the services as a vector for ease of access
		ZeroMemory(&service, sizeof(service));
		fread(&service, sizeof(service), 1, fp);
		if (service.contact == 0) break;
		serviceShomuho.push_back(service);

	}
	//Skipping sort to avoid complications.

	//Entering an empty or zero vector as a terminating condition.
	ZeroMemory(&service, sizeof(service));
	serviceShomuho.push_back(service);
	//Vector manipulation done//

	//Server console outputs:
	cout << "Emergency Services shown :" << serviceShomuho.size() << endl;
	for (unsigned int i = 0; i < serviceShomuho.size(); i++)
	{
		int sendbytes = send(sock, (char*)(&serviceShomuho[i]), sizeof(service), 0);
		if (sendbytes == 0)
		{
			cerr << "Send failed . Client disconnected\n";
			return;
		}
	}

	fclose(fp);
	return;


}

void sendPortal(SOCKET sock)
{
	FILE* fp = fopen(StudentPortal, "r+");
	studentPortal student;
	fseek(fp, 0, 0);

	while (!feof(fp))
	{
		ZeroMemory(&student, sizeof(student));
		fread(&student, sizeof(student), 1, fp);
		int send_success = send(sock, (char*)&student, sizeof(student), 0);
		if (send_success == SOCKET_ERROR)
		{
			cerr << "Client disconnected, try to connect again\n";
			return;
		}
	}

	fclose(fp);
	return;
}

void sendPortal_vect(SOCKET sock)
{
	FILE* fp = fopen(StudentPortal, "r+");
	studentPortal student;

	vector<studentPortal>studentShomuho;
	fseek(fp, 0, 0);

	while (!feof(fp))
	{
		//Enters all the services as a vector for ease of access
		ZeroMemory(&student, sizeof(student));
		fread(&student, sizeof(student), 1, fp);
		if (student.roll == 0) break;
		studentShomuho.push_back(student);

	}
	ZeroMemory(&student, sizeof(student));
	studentShomuho.push_back(student);
	//Vector manipulation done//

	//Server console outputs:
	cout << "Portals shown: " << studentShomuho.size() << endl;
	for (unsigned int i = 0; i < studentShomuho.size(); i++)
	{
		int sendbytes = send(sock, (char*)(&studentShomuho[i]), sizeof(student), 0);
		if (sendbytes == 0)
		{
			cerr << "Send failed . Client disconnected\n";
			return;
		}
	}

	fclose(fp);
	return;
}
//Hash Function
unsigned long long Hash(const char* str)
{
	unsigned long long x = 0;

	for (int i = 0; str[i] != '\0'; i++)
	{
		x = str[i] + (x << 6) + (x << 16) - x; //or multiplying by 65599
		//SDBM hash
	}
	return x;
}

void login_server(SOCKET sock, int* login_stat , int * login_index, vector<studentPortal> &allStudents)
{
	FILE* fp = fopen(StudentPortal, "r+");
	studentPortal student;
	logininfo log;
	char buf = 'S';


	fseek(fp, 0, 0);

	while (!feof(fp))
	{
		//Enters all the services as a vector for ease of access
		ZeroMemory(&student, sizeof(student));
		fread(&student, sizeof(student), 1, fp);
		if (student.roll == 0) break;
		allStudents.push_back(student);

	}
	ZeroMemory(&student, sizeof(student));
	allStudents.push_back(student);
	int recvbytes = recv(sock, (char*)&log, sizeof(logininfo), 0);
	if (recvbytes == 0)
	{
		cout << "Login Failed";
		return;
	}

	for (int i = 0; i < allStudents.size(); i++)
	{
		if (allStudents[i].hash == log.hash)
		{
			int s = send(sock, &buf, sizeof(buf), 0);
			if (s == SOCKET_ERROR)
			{
				cout << "Send Failed\n";
			}
			*login_index = i;
			*login_stat = 1;
			return;
		}
	}

	buf = 'F';
	int s = send(sock, &buf, sizeof(buf), 0);
	if (s == SOCKET_ERROR)
	{
		cout << "Send Failed\n";

	}
	return;

}
void buyTickets(SOCKET sock, int* login_index, vector<studentPortal> &allStudents)
{
	char buff;
	int bytesRecv = recv(sock, &buff, sizeof(buff), 0);

	if (bytesRecv == 0)
	{
		cerr << "Error buying tickets\n";
		return;
	}
	
	if (buff == 'L')
	{
		cout << allStudents[*login_index].balance << endl;

		allStudents[*login_index].balance -= 70;

		
		
		rePrint(allStudents);

		cout << allStudents[*login_index].balance << endl;
	}

	else if (buff == 'S')
	{
		cout << allStudents[*login_index].balance << endl;

		allStudents[*login_index].balance -= 80;

		
		rePrint(allStudents);

		cout << allStudents[*login_index].balance << endl;
	}

}

void rePrint(vector <studentPortal>& allStudents)
{
	remove(StudentPortal);
	FILE* fp = fopen(StudentPortal, "w");

	studentPortal student;
	fseek(fp, 0, 0);
	for (unsigned int i = 0; i < allStudents.size(); i++)
	{
		student = allStudents[i];

		fwrite(&student, sizeof(student), 1, fp);
	}

	fclose(fp);
	return;
}

void admissionQuery(SOCKET sock)
{
	
}
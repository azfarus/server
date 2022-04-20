#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define facultylist "F:\\ProjectDataBase\\FACULTY.txt" 
#define emergencylist "F:\\ProjectDataBase\\EmergencyServices.txt"
#define StudentPortal "F:\\ProjectDataBase\\StudentPortal.txt"

#pragma comment (lib, "ws2_32.lib")

using namespace std;

typedef struct _Info
{
	char name[50], address[100], department[10];
	unsigned long long int phone_no;
	int flag;


} info;

typedef struct _emergency {
	char service[50];
	unsigned long long int contact;
	bool flag;
} EmergencyServices;

//Struct for student portal
typedef struct _student {
	unsigned long long hash;
	unsigned long long roll;
	char name[100], email[50], father[100], mother[100], dept[4];
	double CG;
	long long int balance;
}studentPortal;

typedef struct _logininfo
{
	unsigned long long id;
	unsigned long long hash;
}logininfo;


SOCKET serverCreate();
void sendFaculty(SOCKET sock);
bool facultyDeptComp(info a, info b);
void searchFaculty(SOCKET sock);
void sendHelp(SOCKET sock);
void sendHelp_vect(SOCKET sock);
int facultyNameComp(char* match, const char* toMatch);

vector<string> substringer(char* str);

//New Functions
void sendPortal(SOCKET sock);
void sendPortal_vect(SOCKET sock);

void login_server(SOCKET sock, int* login_stat, int* login_index, vector<studentPortal>& allStudents);

unsigned long long Hash(const char* str);
void buyTickets(SOCKET sock, int* login_index, vector<studentPortal>&allStudents);

void rePrint(vector<studentPortal> &allStudents);

void chat(SOCKET sock);
using namespace std;
#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define facultylist "E:\\codes\\utility codes\\utility codes\\FACULTY.txt" 
#define emergencylist "G:\\Project Works\\Socket project\\utility codes\\EmergencyServices.txt"

#pragma comment (lib, "ws2_32.lib")

using namespace std;

typedef struct _Info
{
	char name[50], address[100], department[10];
	unsigned long long int phone_no ;
	int flag;


} info;

typedef struct _emergency {
	char service[50];
	unsigned long long int contact;
	bool flag;
} EmergencyServices;


SOCKET serverCreate();
void sendFaculty(SOCKET sock);
bool facultyDeptComp(info a, info b);
void searchFaculty(SOCKET sock);
void sendHelp(SOCKET sock);
int facultyNameComp(char* match, const char* toMatch);
vector<string> substringer(char* str);
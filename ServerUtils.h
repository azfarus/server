#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <algorithm>


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define facultylist "G:\\Project Works\\Socket project\\utility codes\\FACULTY.txt"  

#pragma comment (lib, "ws2_32.lib")

using namespace std;

typedef struct _Info
{
	char name[50], address[100], department[10];
	unsigned long long int phone_no ;
	int flag;


} info;


SOCKET serverCreate();
void sendFaculty(SOCKET sock);
bool facultyComp(info a, info b);

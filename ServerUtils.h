#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "ws2_32.lib")

using namespace std;

typedef struct _Info
{
	char name[50], address[100], department[10];
	int phone_no ;
	int flag;


} info;

typedef struct _emergency {
	char service[50];
	int contact, flag;
} EmergencyServices;

SOCKET serverCreate();
void sendFaculty(SOCKET sock);
void sendHelp(SOCKET sock);
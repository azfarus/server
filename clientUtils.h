#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment (lib, "ws2_32.lib")

using namespace std;

SOCKET serverCreate();

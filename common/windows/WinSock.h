#pragma once

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <string>
using namespace std;

int WinSock::ref_count = 0;
class WinSock
{
public:
	WinSock();
	int Bind(string addr, string port);
	int Connect(string addr, string port);
	int Listen(int que_len);
	WinSock Accept(void);
	int Send(const char *buf, size_t size);
	int Receive(char *buf, size_t size);
	virtual ~WinSock();
private:
	WSADATA wsaData;
	SOCKET Socket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	int iResult;
	static int ref_count;
};


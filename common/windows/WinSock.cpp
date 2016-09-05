#include "WinSock.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

WinSock::WinSock()
{
	if (ref_count++ == 0)
	{
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			printf("WSAStartup failed with error: %d\n", iResult);
		}
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
}

int WinSock::Connect(string addr, string port)
{
	// Resolve the server address and port
	iResult = getaddrinfo(addr.c_str, port.c_str, &hints, &result);
	if (iResult != 0) 
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
	{

		// Create a SOCKET for connecting to server
		Socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (Socket == INVALID_SOCKET)
		{
			printf("socket failed with error: %ld\n", WSAGetLastError());
			return 1;
		}

		// Bind to server.
		iResult = connect(Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) 
		{
			Socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (Socket == INVALID_SOCKET) 
	{
		printf("Unable to connect to server!\n");
		return 1;
	}
	return 0;
}
int WinSock::Listen(int que_len)
{
	iResult = listen(Socket, que_len);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen function failed with error: %d\n", WSAGetLastError());
		return 1;
	}
	return 0;
}
WinSock WinSock::Accept(void)
{
	WinSock newWinSock;
	iResult = newWinSock.Socket = accept(Socket, NULL, NULL);
	if (iResult == INVALID_SOCKET)
	{
		printf("accept failed with error: %ld\n", WSAGetLastError());
		return newWinSock;
	}
	return newWinSock;
}
int WinSock::Send(const char *buf, size_t size)
{
	// Send an initial buffer
	iResult = send(Socket, buf, size, 0);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		return 1;
	}
	return 0;
}
int WinSock::Receive(char *buf, size_t size)
{
	iResult = 0;
	// Receive until no data are received
	do 
	{
		iResult = recv(Socket, buf + iResult, size - iResult, 0);
	} while (iResult > 0);
	return iResult;
}
WinSock::~WinSock()
{
	// Close socket and cleanup
	if (Socket != INVALID_SOCKET)
	{
		closesocket(Socket);
	}
	if (--ref_count == 0)
	{
		WSACleanup();
	}
}

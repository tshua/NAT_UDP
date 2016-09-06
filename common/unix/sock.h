#ifndef _SOCK_H_
#define _SOCK_H_

#include "protocol.h"
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <list>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
using namespace std;

class Sock
{
public:
	char sendBuf[MAX_PACKET_SIZE];
	char recvBuf[MAX_PACKET_SIZE];
	int send_num;
	int recv_num;
	
	Sock();
	Sock(int local_port, int remote_port, string remote_ip);
	sockaddr_in& get_localaddr();
	sockaddr_in& get_remoteaddr();
	void set_localaddr(int local_port);
	void set_remoteaddr(int remote_port, string remote_ip);
	void set_remoteaddr(sockaddr_in remoteaddr);
	int _bind();
	int recv();
	int send(int length);
private:
	int sockfd;
	sockaddr_in localaddr;
	sockaddr_in remoteaddr;
};

#endif
#ifndef _SERVER_MESSAGE_PARSER_H_
#define _SERVER_MESSAGE_PARSER_H_
#include "protocol.h"
#include "sock.h"
class ServerMessageParser
{
public:
	UserList clientList;
	char *sendBuf;
	char *recvBuf;
	sockaddr_in *remoteaddr;
	
	ServerMessageParser();
	ServerMessageParser(char* sendBuf, char* recvBuf, sockaddr_in *remoteaddr);
	virtual ~ServerMessageParser();
	stUserInfo* getUserByName(const char *userName);
	stUserInfo* getUserByIP(const unsigned int& userIP);
	int parse();

};
#endif

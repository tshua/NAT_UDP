#ifndef _UNIX_CLIENT_MESSAGE_PARSER_H_
#define _UNIX_CLIENT_MESSAGE_PARSER_H_
#include "../common/protocol.h"
#include "../common/unix/sock.h"
class UnixClientMessageParser
{
public:
	UserList clientList;
	char *sendBuf;
	char *recvBuf;
	sockaddr_in *remoteaddr;
	UnixClientMessageParser();
	UnixClientMessageParser(char* sendBuf, char* recvBuf, sockaddr_in *remoteaddr);
	virtual ~UnixClientMessageParser();
	stUserInfo* getUserByName(const char* userName);
	stUserInfo* getUserByIP(unsigned int IP);
	int login(string username);
	int logout(string username);
	int parse();
};
#endif
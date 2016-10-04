#ifndef _UNIX_CLIENT_H_
#define _UNIX_CLIENT_H_
#include "unix_client_message_parser.h"
#include "../common/unix/sock.h"

class UnixClient
{
public :
	Sock sock;
	UnixClientMessageParser messageParser;
	bool recvACK;
	string myName;
	stUserInfo* connectedUser;

	UnixClient();
	UnixClient(int local_port);
	virtual ~UnixClient();
	void login();
	void parseMessage();
	bool sendMessage(const char* username, const char* message);
	int parseCommoned(string commond);
	void outputMessage();
};
#endif
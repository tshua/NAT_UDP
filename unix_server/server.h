#ifndef _SERVER_H_
#define _SERVER_H_

#include "sock.h"
#include "server_message_parser.h"

class Server
{
public :
	Sock sock;
	ServerMessageParser messageParser;
	Server();
	Server(int local_port);
	void start();

};

#endif
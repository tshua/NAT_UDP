#ifndef H_SERVER_H
#define H_SERVER_H

#include "../common/sock.h"
#include "server_message_parser.h"

class Server
{
	public :
		Sock sock;
		ServerMessageParser messageParser;

		Server()
		{
			messageParser.sendBuf = sock.sendBuf;
			messageParser.recvBuf = sock.recvBuf;
			messageParser.remoteaddr = &(sock.get_remoteaddr());
		}

		Server(int local_port)
		{
			sock.set_localaddr(local_port);
			messageParser.sendBuf = sock.sendBuf;
			messageParser.recvBuf = sock.recvBuf;
			messageParser.remoteaddr = &(sock.get_remoteaddr());

		}

		void start()
		{
			sock._bind();
			while(1)
			{
				memset(sock.recvBuf, 0, MAX_PACKET_SIZE);
				sock.recv();
				int size = messageParser.parse();
				if(size < 0)
				{
					cout<< "Server init error" <<endl;
					exit(0);
				}
				else if(size > 0)
				{
					sock.send(size);
				}
			}
		}

};



#endif

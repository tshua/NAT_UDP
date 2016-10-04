#include "server.h"

Server::Server()
{
	messageParser.sendBuf = sock.sendBuf;
	messageParser.recvBuf = sock.recvBuf;
	messageParser.remoteaddr = &(sock.get_remoteaddr());
}

Server::Server(int local_port)
{
	sock.set_localaddr(local_port);
	messageParser.sendBuf = sock.sendBuf;
	messageParser.recvBuf = sock.recvBuf;
	messageParser.remoteaddr = &(sock.get_remoteaddr());

}

void Server::start()
{
	sock._bind();
	while(1)
	{
		memset(sock.recvBuf, 0, MAX_PACKET_SIZE);
		sock.recv();
		int size = messageParser.parse();
		if(size == -2) //广播登录消息
		{
			char* sendBuf = sock.sendBuf;
			sockaddr_in& remoteaddr = sock.get_remoteaddr();

			memset(sendBuf, 0, MAX_PACKET_SIZE);
			stMsg* sendmsg = (stMsg*) sendBuf;

			sendmsg->msgType = CMD_USERLIST_RESP_QUIET;
			for (UserList_Iter it = messageParser.clientList.begin(); it != messageParser.clientList.end(); ++it)
			{
				memcpy(sendmsg->userList.users[sendmsg->userList.userNums].userName, (*it)->userName,MAX_NAME_SIZE);
				sendmsg->userList.users[sendmsg->userList.userNums].userIP = (*it)->userIP;
				sendmsg->userList.users[sendmsg->userList.userNums].userPort = (*it)->userPort;
				++sendmsg->userList.userNums;
			}
			
			int size = sizeof(unsigned char) + offsetof(stMsg, userList) + sizeof(unsigned int) + sendmsg->userList.userNums * sizeof(stUserInfo);  
			
			for (UserList_Iter it = messageParser.clientList.begin(); it != messageParser.clientList.end(); ++it)
			{
				remoteaddr.sin_family = AF_INET;
				remoteaddr.sin_port = htons((*it)->userPort);
				remoteaddr.sin_addr.s_addr = htonl((*it)->userIP);

				sock.send(size);
			}

		}
		else if(size < 0)
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



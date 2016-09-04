#ifndef UNIX_CLIENT
#define UNIX_CLIENT

#include "unix_client_message_parser.h"
#include "../common/sock.h"

class UnixClient
{
	public :
		Sock sock;
		UnixClientMessageParser messageParser;
		bool recvACK;
		string myName;
		stUserInfo* connectedUser;

		UnixClient():sock(LOCAL_PORT, SERVER_PORT, SERVER_IP)
		{
			messageParser.sendBuf = sock.sendBuf;
			messageParser.recvBuf = sock.recvBuf;
			messageParser.remoteaddr = &(sock.get_remoteaddr());
			recvACK = false;
			sock._bind();
			connectedUser = NULL;
		}

		UnixClient(int local_port):sock(local_port, SERVER_PORT, SERVER_IP)
		{
			messageParser.sendBuf = sock.sendBuf;
			messageParser.recvBuf = sock.recvBuf;
			messageParser.remoteaddr = &(sock.get_remoteaddr());	
			recvACK = false;
			sock._bind();
			connectedUser = NULL;
		}
		void login()
		{
			sock.send(messageParser.login(myName));
		}

		void parseMessage()
		{
			while(1)
			{
				memset(sock.recvBuf, 0 ,MAX_PACKET_SIZE);
				sock.recv();
				int size =  messageParser.parse();
				if(size == -2 )
				{
					recvACK = true;
				}
				else if(size == -3)//回复ack的时候，设置连接对象
				{
					connectedUser = messageParser.getUserByIP(ntohl(sock.get_remoteaddr().sin_addr.s_addr));
					size = sizeof(unsigned char);
					sock.send(size);
				}
				else if(size < 0)
				{
					cout<< "client error" <<endl;
					exit(0);
				}
				else if(size > 0)
				{
					sock.send(size);
				}
			}
		}

		bool sendMessage(const char* username, const char* message)
		{
			recvACK = false;
			stUserInfo* user = messageParser.getUserByName(username);

			for(int trytime = 0; trytime < MAXRETYR; trytime++)
			{

				memset(sock.sendBuf, 0, MAX_PACKET_SIZE);
				stMsg* sendmsg = (stMsg*) sock.sendBuf;
				sendmsg->msgType = MESSAGE;
				sendmsg->message.length = strlen(message) + 1;
				strcpy(sendmsg->message.content, message);

				sock.get_remoteaddr().sin_addr.s_addr = htonl(user->userIP);
				sock.get_remoteaddr().sin_port = htons(user->userPort);
				sock.get_remoteaddr().sin_family = AF_INET;

				int size = sizeof(unsigned char) + offsetof(stMsg, message) + sizeof(unsigned int) + sendmsg->message.length;
				sock.send(size);

				//等待接收消息线程改RecvedACK标志
				for(int i=0;i<5;i++)
				{
					if(recvACK)
						return true;
					else	
						sleep(1);
				}

				//如果没有接收到目标主机的回应，认为目标主机的端口
				//认为目标主机的端口//映射没有打开，那么发送请求到服务器要求“打洞”。
				sock.get_remoteaddr().sin_family=AF_INET;
				sock.get_remoteaddr().sin_port = htons(SERVER_PORT);
				sock.get_remoteaddr().sin_addr.s_addr = inet_addr(SERVER_IP);

				memset(sock.sendBuf, 0, MAX_PACKET_SIZE);
				sendmsg  = (stMsg*)sock.sendBuf;

				sendmsg->msgType = CMD_PUNCH_REQU;
				strcpy(sendmsg->toUserName, user->userName);
				size = sizeof(unsigned int) + offsetof(stMsg, toUserName) + MAX_NAME_SIZE;
				sock.send(size);

				sleep(1);

			}
			return false;
		}

		
		int parseCommoned(string commond)
		{

			if(commond == "q")
			{
				sock.send(messageParser.logout(myName));
				return -1;
			}
			else if(commond == "p")
			{
				sock.get_remoteaddr().sin_family = AF_INET;
				sock.get_remoteaddr().sin_port = htons(SERVER_PORT);
				sock.get_remoteaddr().sin_addr.s_addr = inet_addr(SERVER_IP);

				memset(sock.sendBuf, 0, MAX_PACKET_SIZE);
				stMsg* sendmsg = (stMsg*) sock.sendBuf;
				sendmsg->msgType = CMD_USERLIST_REQU;
				sock.send(sizeof(unsigned char));

				sleep(1);// 等待接收线程打印在线用户

			}
			else
			{
				stUserInfo* user = messageParser.getUserByName(commond.c_str());
				if(user)
				{
					char message[20] = "punch data";
					if(sendMessage(commond.c_str(), message))
					{
						connectedUser = user;
						cout << "punch Success!" <<endl;
					}
					else
					{
						cout<< "punch Failure!" <<endl;;
					}
				}
				else
				{

					if(connectedUser)
					{
						sendMessage(connectedUser->userName, commond.c_str());
					}
					else
						cout << "command error, input once more!" <<endl;
				}

			}
			return 0;

		}

		void  outputMessage()
		{
			cout<< "You can input you command:" <<endl;;
			cout<< "q for exit \\ p for print the hosts \\ or input the hostname you want to connect \\ if you have already connected success, just type in the message." <<endl; 
		}
};

#endif

#ifndef H_UNIX_CLIENT_MESSAGE_PASER
#define H_UNIX_CLIENT_MESSAGE_PASER

#include "../common/protocol.h"

class UnixClientMessageParser
{
	public:

		UserList clientList;
		char *sendBuf;
		char *recvBuf;
		sockaddr_in *remoteaddr;

		UnixClientMessageParser(){
			sendBuf = recvBuf = NULL;
		}

		UnixClientMessageParser(char* sendBuf, char* recvBuf, sockaddr_in *remoteaddr){
			this->sendBuf = sendBuf;
			this->recvBuf = recvBuf;
			this->remoteaddr = remoteaddr;
		}

		~UnixClientMessageParser()
		{
			UserList_Iter it;
			for ( it = clientList.begin() ; it != clientList.end(); it++ )
				delete *it;
		}

		stUserInfo* getUserByName(const char* userName)
		{
			UserList_Iter it; 
			for( it = clientList.begin(); it != clientList.end(); it++  )
			{
				if(strcmp((*it)->userName, userName) == 0)
				{
					return (*it);

				}

			}
	//		cout<< "cant't find user" << userName <<endl;
			return NULL;

		}

		stUserInfo* getUserByIP(unsigned int IP)
		{
			UserList_Iter it; 
			for( it = clientList.begin(); it != clientList.end(); it++  )
			{
				if((*it)->userIP == IP)
				{
					return (*it);

				}

			}
			cout<< "cant't find user"  <<endl;
			return NULL;

		}




		int login(string username)
		{
			remoteaddr->sin_family = AF_INET;
			remoteaddr->sin_port = htons(SERVER_PORT);
			remoteaddr->sin_addr.s_addr = inet_addr(SERVER_IP);
			memset(sendBuf, 0, MAX_PACKET_SIZE);

			stMsg* sendmsg = (stMsg*)sendBuf; 
			sendmsg->msgType = CMD_LOGIN;
			memcpy(sendmsg->loginInfo.userName, username.c_str(), username.length());
			int size = sizeof(unsigned char) + offsetof(stMsg, loginInfo) + sizeof(stUserInfo);
			return size;
		}

		int logout(string username)
		{
			remoteaddr->sin_family = AF_INET;
			remoteaddr->sin_port = htons(SERVER_PORT);
			remoteaddr->sin_addr.s_addr = inet_addr(SERVER_IP);
			memset(sendBuf, 0, MAX_PACKET_SIZE);

			stMsg* sendmsg = (stMsg*)sendBuf; 
			sendmsg->msgType = CMD_LOGOUT;
			memcpy(sendmsg->logoutUserName, username.c_str(), username.length());	

			return sizeof(unsigned char) + offsetof(stMsg, logoutUserName) + MAX_NAME_SIZE*sizeof(char);
		}



		int parse()
		{
			if(sendBuf == NULL || recvBuf == NULL)
				return -1;

			stMsg* recvmsg = (stMsg*)recvBuf;
			unsigned char msgType = recvmsg->msgType;
			switch(msgType)
			{
				case CMD_USERLIST_RESP:
					{
						unsigned int usernum = recvmsg->userList.userNums;
						clientList.clear();
						cout<< "online hosts:" <<endl;

						for(unsigned int i = 0; i < usernum; i++)
						{
							stUserInfo* user = new stUserInfo();
							memcpy(user->userName, recvmsg->userList.users[i].userName, MAX_NAME_SIZE);
							user->userIP = recvmsg->userList.users[i].userIP;
							user->userPort = recvmsg->userList.users[i].userPort;
							cout<< user->userName <<endl;
							clientList.push_back(user);
						}
						return 0;
					}
				case  CMD_SOMEONEWANTTOCALLYOU:
					{ 
						//接收到打洞命令，向指定IP打洞
						cout<< "Recv someonewanttocallyou data(from server)!" <<endl;


						remoteaddr->sin_family = AF_INET;
						remoteaddr->sin_port = htons(recvmsg->toUserInfo.userPort);
						remoteaddr->sin_addr.s_addr = htonl(recvmsg->toUserInfo.userIP);

						//UDP hole punching
						memset(sendBuf, 0, MAX_PACKET_SIZE);
						stMsg* sendmsg = (stMsg*)sendBuf;
						sendmsg->msgType = MESSAGE_PUNCH;

						return sizeof(unsigned char);
					}
				case MESSAGE_PUNCH:
					{
						//忽略对方的打洞包
						cout<< "recv punch data !" <<endl;
						cout<< "connect is build !" <<endl;
						return 0;
					}
				case MESSAGE_ACK:
					{
						cout<< "recv msgack data !"<<endl;
						cout<< "connect is build !"<<endl;
						return -2;
					}
				case MESSAGE://回 ack
					{
				
						stUserInfo* user  = getUserByIP(ntohl(remoteaddr->sin_addr.s_addr));
						cout<< "Message is from:" << user->userName <<":" << inet_ntoa(remoteaddr->sin_addr) << endl;
						cout<< recvmsg->message.content <<endl;

						memset(sendBuf, 0, MAX_PACKET_SIZE);
						stMsg* sendmsg = (stMsg*) sendBuf;
						sendmsg->msgType = MESSAGE_ACK;
						return sizeof(unsigned char);
					}

			}
			return 0;

		}
};





#endif

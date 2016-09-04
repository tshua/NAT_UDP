#ifndef H_SERVER_MESSAGE_PASER_H
#define H_SERVER_MESSAGE_PASER_H

#include "../common/protocol.h"


class ServerMessageParser
{
	public:

		UserList clientList;
		char *sendBuf;
		char *recvBuf;
		sockaddr_in *remoteaddr;

		ServerMessageParser(){
			sendBuf = recvBuf = NULL;
		}

		ServerMessageParser(char* sendBuf, char* recvBuf, sockaddr_in *remoteaddr){
			this->sendBuf = sendBuf;
			this->recvBuf = recvBuf;
			this->remoteaddr = remoteaddr;
		}

		~ServerMessageParser()
		{
			UserList_Iter it;
			for ( it = clientList.begin() ; it != clientList.end(); it++  )
				delete *it;
		}
		
		stUserInfo* getUserByName(const char *userName)
		{
			UserList_Iter it;
			for( it = clientList.begin(); it != clientList.end(); it++ )
			{
				if(strcmp((*it)->userName, userName) == 0)
				{
					return (*it);
				}
			}
			cout<< "cant't find user" << userName <<endl;
			return NULL;
		}

		stUserInfo* getUserByIP(const unsigned int& userIP)
		{
			UserList_Iter it;
			for( it = clientList.begin(); it != clientList.end(); it++ )
			{
				if((*it)->userIP == userIP)
				{
					return (*it);
				}
			}
			cout<< "can't find user" << endl;
			return NULL;
		}


		int parse()
		{
			if(sendBuf == NULL || recvBuf == NULL)
				return -1;


			stMsg* recvmsg = (stMsg*)recvBuf;
			unsigned char msgType = recvmsg->msgType;
			switch(msgType)
			{
				case CMD_LOGIN:
					{

						cout << "has a client login, user name: " << recvmsg->loginInfo.userName << ":" << inet_ntoa(remoteaddr->sin_addr) \
							<< ":" << ntohs(remoteaddr->sin_port) << endl;
						stUserInfo* currentUser = new stUserInfo(recvmsg->loginInfo);
						currentUser->userIP = ntohl(remoteaddr->sin_addr.s_addr); //转换成主机字节序存储
						currentUser->userPort = ntohs(remoteaddr->sin_port);
						clientList.push_back(currentUser);                       //do not exclude same name user
						return -2;
						
					}
				case CMD_LOGOUT:
					{
						cout << "a client logout, user name: " << recvmsg->logoutUserName << endl;
						stUserInfo* currentUser = getUserByName(recvmsg->logoutUserName);
						clientList.remove(currentUser);
						
						return 0;
					}
				case CMD_USERLIST_REQU:
					{

						memset(sendBuf, 0, MAX_PACKET_SIZE);
						stMsg* sendmsg = (stMsg*) sendBuf;

						sendmsg->msgType = CMD_USERLIST_RESP;
						for (UserList_Iter it = clientList.begin(); it != clientList.end(); ++it)
						{
							memcpy(sendmsg->userList.users[sendmsg->userList.userNums].userName, (*it)->userName,MAX_NAME_SIZE);
							sendmsg->userList.users[sendmsg->userList.userNums].userIP = (*it)->userIP;
							sendmsg->userList.users[sendmsg->userList.userNums].userPort = (*it)->userPort;
							++sendmsg->userList.userNums;
						}
						int size = sizeof(unsigned char) + offsetof(stMsg, userList) + sizeof(unsigned int) + sendmsg->userList.userNums * sizeof(stUserInfo);  
						return size;
					}
				case CMD_PUNCH_REQU:
					{
						memset(sendBuf, 0, MAX_PACKET_SIZE);
						stMsg* sendmsg = (stMsg*) sendBuf;
						
						stUserInfo* currentUser = getUserByIP(ntohl(remoteaddr->sin_addr.s_addr));
						
						cout<< currentUser->userName << "want to connect to " << recvmsg->toUserName <<endl;
						stUserInfo* toUser = getUserByName(recvmsg->toUserName);	

						sendmsg->msgType = CMD_SOMEONEWANTTOCALLYOU;
						sendmsg->toUserInfo.userIP = ntohl(remoteaddr->sin_addr.s_addr);
						sendmsg->toUserInfo.userPort = ntohs(remoteaddr->sin_port);

						remoteaddr->sin_family = AF_INET;
						remoteaddr->sin_port = htons(toUser->userPort);
						remoteaddr->sin_addr.s_addr = htonl(toUser->userIP);
						
						return sizeof(unsigned char) + offsetof(stMsg, toUserInfo) + sizeof(stUserInfo);
					}
			}
			return 0;
		}

};



#endif

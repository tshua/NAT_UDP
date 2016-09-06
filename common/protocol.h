#ifndef H_PROTOCOL_H
#define H_PROTOCOL_H

#include <list>

#define SERVER_PORT 	8000
#define SERVER_IP 	"45.78.43.221"

#define LOCAL_PORT 	5000
#define LOCAL_IP  	"172.16.23.10"


#define MAXRETYR 	3
#define MAX_PACKET_SIZE 1024
#define MAX_NAME_SIZE   15


#define CMD_LOGIN 	0x01 		//客户端->服务器  登录
#define CMD_LOGOUT 	0x02 		//客户端->服务器  登出
#define CMD_USERLIST_REQU 	0x03  	//客户端->服务器  请求在线用户列表
#define CMD_USERLIST_RESP 	0x04 	//服务器->客户端  用户列表应答
#define CMD_PUNCH_REQU 		0x05 	//客户端->服务器  请求服务器命令某个客户端发送打洞包
#define CMD_SOMEONEWANTTOCALLYOU   0x06  	//服务器->客户端  要求次客户端发送udp打洞包
#define CMD_USERLIST_RESP_QUIET    0x10  //服务器->客户端  	后台更新用户列表，不显示

#define MESSAGE 		0x07 	//客户端->客户端  发送消息
#define MESSAGE_ACK 		0x08 	//客户端->客户端  收到消息的应答
#define MESSAGE_PUNCH 		0x09 	//客户端->客户端  打洞包，接受端不处理

//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

//用户信息
struct stUserInfo
{
 	char userName[MAX_NAME_SIZE];
	unsigned int userIP;
	unsigned int userPort;

	//stUserInfo()
	//{
	//	bzero(this, sizeof(*this));
	//}
};

//server命令客户端打洞请求消息
struct stMsgPunchCmd
{
	unsigned int userIP;
	unsigned int userPort;

	//stMsgPunchCmd()
	//{
	//	userIP = 0;
	//	userPort = 0;
	//}
};


struct stUserList
{
	unsigned int userNums;
	stUserInfo users[0];	
};

struct stMessage
{
	unsigned int length;
	char content[0];

};
struct stMsg
{
	unsigned char msgType;
	union
	{
		char logoutUserName[MAX_NAME_SIZE];
		char toUserName[MAX_NAME_SIZE]; 	//客户想要通信的另一客户端的名字
		stUserInfo loginInfo;
		stUserList userList;
		stUserInfo toUserInfo;
		stMessage message;
	};
};


typedef std::list<stUserInfo*> UserList;
typedef std::list<stUserInfo*>::iterator UserList_Iter;

#endif

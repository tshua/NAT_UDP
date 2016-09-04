#ifndef H_SOCK_H
#define H_SOCK_H

#include "protocol.h"
#include <sys/socket.h>
#include <string>



class Sock
{
	int sockfd;
	sockaddr_in localaddr;
	sockaddr_in remoteaddr;



	public:
	
	char sendBuf[MAX_PACKET_SIZE];
	char recvBuf[MAX_PACKET_SIZE];
	int send_num;
	int recv_num;
	
	Sock()
	{
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		bzero(recvBuf, MAX_PACKET_SIZE);
		bzero(sendBuf, MAX_PACKET_SIZE);
		bzero(&remoteaddr, sizeof(remoteaddr));
		send_num = recv_num = 0;
	}

	Sock(int local_port, int remote_port, string remote_ip)
	{
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		
		localaddr.sin_family = AF_INET;
		localaddr.sin_port = htons(local_port);
		localaddr.sin_addr.s_addr = htonl(INADDR_ANY);

		remoteaddr.sin_family = AF_INET;
		remoteaddr.sin_port = htons(remote_port);
		remoteaddr.sin_addr.s_addr = inet_addr(remote_ip.c_str());
		
		bzero(recvBuf, MAX_PACKET_SIZE);
		bzero(sendBuf, MAX_PACKET_SIZE);
		send_num = recv_num = 0;
	}

	sockaddr_in& get_localaddr()
	{
		return localaddr;
	}

	sockaddr_in& get_remoteaddr()
	{
		return remoteaddr;
	}

	

	void set_localaddr(int local_port)
	{
		localaddr.sin_family = AF_INET;
		localaddr.sin_port = htons(local_port);
		localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	
	void set_remoteaddr(int remote_port, string remote_ip)
	{
		remoteaddr.sin_family = AF_INET;
		remoteaddr.sin_port = htons(remote_port);
		remoteaddr.sin_addr.s_addr = inet_addr(remote_ip.c_str());
	}

	void set_remoteaddr(sockaddr_in remoteaddr)
	{
		this->remoteaddr = remoteaddr;
	}

	int _bind()
	{
		if(bind(sockfd, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0)
		{
			cout<< "bind socket failed!" <<endl;
			return -1;
		}
		return 0;
	}

	int recv()
	{
		unsigned int nLen = sizeof(remoteaddr);
		recv_num = recvfrom(sockfd, recvBuf , MAX_PACKET_SIZE, 0, (sockaddr*)&remoteaddr,&nLen);
		//cout<< ntohs(remoteaddr.sin_port) <<endl;
		//cout<< remoteaddr.sin_addr.s_addr <<endl;


		if(recv_num < 0)
		{
			std::cout << "recv error " << std::endl;
			return -1;
		}	
		return 0;		
	}
	

	int send(int length)
	{
		send_num = sendto(sockfd, sendBuf, length, 0, (sockaddr*)&remoteaddr, sizeof(remoteaddr));
		if(send_num < 0)
		{
			std::cout<< "send error " <<std::endl;
			return -1;
		}
		return 0;
	}


};



#endif

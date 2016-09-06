#include "sock.h"

Sock::Sock()
{
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(recvBuf, MAX_PACKET_SIZE);
	bzero(sendBuf, MAX_PACKET_SIZE);
	bzero(&remoteaddr, sizeof(remoteaddr));
	send_num = recv_num = 0;
}
Sock::Sock(int local_port, int remote_port, string remote_ip)
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
sockaddr_in& Sock::get_localaddr()
{
	return localaddr;
}
sockaddr_in& Sock::get_remoteaddr()
{
	return remoteaddr;
}
void Sock::set_localaddr(int local_port)
{
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(local_port);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
}
void Sock::set_remoteaddr(int remote_port, string remote_ip)
{
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_port = htons(remote_port);
	remoteaddr.sin_addr.s_addr = inet_addr(remote_ip.c_str());
}
void Sock::set_remoteaddr(sockaddr_in remoteaddr)
{
	this->remoteaddr = remoteaddr;
}
int Sock::_bind()
{
	if(bind(sockfd, (struct sockaddr*)&localaddr, sizeof(localaddr)) < 0)
	{
		cout<< "bind socket failed!" <<endl;
		return -1;
	}
	return 0;
}
int Sock::recv()
{
	socklen_t nLen = sizeof(remoteaddr);
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
int Sock::send(int length)
{
	send_num = sendto(sockfd, sendBuf, length, 0, (sockaddr*)&remoteaddr, sizeof(remoteaddr));
	if(send_num < 0)
	{
		std::cout<< "send error " <<std::endl;
		return -1;
	}
	return 0;
}
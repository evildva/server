#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <iostream>
#include <stdio.h>//perror
#include <sys/types.h>
#include <sys/socket.h>
#include <bits/socket.h>
#include <unistd.h>//close

#include "error.hpp"

using namespace ERROr;

namespace SOCKEt{

	class Server
	{
	public:
		Server(int protocol_family,int tcp_udp,int protocol=0);
		~Server();
		
		int sockfd;
		/*
		PF_INET(ipv4)/PF_INET6(ipv6) PF_UNIX(UNIX)
		SOCK_STREAM(tcp)/SOCK_UGRAM(udp) >2.6.17时,可以与(&) SOCK_NONBLOCK设为非阻塞, SOCK_CLOEXEC在fork的子进程中关闭socket < fcntl
		始终应为0
		成功返回fd,失败返回-1
		*/
		int Socket(int protocol_family,int tcp_udp,int protocol=0);

		/*
		成功返回0,失败返回-1
		*/
		int Bind(int sockfd,const sockaddr* addr,socklen_t addrlen);

		/*
		成功返回0,失败返回-1
		*/
		int Listen(int sockfd,int max_listen=5);

		/*
		成功返回newfd,失败返回-1
		*/
		int Accept(int sockfd,struct sockaddr* addr,socklen_t* addrlen);

		int Close(int sockfd);

		/*
		how: SHUT_RD 关闭读 SHUT_WR 关闭写 SHUT_RDWR 全部关闭
		成功返回newfd,失败返回-1
		*/
		int Shutdown(int sockfd,int how);

	};

	Server::Server(int protocol_family,int tcp_udp,int protocol):sockfd(0){
		this->sockfd=Socket(protocol_family,tcp_udp,protocol);
	}

	Server::~Server(){
		if(this->sockfd!=0){
			Shutdown(this->sockfd,SHUT_RDWR);
		}
	}

	int Server::Socket(int protocol_family,int tcp_udp,int protocol){
		int sockfd=socket(protocol_family,tcp_udp|SOCK_NONBLOCK,protocol);
		if(sockfd==-1){
			perror("server socket create fail");
			return 0;
		}
		return sockfd;
	}

	int Server::Bind(int sockfd,const sockaddr* addr,socklen_t addrlen){
		int ret=bind(sockfd,addr,addrlen);
		if(ret==-1){
			perror("server bind fail");
			return 0;
		}
		return ret;
	}

	int Server::Listen(int sockfd,int max_listen){
		int ret=listen(sockfd,max_listen);
		if(ret==-1){
			perror("server listen fail");
			return 0;
		}
		return ret;
	}

	int Server::Accept(int sockfd,struct sockaddr* addr,socklen_t* addrlen){
		int newfd=accept(sockfd,addr,addrlen);
		if(newfd==-1){
			perror("server accept fail");
			return 0;
		}
		return newfd;
	}

	int Server::Close(int sockfd){
		close(sockfd);
		return 0;
	}

	int Server::Shutdown(int sockfd,int how){
		shutdown(sockfd,how);
		return 0;
	}

	class Client
	{
	public:
		Client(int protocol_family,int tcp_udp,int protocol=0);
		~Client();

		int sockfd;

		int Socket(int protocol_family,int tcp_udp,int protocol=0);

		/*
		成功返回0,失败返回-1
		*/
		int Connect(int sockfd,const sockaddr* server_addr,socklen_t addrlen);
		
		int Close(int sockfd);

		int Shutdown(int sockfd,int how);
	};

	Client::Client(int protocol_family,int tcp_udp,int protocol):sockfd(0){
		this->sockfd=Socket(protocol_family,tcp_udp&SOCK_NONBLOCK,protocol);
	}

	Client::~Client(){
		if(this->sockfd!=0){
			Shutdown(this->sockfd,SHUT_RDWR);
		}
	}

	int Client::Socket(int protocol_family,int tcp_udp,int protocol){
		int sockfd=socket(protocol_family,tcp_udp,protocol);
		if(sockfd==-1){
			perror("client socket create fail");
			return 0;
		}
		return sockfd;
	}

	int Client::Connect(int sockfd,const sockaddr* server_addr,socklen_t addrlen){
		int ret=connect(sockfd,server_addr,addrlen);
		if(ret==-1){
			perror("client connect fail");
			return 0;
		}
		return ret;
	}

	int Client::Close(int sockfd){
		close(sockfd);
		return 0;
	}

	int Client::Shutdown(int sockfd,int how){
		shutdown(sockfd,how);
		return 0;
	}

	ssize_t Recv(int sockfd,void* buffer,size_t len,int flags=0){
		int ret=recv(sockfd,buffer,len,flags);
		if(ret==-1){
			perror("recv fail");
			return 0;
		}else if(ret==0){
			perror("recv client close");
			return 0;
		}
		return ret;
	}

	ssize_t Send(int sockfd,const void* buffer,size_t len,int flags=0){
		int ret=send(sockfd,buffer,len,flags);
		if(ret==-1){
			perror("send fail");
			return 0;
		}
		return ret;
	}

}

#endif

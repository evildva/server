#include <iostream>
#include <string.h>
#include <string>
#include <cassert>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>

#include <queue>
#include <pthread.h>

#include "error.hpp"
#include "socket.hpp"


using namespace std;
using namespace SOCKEt;

struct para
{
	int fd;
};

void* work(void* arg)
{
	cout<<"do"<<endl;
	pthread_detach(pthread_self());
}

void* process(void* arg)
{
	pthread_t* th;
	pthread_create(th,NULL,&work,arg);
	cout<<"process"<<endl;
}

int main(int args,char* argv[]){

	if(args<3){
		perror("address port server/client");
		return 1;
	}

	string ip=argv[1],sc=argv[3],se="server",cl="client";
	int port=atoi(argv[2]);

	cout<<argv[3]<<endl;

	if(se.compare(sc)==0){

		struct sockaddr_in addr;
		bzero(&addr,sizeof(addr));
		addr.sin_family=AF_INET;
		inet_pton(AF_INET,ip.c_str(),&addr.sin_addr);
		//addr.sin_port=htons(port);
		addr.sin_port=port;

		Server server(PF_INET,SOCK_STREAM,0);
		assert(server.sockfd>0);
		server.Bind(server.sockfd,(struct sockaddr*)&addr,sizeof(addr));

		server.Listen(server.sockfd);

		struct epoll_event event,*events;
		int epfd=epoll_create(1);
		event.data.fd=server.sockfd;
		event.events=EPOLLIN | EPOLLET;
		epoll_ctl(epfd,EPOLL_CTL_ADD,server.sockfd,&event);
		events = (epoll_event*)calloc(64, sizeof event);

		bool running=true;
		pthread_t* th;
		pthread_create(th,NULL,process,NULL);
		void **ret;
		pthread_join(*th,ret);

		while(running)
		{
			int nums=epoll_wait(epfd,events,64,-1);
			for(int i=0;i<nums;i++)
			{
				int fd=events[i].data.fd;
				if(fd==server.sockfd)
				{
					struct sockaddr_in client;
					socklen_t cl=sizeof(client);
					int connfd=server.Accept(server.sockfd,(struct sockaddr*)&client,&cl);
					if(connfd>0){
						struct epoll_event e;
						e.data.fd=connfd;
						e.events=EPOLLIN | EPOLLET;
						epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&e);
					}
				}
			}
		}

	}

	if(cl.compare(sc)==0){

		struct sockaddr_in server_addr;
		bzero(&server_addr,sizeof(server_addr));
		server_addr.sin_family=AF_INET;
		inet_pton(AF_INET,ip.c_str(),&server_addr.sin_addr);

		Client client(PF_INET,SOCK_STREAM,0);
		assert(client.sockfd>0);
		client.Connect(client.sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
		char buffer[10];
		Recv(client.sockfd,buffer,strlen(buffer),0);
		cout<<buffer<<endl;
		Send(client.sockfd,buffer,strlen(buffer),0);
	}

	return 0;
}
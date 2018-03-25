#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

struct game_server* game_server_create(server_type type, char* self_port, char* master_port, char* master_address){
	struct game_server* server = malloc(sizeof(struct game_server));
	server->type = type;
	if(type == srn_server_main){
		server->master_port = self_port;
		server->master_address = "localhost";
		server->self_port = self_port;
		server->self_address = "localhost";
	}else{
		server->master_port = master_port;
		server->master_address = master_address;
		server->self_port = self_port;
		server->self_address="localhost";
	}
	server->mutex = PTHREAD_MUTEX_INITIALIZER;
	
	array_conf_init(&(server->server_conf));
	arrray_new_conf(&(server->server_conf),&(server->connections));

	server->epoll_fd= epoll_create1(0);
	if(server->epoll_fd < 0){
		//ERROR
	}
	

	return server;
}

void* connect_and_bind_thread(void* a){
	struct game_server* server = (struct game_server*)a;
	
	server->serverfd = socket(AF_INET,SOCK_STREAM,0);
	if(server->serverfd < 0){
		//ERROR
	}
	memset(&server->server_addr,0,sizeof(struct sockaddr_in));
	int portnum = atoi(server->self_address);
	server->server_addr.sin_family=AF_INET;
	server->server_addr.sin_port=htons(portnum);
	server->server_addr.sin_addr.s_addr = INADDR_ANY;
	int result;
	
	result = bind(server->serverfd, (struct sockaddr*) &(server->server_addr), sizeof(struct socaddr_in));
	if(result < 0){
		//ERROR
	}
	
	listen(server->serverfd,5);
	
	struct sockaddr_in client;
	int client_length = sizeof(client);
	struct epoll_event event;
	while(1){
		int new_client = accept(server->serverfd, (struct sockaddr*) &client, &client_length);
		if(new_client < 0){
			//ERROR
		}
		pthread_mutex_lock(&(server->mutex));
		array_add(server->connections,(void*)new_client);//pointers should always be at least as big as the int fd
		event.events = EPOLLIN|EPOLLET|EPOLLHUP;
		event.data.fd = new_client;
		result = epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, new_client, &event);
		if(result < 0){
			//ERROR
		}
		pthread_mutex_unlock(&(server->mutex));
	}
	return (void*)server;
}

int send_to_client(struct game_server* server, int fd, char* msg){
	int ret = write(fd,msg,strlen(msg));
	if(ret < 0){
		//ERROR
	}
	
}



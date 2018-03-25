#ifndef SRN_SERVER_H
#define SRN_SERVER_H

#include "../../Collections-C/src/aray.h"
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

enum server_type{
	srn_server_main,
	srn_server_shard,
};

enum server_client_responses{
	client_disconnect,
};

struct game_server{
	server_type type;

	char* self_port;
	char* self_address;
	char* master_port;
	char* master_address;

	int serverfd;
	struct sockaddr_in server_addr;
	int epoll_fd;
	
	ArrayConf server_conf;
	Array* connections;
	pthread_mutex_t mutex;
};

struct game_server* game_server_create(server_type type, char* self_port, char* master_port, char* master_address);

void* connect_and_bind_thread(void* a);

int send_to_client(struct game_server* server, int fd, char* msg);

#endif

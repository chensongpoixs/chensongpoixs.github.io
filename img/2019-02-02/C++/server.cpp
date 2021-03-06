#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cerrno>


#define BUFFER_SIZE 	1024


int main(int argc, char *argv[])
{
	if (argc <= 2)
	{
		printf("usage: %s ip_address port_number recv_buffer_seize\n", basename(argv[0]));
		return 1;
	}
	int ret = 0;
	const char * ip = argv[1];
	int port = atoi(argv[2]);
	
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);
	
	
	// create socket
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock <= 0)
	{
		close(sock);
		printf("create socket error = %d\n", errno);
		return 0;
	}
	
	int recvbuf = atoi(argv[3]);
	int len = sizeof(recvbuf);
	
	// setting TCP SEDBUFF size read 
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &recvbuf, sizeof(recvbuf));
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &recvbuf, (socklen_t *)&len);
	printf("the tcp recv buffer size after setting is %d\n", recvbuf);
	
	ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
	if (ret == -1)
	{
		close(sock);
		printf("bind socket error = %d\n", errno);
		return 1;
	}
	
	ret = listen(sock, 5);
	if (ret == -1)
	{
		close(sock);
		printf("listen socket error = %d\n", errno);
		return 1;
	}
	
	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(sock, (struct sockaddr*) &client, &client_addrlength);
	if (connfd < 0)
	{
		printf("errno is : %d\n", errno);
	}
	else
	{
		char buffer[BUFFER_SIZE];
		memset(buffer, '\0', BUFFER_SIZE);
		while(recv(connfd, buffer, BUFFER_SIZE - 1, 0) > 0) {}
		close(connfd);
	}
	
	close(sock);
	return 0;
}
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <cerrno>


#define		BUFFER_SIZE		512

int main(int argc, char *argv[])
{
	if (argc <= 2)
	{
		printf("usage: %s ip_address port_number send_bufer_seize\n", basename(argv[0]));
		return 1;
	}
	const char * ip = argv[1];
	
	int port = atoi(argv[2]);
	
	struct sockaddr_in server_address;
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &server_address.sin_addr);
	server_address.sin_port = htons(port);
	// create socket
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock <= 0)
	{
		close(sock);
		printf("create socket error = %d\n", errno);
		return 0;
	}
	
	int sendbuf = atoi(argv[3]);
	int len = sizeof(sendbuf);
	
	// setting TCP SEDBUFF size read 
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof(sendbuf));
	getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t *)&len);
	printf("the tcp send buffer size after setting is %d\n", sendbuf);
	
	if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) != -1)
	{
		char buffer[BUFFER_SIZE];
		memset(buffer, 'a', BUFFER_SIZE);
		send(sock, buffer, BUFFER_SIZE, 0);
		
	}
	close(sock);
	return 0;
}
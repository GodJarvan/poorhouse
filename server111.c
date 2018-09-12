#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>


#define _PORT_ 9999
#define _MAXLOG_ 5


int main()
{

	
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


	struct sockaddr_in server_socket;
	struct sockaddr_in client_socket;

	bzero(&server_socket, sizeof(server_socket));

	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = htonl(INADDR_ANY);
	server_socket.sin_port = htons(_PORT_);

	bind(sock, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in));

	listen(sock, _MAXLOG_);

	printf("wait accept..\n");

	while(1)
	{
	socklen_t len =0;
	int client_sock = accept(sock, (struct sockaddr*)&client_sock, &len);

	char buf_ip[INET_ADDRSTRLEN];
	memset(buf_ip, '\0', sizeof(buf_ip));
	
	inet_ntop(AF_INET, &client_socket.sin_addr, buf_ip, sizeof(buf_ip));
	printf("get connect, ip is : %s port is  %d\n",buf_ip, ntohs(client_socket.sin_port));


        
        
	char othername[10];
	memset(othername, '\0', sizeof(othername));
	char name[10];
	memset(name, '\0', sizeof(name));
	printf("please input your name\n");
	fgets(name, sizeof(name), stdin);
        
        
	send(client_sock, name, sizeof(name), 0);
        recv(client_sock, othername, sizeof(othername), 0);
	othername[strlen(othername)-1]='\0';

		while(1)
		{


			
			
    			char buf[20];
			memset(buf, '\0', sizeof(buf));
			read(client_sock, buf, sizeof(buf));
                        printf("%s:%s\n", othername, buf);
			
			//printf("%s:", name);

			memset(buf, '\0', sizeof(buf));
			fgets(buf, sizeof(buf), stdin);
			buf[strlen(buf) - 1]='\0';
			write(client_sock, buf, strlen(buf)+1);
			printf("please wait...\n");
		}
	}
	close(sock);
	return 0;
	

}

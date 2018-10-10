#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>

#define _PORT_ 9999
#define _BACKLOG_ 10

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_socket;
    struct sockaddr_in client_socket;                                           
    bzero(&server_socket, sizeof(server_socket));
    
    server_socket.sin_family = AF_INET;
    server_socket.sin_addr.s_addr = htonl(INADDR_ANY);
    server_socket.sin_port = htons(_PORT_);

    if(bind(sock, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in)) < 0)
    {
        perror("bind");
        close(sock);
        exit(2);
    }

    if(listen(sock, _BACKLOG_) < 0)
    {
        perror("listen");
        close(sock);
        exit(3);
    }

    printf("欢迎进入陈浩基私人聊天室！\n等待他人连接中.....\n");

    while(1)
    {
        socklen_t len = 0;                                                      
        int client_sock = accept(sock, (struct sockaddr*)&client_socket, &len);
        if(client_sock < 0)
        {
            perror("accept");
            close(sock);
            exit(4);
        }
        char buf_ip[INET_ADDRSTRLEN];
        memset(buf_ip, '\0', sizeof(buf_ip));
        inet_ntop(AF_INET,&client_socket.sin_addr, buf_ip, sizeof(buf_ip));
        printf("成功与对象连接, ip is : %s port is : %d\n",buf_ip, ntohs(client_socket.sin_port));
        char name[20]={};
        char other_name[20]={};
        //printf("请输入你的用户名：");
        //scanf("%s", name);
        send(client_sock,name,strlen(name),0);
        recv(client_sock,other_name,strlen(other_name), 0);
        char buf[20]={};
        memset(buf, '\0', sizeof(buf));
        char buf_send[255]={};
        memset(buf_send, '\0', sizeof(buf_send));
        char buf_recv[255]={};
        memset(buf_recv, '\0', sizeof(buf_recv));
        while(1)
        {
        
        
            char buf[1024];
            memset(buf, '\0',sizeof(buf));
            read(client_sock, buf, sizeof(buf));
            printf("client#:%s\n",buf);
            printf("server#:");

            memset(buf, '\0', sizeof(buf));
            fgets(buf, sizeof(buf), stdin);
            buf[strlen(buf)-1] = '\0';
            write(client_sock, buf, strlen(buf)+1);
            printf("please wait...\n");                                         
        
        
            
            
            
            
            
           // memset(buf,"\0",sizeof(buf));
            
            //输入quit退出
            if(0 == strcmp("quit",buf))
            {
                printf("%s,您已经退出了陈浩基的聊天室\n",name);
                return 0;
            }                                         
        }
    }
    close(sock);
    return 0;
}

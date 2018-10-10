#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>

int fd;
#define _port 9999
typedef struct sockaddr *sockaddrp;
int sockfd;

void *recv_other(void *arg)
{
    char buf[255]= {};
    while(1)
    {
        int ret = recv(sockfd,buf,sizeof(buf),0);
	write(fd,buf,sizeof(buf));
        if(0 > ret)
        {
            perror("recv");
            return;
        }
        printf("%s\n",buf);
    }
}




int main(int argc,char **argv)
{
    
    if(strcmp(argv[3], "1") == 0)
    {
    if(4 != argc)
    {
        perror("参数错误");
        return -1;
    }
    
    fd=open("liaotian",fd);

    //建立socket对象
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(0 > sockfd)
    {
        perror("socket");
        return -1;
    }

    //准备连接地址
    struct sockaddr_in addr = {AF_INET};
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    socklen_t addr_len = sizeof(addr);


    //连接
    int ret = connect(sockfd,(sockaddrp)&addr,addr_len);
    if(0 > ret)
    {
        perror("connect");
        return -1;
    }

    //发送名字
    char buf[255] = {};
    char name[255] = {};
    printf("陈浩基聊天室欢迎您\n请输入您的昵称：");
    
    scanf("%s",name);
    ret = send(sockfd,name,strlen(name),0);
    //printf("您已经进入陈浩基多人聊天室，要私聊请输入<我要私聊>");
    
    if(0 > ret)
    {
        perror("connect");
        return -1;
    }

    //创建接收子线程
    pthread_t tid;
    ret = pthread_create(&tid,NULL,recv_other,NULL);
    
    if(0 > ret)
    {
        perror("pthread_create");
        return -1;
    }
    //循环发送
    while(1)
    {
        //printf("%s:",name);
        scanf("%s",buf);
	write(fd,buf,sizeof(buf));
        int ret = send(sockfd,buf,strlen(buf),0);
        if(0 > ret)
        {
            perror("send");
            return -1;
        }

        //输入quit退出
        if(0 == strcmp("quit",buf))
        {
            printf("%s,您已经退出了陈浩基的聊天室\n",name);
            return 0;
        }

    }
    }
    else
    {/*
        if(4 != argc)
    {
        perror("参数错误");
        return -1;
    }
    
    fd=open("liaotian",fd);
    printf("欢迎来到陈浩基私人聊天房\n");
    printf("请输入你的用户性名：");
    char name[255]={};
    scanf("%s",name);
    printf("请输入你聊天对象的ip地址：");
    char _addr[20]={};
    scanf("%s",_addr);

    //建立socket对象
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(0 > sockfd)
    {
        perror("socket");
        return -1;
    }

    //准备连接地址
    struct sockaddr_in addr = {AF_INET};
    addr.sin_port = htons(atoi(_addr));
    addr.sin_addr.s_addr = inet_addr(_port);

    socklen_t addr_len = sizeof(addr);


    //连接
    int ret = connect(sockfd,(sockaddrp)&addr,addr_len);
    if(0 > ret)
    {
        perror("connect");
        return -1;
    }

    //发送名字
    char buf[255] = {};

    
    ret = send(sockfd,name,strlen(name),0);

    
    //循环发送
    while(1)
    {
        printf("%s:",name);
        scanf("%s",buf);
	write(sockfd,buf,sizeof(buf));
        int ret = send(sockfd,buf,strlen(buf),0);
        if(0 > ret)
        {
            perror("send");
            return -1;
        }
        printf("please wait...\n");
        read(sockfd, buf, sizeof(buf));
        printf("%s: %s\n",name,buf); 
        //输入quit退出
        if(0 == strcmp("quit",buf))
        {
            printf("%s,您已经退出了陈浩基的私聊室\n",name);
            return 0;
        }

    }
        
        
    }
    return 0;*/
    fd=open("liaotian",fd);
    printf("欢迎来到陈浩基私人聊天房\n");
   // printf("请输入你的用户性名：");
    ///char name[255]={};
    //scanf("%s",name);
    printf("请输入你聊天对象的ip地址：");
    char _addr[20]={};
    scanf("%s",_addr);
    char *str = argv[1];
    char buf[20];
    memset(buf, '\0', sizeof(buf));

    struct sockaddr_in server_sock;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_sock, sizeof(server_sock));
    server_sock.sin_family = AF_INET;
    inet_pton(AF_INET, _addr, &server_sock.sin_addr);
    server_sock.sin_port = htons(_port);
    int ret = connect(sock, (struct sockaddr*)&server_sock, sizeof(server_sock));
    if(ret < 0)
    {
        printf("perror");
        return 1;
    }
    printf("成功连接私聊对象.....\n");
    char other_name[20]={};
    //send(sock,name,strlen(name),0);
    //recv(sock, other_name,strlen(other_name),0);
    char buf_send[255]={};
    memset(buf_send, '\0', sizeof(buf_send));
    char buf_recv[255]={};
    memset(buf_recv, '\0', sizeof(buf_recv));
    
    while(1)
    {
        //printf("%s:",name);
            printf("client#:");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf)-1] = '\0';
        write(sock, buf, sizeof(buf));
        if(strncasecmp(buf, "quit", 4) == 0)
        {
            perror("perror");
            break;
        }
        printf("please wait...\n");
        read(sock, buf, sizeof(buf));
        printf("server$: %s\n",buf);
            
        
       // memset(buf,"\0",sizeof(buf));
        

        //输入quit退出
        if(0 == strcmp("quit",buf))
        {
           // printf("%s,您已经退出了陈浩基的聊天室\n",name);
            return 0;
        }
    }
    close(sock);
    return 0;
    }
}


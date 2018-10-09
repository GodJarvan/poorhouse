#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>     
#include<string.h>


#define PORT 1234   
#define MAXDATASIZE 100   

char sendbuf[1024];
char sendbuf_1[1024];
char _sendbuf[1024];
char _sendbuf_1[1024];
char _recvbuf[1024];

char recvbuf[1024];
char name[100];   //自己的名字
char Chat_Name[6]; //聊天人的名字
int fd;
int _fd;

void pthread_recv_group(void* ptr)
{

        while(1)
        {
            if ((recv(fd,recvbuf,MAXDATASIZE,0)) == -1){ 
                printf("recv() error\n"); 
                exit(1); 
            } 

            printf("%s",recvbuf);
            memset(recvbuf,0,sizeof(recvbuf));
        }
}


int Choose_Box()
{
	int num;
	printf("请选择你想使用的聊天气泡的序号:\n");
	printf("< 1 >\n");
	printf("* 2 *\n");
        printf("& 3 &\n");
        printf("# 4 #\n");
        printf("{ 5 }\n");
        printf("@ 6 @\n");
        printf("% 7 %\n");
        printf("8:不使用聊天气泡\n");
        printf("若选择序号错误，则默认不使用气泡。\n");
	scanf("%d", &num);
        return num;
}

void Box(int n, int m)
{
        int i, j, k, h;
    	switch(n)
	{
		case 1:
                    
                    for(i=0; i<m; i++)
                    {
                        sendbuf_1[i]=' ';
                    }
                    sendbuf_1[m]='<';
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                    if (sendbuf_1[strlen(m+sendbuf)] == '\n')
                    {
                        sendbuf_1[strlen(m+sendbuf)] = '\0'; //因为是用的fgets来读取的要发送的信息，但是末尾是自动换行的，所以为了避免格式的错误，应该将末尾的'\n'换成'\0'。
                    }   

                    sendbuf_1[strlen(sendbuf)+m]='>';
                    sendbuf_1[strlen(sendbuf)+1+m]='\n';
                break;
                case 2:
                    
                    for(j=0; j<m; j++)
                    {
                        sendbuf_1[j]=' ';
                    }
                    sendbuf_1[m]='*';
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                    if (sendbuf_1[strlen(m+sendbuf)] == '\n')
                    {
                        sendbuf_1[strlen(m+sendbuf)] = '\0';
                    }   

                    sendbuf_1[strlen(sendbuf)+m]='*';
                    sendbuf_1[strlen(sendbuf)+1+m]='\n';
                break;
                case 3:
                    
                    for(k=0; k<m; k++)
                    {
                        sendbuf_1[k]=' ';
                    }
                    sendbuf_1[m]='&';
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                    if (sendbuf_1[strlen(m+sendbuf)] == '\n')
                    {
                        sendbuf_1[strlen(m+sendbuf)] = '\0';
                    }   

                    sendbuf_1[strlen(sendbuf)+m]='&';
                    sendbuf_1[strlen(sendbuf)+1+m]='\n';
                break;
            
                case 4:
                    
                    for(k=0; k<m; k++)
                    {
                        sendbuf_1[k]=' ';
                    }
                    sendbuf_1[m]='#';
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                    if (sendbuf_1[strlen(m+sendbuf)] == '\n')
                    {
                        sendbuf_1[strlen(m+sendbuf)] = '\0';
                    }   

                    sendbuf_1[strlen(sendbuf)+m]='#';
                    sendbuf_1[strlen(sendbuf)+1+m]='\n';
                break;
             
                case 5:
                    
                    for(k=0; k<m; k++)
                    {
                        sendbuf_1[k]=' ';
                    }
                    sendbuf_1[m]='{';
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                    if (sendbuf_1[strlen(m+sendbuf)] == '\n')
                    {
                        sendbuf_1[strlen(m+sendbuf)] = '\0';
                    }   

                    sendbuf_1[strlen(sendbuf)+m]='}';
                    sendbuf_1[strlen(sendbuf)+1+m]='\n';
                break;
                case 6:
                    
                    for(k=0; k<m; k++)
                    {
                        sendbuf_1[k]=' ';
                    }
                    sendbuf_1[m]='@';
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                    if (sendbuf_1[strlen(m+sendbuf)] == '\n')
                    {
                        sendbuf_1[strlen(m+sendbuf)] = '\0';
                    }   

                    sendbuf_1[strlen(sendbuf)+m]='@';
                    sendbuf_1[strlen(sendbuf)+1+m]='\n';
                break;
                case 7:
                    
                    for(k=0; k<m; k++)
                    {
                        sendbuf_1[k]=' ';
                    }
                    sendbuf_1[m]='%';
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                    if (sendbuf_1[strlen(m+sendbuf)] == '\n')
                    {
                        sendbuf_1[strlen(m+sendbuf)] = '\0';
                    }   

                    sendbuf_1[strlen(sendbuf)+m]='%';
                    sendbuf_1[strlen(sendbuf)+1+m]='\n';
                break;
                case 8:
                    for(h=0; h<m; h++)
                    {
                        sendbuf_1[h]=' ';
                    }
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                break;
                default:
                    for(h=0; h<m; h++)
                    {
                        sendbuf_1[h]=' ';
                    }
                    strncat(sendbuf_1,sendbuf,strlen(sendbuf));
                    
                break;
        }
  
}

int Message(char friend[])
{
        int n;
        printf("%s wanna chat with you. If you accept please press 1, refuse please  press 0.\n", friend);
        scanf("%d", &n);
        return n;
}

int main(int argc, char *argv[]) 
{
	int  numbytes;   
	char buf[MAXDATASIZE];   
    	struct hostent *he;       
    	struct sockaddr_in server;  


    	if (argc !=2) {         
		printf("Usage: %s <IP Address>\n",argv[0]); 
    		exit(1); 
    	} 


    	if ((he=gethostbyname(argv[1]))==NULL){  
    		printf("gethostbyname() error\n"); 
    		exit(1); 
    	} 

    	if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){ 
    		printf("socket() error\n"); 
    		exit(1); 
    	} 

    	bzero(&server,sizeof(server));

    	server.sin_family = AF_INET; 
    	server.sin_port = htons(PORT); 
    	server.sin_addr = *((struct in_addr *)he->h_addr);  
    	if(connect(fd, (struct sockaddr *)&server,sizeof(struct sockaddr))==-1){  
    		printf("connect() error\n"); 
    		exit(1); 
    	} 

    	printf("connect success\n");
        
        recv(fd,recvbuf,MAXDATASIZE,0);
        printf("%s\n", recvbuf);
        memset(recvbuf,0,sizeof(recvbuf));
        
    	char str[]=" 已进入聊天室\n";
    	printf("请输入用户名（最多六个字母）：");
    	fgets(name,sizeof(name),stdin);
      
        printf("提醒：退出聊天时请输入“quit若失败请多次尝试输入\n");
        printf("\n");
        
        char answer[1];
        
        printf("选择群聊请按1, 私聊请按2\n");
        
        scanf("%s", &answer);
        send(fd, answer, 1, 0); 
        
        if(strcmp(answer, "2")==0)
        {
            memset(Chat_Name, 0, sizeof(Chat_Name));
    
            printf("请输入你想要聊天的人的名字并在后面加‘-’(如：张三- 和 李四-): \n");
            scanf("%s", &Chat_Name);
            send(fd, Chat_Name, 6, 0);
        }
        
        
        send(fd,name,(strlen(name)-1),0);
        
        send(fd,name,(strlen(name)-1),0);
        send(fd,str,(strlen(str)),0);
        
        
            pthread_t tid;
            pthread_create(&tid,NULL,(void*)pthread_recv_group,&fd);
    
            int box=8;
//          box=Choose_Box();

            while(1)
            {         
                    memset(sendbuf,0,sizeof(sendbuf));
                    memset(sendbuf_1,0,sizeof(sendbuf_1));
                    fgets(sendbuf,sizeof(sendbuf),stdin);
                
                    Box(box, strlen(name)-1);

                    if(strcmp(sendbuf,"exit\n")==0){
                            memset(sendbuf,0,sizeof(sendbuf));
                            printf("您已退出群聊\n");
                            send(fd,sendbuf,(strlen(sendbuf)),0);
                            break;
                    }
                    if(strlen(sendbuf)>1)   //防止用户在聊天时没有打字也按空格就发出去的情况
                    {
                        send(fd,name,(strlen(name)-1),0);
                        send(fd,":\n",2,0);
                        send(fd,sendbuf_1,(strlen(sendbuf_1)),0);  //sendbuf_1是添加过气泡的消息
                    }

            }
            close(fd);
}

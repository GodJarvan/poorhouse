#include <stdio.h>          
#include <unistd.h>          
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include<pthread.h> 
#include<stdlib.h>
#include <string.h>
#include <ctype.h>

#define PORT 1234    
#define BACKLOG 1 
#define Max 10
#define MAXSIZE 2048

//连接后记录confd数组
char information[100];  //提示有人进入聊天室
int confd[50] = {};
char Nameset[200];
char nameset[200]={};
//设置连接人数
int count = 0;
//每个人的信息结构
typedef struct user{
  char name [100];
  int flag;       //是否在线 
  int object;     //正在私聊的对象的fd 没有的话为-1
  int status;    //是否忙碌 没有私聊的话为0, 忙碌的话为1
  int fans;       //想要跟自己聊天的人， 没有时为0, 可以累加
  int number;
}user;

//结构体数组
user info [50]={}; 


/*定义全局变量*/
int fdt[Max]={0};
char mes[1024];
char _mes[1024];
int number=0;

char buf_snd[50];
char _buf_snd[50];


int SendToClient(int fd,char* buf,int Size, int index)
{
    int i;
    int k;
    memset(Nameset,0,200);
    
    for(k=0;k<=count;k++){
        if(info[k].flag==0){
         
            continue;
        }
        strcat(Nameset,info[k].name);
    }
    
    
    memset(nameset,0,200);
    
    sprintf(nameset,"(当前在线的有%s)\n",Nameset);
    
    
    for(i=0;i<Max;i++)
    {
        //printf("fdt[%d]=%d\n",i,fdt[i]);   
        
        if((fdt[i]!=0)&&(fdt[i]!=fd))
        {
        
            send(fdt[i],buf,Size,0); 
            //printf("send message to Name:%s fdth:%d\n",info[i].name, fdt[i]);
            
        }
    }
    return 0;
}

int Search_Name(char a[8])
{
        int i;
        for(i=0; i<50; i++)
        {
            printf("info[i].name : %s\n", info[i]);
            if(strcmp(a, info[i].name)==0)
            {
                return i;
            }
        }
        return -1;
}

void *pthread_service(void* sfd)
{    
    
    int fd=*(int *)sfd;  
    int index;
    
    send(fd,nameset,sizeof(nameset),0);
    
    
    int n;
    for(n=0; n<50; n++)
    {
        if(confd[n]==fd)
        {
            index=n;
            break;
        }
    }
    
    info[index].number=index;
    info[index].flag=1;
    char Name[20];
    
    char b[1];
    recv(confd[index], b, 1, 0);
    
    
    if(strcmp(b, "1")==0)    //选择群聊
    {
    
        recv(confd[index],Name,6,0);
        strcpy(info[index].name,Name);
        char* jiange="-";
        strcat(info[index].name,jiange);

    
        int k=0;
        int flag;
    
        while(1)
        {
            int numbytes;
            int i;
            numbytes=recv(fd,mes,MAXSIZE,0);

/*        if(k==0)
        {
                int m=0;
                int n=0;
                while((mes[m]&&mes[m]!=' ') && isascii(mes[m]))
                {
                    m++;
                }
                flag=m;
                strncpy(Name, mes, m);
                Name[m]='\0';

                        
                strncpy(info[index].name , Name, m);
                char* jiange="--";
                strcat(info[index].name,jiange);
                info[index].name[m+1]='\0';
        }

        
*/        
            char a[4];

            memset(buf_snd,0,50);
        
        
            if((0 == strcmp("quit",strncpy(a, mes+flag+2, 4)))||(0 == strcmp("quit",strncpy(a, mes, 4)))||(0 == strcmp("quit",strncpy(a, mes+flag+3, 4))) || (0 == strcmp("quit",strncpy(a+1, mes, 4))))  //因为"mes"有时候会带上名字，有时候没有带名字
            {
  
                sprintf(buf_snd,"%s已经退出聊天室\n",info[index].name);
                int i;
                for(i = 0;i <= count;i++)
                {
                    if(i == index || 0 == confd[i])
                    {
                        continue;
                    }
                    send(confd[i],buf_snd,strlen(buf_snd),0);
                }
                confd[index] = -1;
                info[index].flag=0;
                info[index].number=1024;
                close(fd);
                break;
            }
        
        
            if(numbytes<=0){
                for(i=0;i<Max;i++){
                    if(fd==fdt[i]){
                        fdt[i]=0;               
                    }
                }
                break;
            }

            SendToClient(fd,mes,numbytes, index);
            k++;
        }
        close(fd);
        
    }
    
    else if(strcmp(b, "2")==0)  //选择私聊
    {
      
            char* jiange="-";
            char chat_name[8];
            recv(confd[index],chat_name,6,0);
            strcat(chat_name,jiange);
//            printf("聊天对象: %s\n", chat_name);
            
            recv(confd[index],Name,6,0);
//            printf("自己的名字：%s\n", Name);
            
            strcpy(info[index].name,Name);
            strcat(info[index].name,jiange);
            
            strcat(Nameset, Name);
            
            
            
            int c=Search_Name(chat_name);
            if(c==-1) printf("对不起，没有这个人。\n");
            else printf("正在请求与其对话\n");
            
            send(confd[c], Name, strlen(Name), 0);
            send(confd[c], "请求与你私聊， 同意请选择1, 拒绝请选择2 (ATTENTION)\n",100, 0);
            //发送给聊天对象
/*          char answer[1];
            recv(confd[c],answer,1,0);
            printf("Answer: %s\n", answer);
 */           
            memset(Nameset,0,200);
    }
        
}
            
            
int  main()  
{ 
    int listenfd, connectfd;    
    struct sockaddr_in server; 
    struct sockaddr_in client[50];      
    int sin_size; 
    sin_size=sizeof(client[0]); 
    int fd;


    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {   
        perror("Creating socket failed.");
        exit(1);
    }


    int opt = SO_REUSEADDR;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&server,sizeof(server));  


    server.sin_family=AF_INET; 
    server.sin_port=htons(PORT); 
    server.sin_addr.s_addr = htonl (INADDR_ANY); 


    if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) { 
    perror("Bind error.");
    exit(1); 
    }   


    if(listen(listenfd,BACKLOG) == -1){  
    perror("listen() error\n"); 
    exit(1); 
    } 
    printf("Waiting for client....\n");

    int k;
    for(k=0;k<50;k++)
    {
      info[k].flag=0;
      info[k].status=0;
      info[k].object=-1;
      info[k].fans=0;
      memset(info[k].name,0,20);
      info[k].number=1024;
    }
    
    while(1)
    {
        
        if ((fd = accept(listenfd,(struct sockaddr*)&client[count],&sin_size))==-1) {
            perror("accept() error\n"); 
            exit(1); 
        }
        
        
        if(number>=Max){
            printf("no more client is allowed\n");
            close(fd);
        }

        
        int i;
        for(i=0;i<Max;i++){
            if(fdt[i]==0){
                fdt[i]=fd;
                break;
            }

        }
        
        confd[count]=fd;
        ++count;
    
        
        pthread_t tid;
        
        pthread_create(&tid,NULL,(void*)pthread_service,&fd);
        
        
        number=number+1;
        
    }

    close(listenfd);            
}

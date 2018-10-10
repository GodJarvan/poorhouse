#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct sockaddr *sockaddrp;

//存储客户端地址的结构体数组
struct sockaddr_in src_addr[50];
socklen_t src_len = sizeof(src_addr[0]);

int quin =0;

//连接后记录confd数组
int confd[50] = {};


//设置连接人数
int count = 0;
//每个人的信息结构
typedef struct user{
  char name [20];
  int flag;
  int number;
  int FLAG;
  int friend;
}user;

//结构体数组
user info [50]={}; 

void *broadcast(void *indexp)
{
    int index = *(int *)indexp;
    info[index].number=index;
    info[index].flag=1;
    char buf_rcv[255] = {};
    char buf_snd[255] = {};
    char fname[20];//私聊时才用，不然没用
    memset(fname,0,20);
    //第一次读取用户姓名
    char name[20] = {};
    int ret = recv(confd[index],name,sizeof(name),0);
    if(0 > ret)    
      {
        perror("recv");
        close(confd[index]);
        return;
      }
    //把name存到info中
    strcpy(info[index].name,name);
    char* jiange="-";
    strcat(info[index].name,jiange);
    
    while(1)
      {
	
        bzero(buf_rcv,sizeof(buf_rcv));
	bzero(buf_snd,255);
	recv(confd[index],buf_rcv,sizeof(buf_rcv),0);
	

	 //判断是否退出
        if(0 == strcmp("quit",buf_rcv))
        {
            sprintf(buf_snd,"%s已经退出悟空聊天室",name);
	    int i;
            for(i = 0;i <= count;i++)
            {
                if(i == index || 0 == confd[i])
                {
                    continue;
                }
		
                send(confd[i],buf_snd,strlen(buf_snd),0);
		  bzero(buf_snd,255);

            }
            confd[index] = -1;
	    info[index].flag=0;
	    info[index].number=1024;
	
            pthread_exit(0);
                  
        }
	//将所有名字集合在一起。作为成员列表名单指令
	int k;
	char Nameset [200];
	memset(Nameset,0,200);
	for(k=0;k<=count;k++){
	  if(info[k].flag==0)
	    {
	      continue;}
	  strcat(Nameset,info[k].name);

	  }       
	if(0 == strcmp("list",buf_rcv))
	  {
	    sprintf(buf_snd,"当前在线的有：%s",Nameset);  
	    send(confd[index],buf_snd,strlen(buf_snd),0);
		  bzero(buf_snd,255);

	  }

	  /*if(info[index].FLAG == 0){
              if(0 == strcmp("2",buf_rcv)){
                  info[index].FLAG = 2;
                  sprintf(buf_snd,"你要和谁私聊？");
                  send(confd[index], buf_snd, strlen(buf_snd), 0);
			  bzero(buf_snd,255);                  
            }
              else{
                  int i;
	  for(i = 0;i <= count;i++)
	    {
            if(info[i].flag==0||info[i].number==index)
            {
                continue;
            }
	   
	    else{  
		send(confd[i],buf_snd,strlen(buf_snd),0);
		  bzero(buf_snd,255);
		}
	
	  }
		quin++;
            }
              
        }
          else if(info[index].FLAG == 1){
              if(0 == strcmp("1",buf_rcv)){
                  info[index].FLAG = 1;
                  char send_[20]={};
                  sprintf(send_,info[index].name,"已经退出私聊" );
		  int i;
                  for(i=0;i<=count;i++) send(confd[index], send_, strlen(buf_snd), 0);
                  
                  
                  info[index].FLAG=0;
                  info[index].friend=-1;
            }              
              else{
                   
                  send(confd[info[index].friend], buf_rcv,strlen(buf_rcv),0);
         	  bzero(buf_snd,255);
         
            }
              
        }
          else if(info[index].FLAG == 2){
              int q=0;
              int j;char mizi[20]={};
              strcmp(mizi,buf_rcv);
              for(j=0; j<=count;j++){
                  if(strcmp(info[j].name, mizi)==0){
                      q=1;
                      break;
                   }
              }
              if(q==1){
                  info[index].FLAG=1;
                  info[index].friend=j;
                  info[j].FLAG=1;
                  info[j].friend=index;
		sleep(5);
		printf("%s,%d",info[j].name,info[j].FLAG);
                  
            }
              else {
		send(confd[index],"noname", strlen("noname"), 0);
		
		      info[index].FLAG=0;
		}
		printf("%s,%d",info[j].name,info[j].FLAG);

        }    */
	  
	  
	  
	  
	/*//私聊指令
	  if(0==strcmp("我要私聊",buf_rcv)){
	  info[index].FLAG=1;
	  sprintf(buf_snd,"请输入你要私聊的对象");
	  send(confd[index],buf_snd,strlen(buf_snd),0);
	  memset(fname,0,20);
	  recv(confd[index],fname,sizeof(fname),0);
	  int i;
	  for(i=0;i<count;i++){
	    if(0==strcmp(fname,info[i].name))
	      {
		info[i].FLAG=1;
		info[i].friend=index;
		info[index].friend=i;
		  }
		}
	  sprintf(buf_snd,"%s私聊中",name);
	  for(i = 0;i <= count;i++)
	    {
            if(info[i].flag==0||info[i].number==index)
            {
                continue;
            }
	    else  send(confd[i],buf_snd,strlen(buf_snd),0);

	    }
	}
	*/
	//群聊指令
	if(0==strcmp("我要群聊",buf_rcv)){
	  info[index].FLAG=0;
	  sprintf(buf_snd,"对方已退出私聊");
	  send(confd[info[index].friend],buf_snd,strlen(buf_snd),0);
	  info[info[index].friend].FLAG=0;
	  sprintf(buf_snd,"%s加入群聊",name);
	  int i;
	  for(i = 0;i <= count;i++)
	    {
            if(info[i].flag==0||info[i].number==index)
            {
                continue;
            }
	   
	    else  send(confd[i],buf_snd,strlen(buf_snd),0);
	
	  }
	}
        





	sprintf(buf_snd,"%s:%s",name,buf_rcv);
	if(info[index].FLAG==0){
	int i;
	for(i = 0;i <= count;i++)
        {
            if(info[i].flag==0||info[i].number==index||info[i].FLAG==1)
            {
                continue;
            }
	   
	    else  send(confd[i],buf_snd,sizeof(buf_snd),0);
           
	}
	}

	else if(info[index].FLAG==1){
	int i;
	for(i = 0;i <= count;i++)
        {
	  if(info[i].number==info[index].friend)//只送给对方
	      send(confd[info[i].number],buf_snd,sizeof(buf_snd),0);
	}
	}

	
      }//while结束

}//广播结束






int main(int argc,char **argv)
{
    printf("陈浩基的聊天室欢迎您\n");
    
   

    //创建通信对象
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(0 > sockfd)
    {
        perror("socket");
        return -1;
    }

    //准备地址
    struct sockaddr_in addr = {AF_INET};
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = inet_addr(argv[2]);

    socklen_t addr_len = sizeof(addr);



    //绑定
    int ret = bind(sockfd,(sockaddrp)&addr,addr_len);
    if(0 > ret)
    {
        perror("bind");
        return -1;
    }


    //设置最大排队数
    listen(sockfd,50);

    int index = 0;

    //置空用户信息(1024表示没人用)
    int k;
    for(k=0;k<50;k++){
      info[k].flag=0;
      memset(info[k].name,0,20);
      info[k].number=1024;//未使用
      info[k].friend=1024;//没私聊对象
      info[k].FLAG=0;//在群聊
    }

    while(count <= 50)
    {
        confd[count] = accept(sockfd,(sockaddrp)&src_addr[count],&src_len);
        ++count;
        //保存此次客户端地址所在下标方便后续传入
        index = count-1;

        pthread_t tid;
        int ret = pthread_create(&tid,NULL,broadcast,&index);
        if(0 > ret)
        {
            perror("pthread_create");
            return -1;
        }


    }


}


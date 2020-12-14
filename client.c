#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define MAX_NAME    30
#define MAX_MSG    1024

typedef struct _client{	
    int disc;
    int unum;
    int num;
    char client_name[MAX_NAME];
}Client;

int service;
int num;
int flag = 0;
pthread_mutex_t usermutex;
char fname[100];

void CSockExit(int signo);
int Sockets(char *ip, int port);
void *SendMsg(void *client);
void *RecvMsg(void *client);
void StartTalk(int dsc);

void StartTalk(int dsc)	// 입장
{
    Client client;
    pthread_t ptr[2];
    char name[MAX_NAME]="";	 
    char rbuf[MAX_MSG]="";	

    recv(dsc, rbuf, 100, 0);	
    fputs(rbuf,stdout);		 
    fgets(name, sizeof(name), stdin);	 
    send(dsc, name, strlen(name), 0);	 
    client.disc = dsc;
    strcmp(client.client_name,name);

	// 쓰레드 생성    
    pthread_create(&ptr[0], NULL, RecvMsg, &client);	 
    pthread_detach(ptr[0]);
    pthread_create(&ptr[1], NULL, SendMsg, &client);	 
    pthread_detach(ptr[1]);

    while(1)
        pause();
}

void *RecvMsg(void *client)	// 메시지 수신
{
    Client cs = *(Client *)client;
    char rbuf[MAX_MSG];	
    int fd;	
    int cnt;	

    memset(rbuf, 0, sizeof(rbuf));	 

    while(1)
    {
        if(flag == 0)	
        {
            recv(cs.disc, rbuf, sizeof(rbuf), 0);
            memset(rbuf, 0, sizeof(rbuf));	 
        }

        while(recv(cs.disc, rbuf, sizeof(rbuf), 0) > 0)
        {
            if(flag == 1)	
            {
                fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644);	 
                cnt = write(fd, rbuf, strlen(rbuf));
                close(fd);
                flag = 0;
       			memset(rbuf, 0, sizeof(rbuf));	
            }
            else
            {
                fputs(rbuf, stdout);	
                memset(rbuf, 0, sizeof(rbuf));	 
            }
        }
    }
}

void *SendMsg(void *client)	// 메시지 전송
{
    Client cs = *(Client *)client;
    char sbuf[MAX_MSG];	
    char *fio;	

    while(1)
    {
        fgets(sbuf, sizeof(sbuf), stdin);	
        send(cs.disc, sbuf, sizeof(sbuf), 0);	
        if(!strncmp(sbuf, "/f", 2)){	
            flag = 1;	 
            fio = strtok(sbuf, " ");	
            fio = strtok(NULL, " ");	 
            fio[strlen(fio)-1] = '\0';	 
            memset(fname, 0, sizeof(fname));	 
            strcpy(fname, fio);	
        }
        memset(sbuf, 0, sizeof(sbuf));	 
    }
}

int Sockets(char *ip, int port)	// 서버 연결
{
    int dsc;
    
    if((dsc = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {	
        perror("sock error");
        return -1;
    }
    
    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;	
    servaddr.sin_addr.s_addr = inet_addr(ip);	
    servaddr.sin_port = htons(port);	

    if(connect(dsc, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {	
        perror("connect");
        return -1;
    }

    return dsc;
}

void CSockExit(int signo)	
{
    printf("클라이언트를 종료합니다.\n");
    close(service);
    exit(0);
}


int main(int argc,char *argv[])
{
    int port;
    char pt[MAX_MSG];

    printf("포트 번호를 입력하세요: ");
    scanf("%d", &port);
    getchar();
    signal(SIGINT, CSockExit);
    
    pthread_mutex_init(&usermutex, NULL);
    
    if((service = Sockets(IP, port)) == -1)
    {	
        perror("socket");
        return 0;
    }

    StartTalk(service);	

    return 0;
}

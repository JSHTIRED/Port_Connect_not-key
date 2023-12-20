#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

int main(int argc, char *argv[])
{
	int sock;
	int str_len;
	struct sockaddr_in serv_adr;
	int t=6000;
	int i=0;
	int serv_sock, clnt_sock;
	struct clnt_adr;
	char* a="10.10.141.52";
	pid_t pid;
//	socklen_t adr_sz;
	int state;
	int message=1;
	char BUF[message];
	int total=10;
	int b;

	int man=1;
	int woman=1;
	int ms,ws;
	while(1)
	{
		
	    	sock=socket(PF_INET, SOCK_STREAM, 0);
	//		printf("%d	q\n",sock);
	    	memset(&serv_adr, 0, sizeof(serv_adr));
//			printf("%ld	Q\n",sizeof(serv_adr));
	    	serv_adr.sin_family=AF_INET;
	    	serv_adr.sin_addr.s_addr=inet_addr(a);
	    	serv_adr.sin_port=htons(t);
		while(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))<0)
			sleep(5);
			
		}
}



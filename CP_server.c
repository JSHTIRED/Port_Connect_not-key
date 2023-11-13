#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/stat.h>
#define message 1
#define cp 50
#define conn 15
#define com 2 
//conn은 연결하는 client수 *5
void read_childproc(int sig); // (line 206~211)fork 문 만드는 함수
void read_routine(int clnt_sock, char BUF[message]);//(line 212~235)받은 정보를 분류하는 함수
void closed(int clnt_sock,int serv_sock); //(line 236~) 서버와 클라이언트를 종료하는 함수
int serv_sock, clnt_sock; // 서버,클라 소켓
struct sockaddr_in serv_adr, clnt_adr; // 서버와 클라 주소
pid_t pid; //프로세스 번호 저장타입
struct sigaction act; //확장성 및 조절을 위한 시그널 변수
socklen_t adr_sz; //소켓의 길이 변수
int str_len; // 불러오는 buf의 길이 변수 
int n=0; //전체 횟수에 대한 카운팅 변수
int T=3; // 연결하는 클라이언트의 수 
int n1=0,n2=0,n3=0; // T의 값에 따른 카운팅 변수 
char BUF[message]; // 클라이언트에서 받은 buf
char buf[cp],buf1[cp],buf2[cp],buf3[cp];//임시로 값을 저장하는 변수,T의 값에 따라서 각각을 저장하는 변수들
int t1=0,t3,p=1; // 각 port값(name)을 저장하는 변수 
char* arry; //출력(print)을 위한 암호코드
time_t s,e; //child가 재대로 작동하는 지를 확인하는 변수

//char *a="127.0.0.1"; // server의 ip주소
pthread_t t_id[com] = {0}; // fork문 산하에 다량의 업무를 지시하는 경우 사용
char msg[10];
int i=0,check=0;
int t=6000;
int main(int argc, char *argv[])
{
	int t2=6002;
	srand(time(NULL));
	while(1)
	{
		for(;;t++)//id와 passward 없이도 가능한 연결 방법
		{
			printf("%d case 1\n",t); //작동확인
			serv_sock=socket(PF_INET, SOCK_STREAM, 0); //소켓 제작
			memset(&serv_adr, 0, sizeof(serv_adr)); // 소켓 초기화
			serv_adr.sin_family=AF_INET; //	IPv4인터넷 프로토콜
			serv_adr.sin_addr.s_addr=inet_addr("10.10.141.52"); //주소 여기서 "127.0.0.1"대신 char* a를 대입할 수 있음 
			serv_adr.sin_port=htons(t);//여기에 t값을 대입해도 가능
			printf("socket: 10.10.141.52 %d\n",t);
			bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)); // 소켓을 bind
			listen(serv_sock,2); // 예비로 2개의 연결을 허용
			adr_sz=sizeof(clnt_adr); //클라이언트 주소 확인
			clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz); //클라이언트와의 접속을 시도 무한대기에 잡히면 부모 쓰레드 가동
			str_len=read(clnt_sock,BUF,message); //접속이 되면 클라이언트 소켓에 있는 BUF를 받아옴
			read_routine(clnt_sock,BUF); // 받은 BUF를 통해서 적당한 값을 임시의 변수인 buf에 대입
			strcpy(buf1,buf); //buf1(t=6000에 배당된 buf변수)에 임시변수 buf의 값을 복사함
			if(check==0)//o개의 통신이 된다면 (최초 통신 후 3번째 통신부터 port변경)
			{
				t1=0;
				buf2==0;
				t1=(rand()&65535);// t1=t도 가능 다만 다중으로 작동시 오류가 날 수도 있기에 직접 대입
				while(t1<10000)
				{
					t1=(rand()&65535);
				}
				sprintf(buf2,"%d",t1);
				check=1;
			}	
			if(buf2!=NULL)
			{
				str_len=write(clnt_sock,buf2,strlen(buf2));
			}
			printf("%d: %s\n",t1,buf1); //case 0 작동확인 코드
			printf("%s\n",buf2);
			if(t>=t2)
			{
				t2=t1+com;
				if(t1!=0)
				{
					t=t1-1;
					printf("%d\n",t);
					check=0;
				}
				else
				{
					t=t2;
					printf("%d\n",t);
					check=0;
				}
			}	
		
			buf==0; // 임시의 변수인 buf를 초기화하라! if초기화를 안할 경우 값이 이상해질 수 있음
			BUF==NULL; // 클라이언트에서 받아온 BUF도 초기화해라
			closed(clnt_sock,serv_sock); //모든 소켓을 닫아라
			printf("close\n");
		}
	}
	
}
void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
}
void read_routine(int clnt_sock,char BUF[message]) //분류표현
{
  if (strcmp(BUF,"I")==0)		//받은 값이 I라면
	  (strcpy(buf,"남 원활 여 원활")); //다음을 저장하라!
  else if (strcmp(BUF,"J")==0)
	  (strcpy(buf,"남 원활 여 혼잡"));
  else if (strcmp(BUF,"L")==0)
	  (strcpy(buf,"남 원활 여 포화"));
  else if (strcmp(BUF,"Q")==0)
	  (strcpy(buf,"남 혼잡 여 원활"));
  else if (strcmp(BUF,"R")==0)
	  (strcpy(buf,"남 혼잡 여 혼잡"));
  else if (strcmp(BUF,"T")==0)
	  (strcpy(buf,"남 혼잡 여 포화"));
  else if (strcmp(BUF,"a")==0)
	  (strcpy(buf,"남 포화 여 원활"));
  else if (strcmp(BUF,"b")==0)
	  (strcpy(buf,"남 포화 여 혼잡"));
  else if (strcmp(BUF,"d")==0)
	  (strcpy(buf,"남 포화 여 포화"));
  else
	 printf("error"); //만약 어느 값도 받지 못했다면 프린트하라

}
void closed(int clnt_sock,int serv_sock)//sock폐쇠 함수
{
	close(clnt_sock);
	close(serv_sock);

}


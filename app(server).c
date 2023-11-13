/*
 * app.c
 *
 *  Created on: Oct 3, 2023
 *      Author: iot22
 */
#include "app.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "stm32f4xx_hal.h"
#include <stdint.h>

#define cp 50
#define com 2
#define message 2
int serv_sock, clnt_sock;
struct sockaddr_in serv_adr, clnt_adr;
int c = sizeof(struct sockaddr_in);
socklen_t adr_sz;

uint16_t t=6000;
uint32_t t1=0; //만약 전달하는 값을 어마무시한 값으로 하고 싶다면 uint16_t가 아닌 int형으로 선언할 것 단 이 경우 음수 값에 주의
//여기서는 반드시 uint32_t를 사용할 것
char BUF[message]={0};
char msg[10]={0,};
int i=0,check=0;
int read_size;
char buf[cp]={0,},buf1[cp]={0,},buf2[cp]={0,};
int T1=0;
int str_len;
uint16_t t2=6002;
uint16_t t3=0;


// uint8_t t1;
//,uint16 t1,uint32 t1,uint64 t1
RNG_HandleTypeDef RngHandle; // stm32의 하드웨어 RNG(랜덤 넘버 제너레이터) 사용할 핸등

void read_routine(int clnt_sock,char BUF[message]);
void closed(int clnt_sock,int serv_sock);

void app(void)
{
	HAL_Init();
	__HAL_RCC_RNG_CLK_ENABLE();
	RngHandle.Instance = RNG;
	HAL_RNG_Init(&RngHandle);
	if(t==6003)
	{
		t=T1;
	}
	serv_sock = socket(AF_INET , SOCK_STREAM , 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = INADDR_ANY;
	serv_adr.sin_port = htons(t);
	printf("socket: 10.10.141.52 %d\n",t);
	if(bind(serv_sock,(struct sockaddr *)&serv_adr , sizeof(serv_adr)) < 0)
	{
		perror("bind failed. Error");
	}
	listen(serv_sock , 3);
	printf("adr_sz\n");
	if((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr,&adr_sz))<0)
		perror("accept");
	printf("accept\n");
	str_len=read(clnt_sock,BUF,message);
	read_routine(clnt_sock,BUF);
	printf("read\n");
	strcpy(buf1,buf);
	if(check==0)
	{
		t1=0;
		printf("check0\n");
		t1= HAL_RNG_GetRandomNumber(&RngHandle);
		while(1)
		{
			if(t1<641935334)
			{
				if(t1>100000000)
				{
					break;
				}
			}
			t1= HAL_RNG_GetRandomNumber(&RngHandle);
		}
		sprintf(buf2,"%d",t1);
		check=1;
	}
	if(buf2!=NULL)
	{
		str_len=write(clnt_sock,buf2,strlen(buf2));
	}
	printf("t1(%d): buf1(%s)\n",t1,buf1); //case 0 작동확인 코드
	printf("buf2: %s\n",buf2);
	if(t>=t2)
	{
		t2=t1+com;
		if(t1!=0)
		{
			t=t1-1;
			printf("t: %d\n",t);
			check=0;
		}
		else
		{
			t=t2;
			printf("t: %d\n",t);
			check=0;
		}
	}

	closed(clnt_sock,serv_sock); //모든 소켓을 닫아라
	printf("close\n");
	t++;
}

void read_routine(int clnt_sock,char BUF[message]) //분류표현
{
  if (strcmp(BUF,"I")==0)		//받은 값이 I라면
	  (strcpy(buf,"AA")); //다음을 저장하라!
  else if (strcmp(BUF,"J")==0)
	  (strcpy(buf,"AB"));
  else if (strcmp(BUF,"L")==0)
	  (strcpy(buf,"AC"));
  else if (strcmp(BUF,"Q")==0)
	  (strcpy(buf,"BA"));
  else if (strcmp(BUF,"R")==0)
	  (strcpy(buf,"BB"));
  else if (strcmp(BUF,"T")==0)
	  (strcpy(buf,"BC"));
  else if (strcmp(BUF,"a")==0)
	  (strcpy(buf,"CA"));
  else if (strcmp(BUF,"b")==0)
	  (strcpy(buf,"CB"));
  else if (strcmp(BUF,"d")==0)
	  (strcpy(buf,"CC"));
  else
	 printf("error"); //만약 어느 값도 받지 못했다면 프린트하라

  printf("\n");

}

void closed(int clnt_sock,int serv_sock)//sock폐쇠 함수
{
	close(clnt_sock);
	close(serv_sock);

}

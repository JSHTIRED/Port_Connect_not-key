/*
 * app.c
 *
 *  Created on: Sep 24, 2023
 *      Author: iot22
 */

#include "main.h"
#include "app.h"
#include "string.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"
#include "lwip/api.h"
#include "lwip/ip_addr.h"
#include "lwip/arch.h"
#include "lwip/sockets.h"

#define Total 10
uint16_t Port1=0;
uint16_t t=6002;
int sock;
struct sockaddr_in server_addr;
char message[70];
char msg[10];
int a=0,b=0,c=0,d1=0,d2=0;
int cds=0;
int n=0,n1=0;
int i=0,o=0;
int IN=0,OUT=0;
int val = 0;
int R;
uint32_t  INTime = 0;
uint32_t  OUTTime = 0;
uint32_t  checkTime = 0;
uint32_t  setTime = 0;
uint32_t  wwTime = 0,wwTime2 = 0;
uint32_t  delayTime = 0,delay2Time=0;
int distance=0, duration=0;
int Y=0;
//pa5==trig
//pc7==in sensor
//pe0 == out sensor
//pf13==w sensor
void call(void);
void equal(void);
void not_equal(void);
void Count_Person(void);
void HAL_GPIO_ReadPin1(uint16_t GPIO_Pin);
void TcpClientConnect(int A);
//void TcpClientConnect(void);
//void aaaa(int wq);
//void close1(void);

void app(void)
{
	call();
	osDelay(30);
	Count_Person();
}
void call(void)
{
	setTime=HAL_GetTick();
	HAL_GPIO_ReadPin1(in_sensor_Pin);
	HAL_GPIO_ReadPin1(out_sensor_Pin);
	HAL_GPIO_ReadPin1(wpin_Pin);
}
void Count_Person(void)
{
	if(i==o)
	{
		equal();
		osDelay(30);
	}
	else if(i!=o)
	{
		not_equal();
	}
	R=(((float)n/(float)Total)*100);
	printf("i: %d, o: %d, n: %d, R: %d\n",i,o,n,R);
	osDelay(30);
}

void insensor(void)
{
	i++;
	osDelay(30);
	INTime = HAL_GetTick();
	osDelay(30);
	printf("i= %d\n",i);
	//echo1();
}
void outsensor(void)
{
	o++;
	osDelay(30);
	OUTTime = HAL_GetTick();
	osDelay(30);
	printf("o= %d\n",o);
}
void HAL_GPIO_ReadPin1(uint16_t GPIO_Pin)
{
	  if(GPIO_Pin == in_sensor_Pin)
	  {
		a=HAL_GPIO_ReadPin(in_sensor_GPIO_Port,in_sensor_Pin);
		if(a<1)
		{
			HAL_GPIO_WritePin(GPIOB, LD3_Pin, GPIO_PIN_SET);
			insensor();
			HAL_GPIO_WritePin(GPIOB, LD3_Pin, GPIO_PIN_RESET);
		}
	  }
	  else if(GPIO_Pin == out_sensor_Pin)
	  {
		b=HAL_GPIO_ReadPin(out_sensor_GPIO_Port,out_sensor_Pin);
		if(b<1)
		{
			HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_SET);
			outsensor();
			HAL_GPIO_WritePin(GPIOB, LD2_Pin, GPIO_PIN_RESET);
		}
	  }
	  else if(GPIO_Pin == wpin_Pin)
	  {
		c=HAL_GPIO_ReadPin( wpin_GPIO_Port,wpin_Pin);
		printf("C: %d\n",c);
		if(c>0)
		{
			HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_RESET);
		}
	  }
}
void not_equal(void)
{
	checkTime = HAL_GetTick();
	   // 만약 측정이 잘못된 이후 사람의 출입이 있었다면 나오는 오류 수정 필요 08/31
	printf("not equal\n");
	      if(i<o) // 들어가려다 나옴 = 들어가지 않음
	      {
	        if((checkTime-OUTTime)>5000)//이동이 없음= 센서오류
	        {
	          o--;
	          INTime=0;
	          OUTTime=0;
	          printf("not equal : o--\n");
	        }
	      }
	      else if(i>o) // 나가려다 들어옴 = 나가지 않음
	      {
	        if((checkTime-INTime)>5000)
	          {

	              i--;
	              INTime=0;
	              OUTTime=0;
	              printf("not equal : i--\n");
	          }
	      }
}
void equal(void)
{
	//fix_order();
	    if(OUTTime<INTime)
	    {
	      INTime=0;
	      OUTTime=0;

	      HAL_GPIO_WritePin(GPIOB, LD3_Pin,GPIO_PIN_SET);
	      HAL_GPIO_WritePin(GPIOB, LD1_Pin, GPIO_PIN_RESET);
	      n--;
	      printf("n--\n");

	      if(n < 0) // 인원 수 보정
	        n = 0;
	    }
	    else if(OUTTime>INTime)
	    {
	      INTime=0;
	      OUTTime=0;

	     HAL_GPIO_WritePin(GPIOB, LD1_Pin,GPIO_PIN_SET);
	     HAL_GPIO_WritePin(GPIOB, LD3_Pin, GPIO_PIN_RESET);

	      n++;
	      printf("n++\n");
	    }
}
void coonnect(void)
{
	if(c>0)
	{
		do{
		if(n>0)
		{
			int B=2;
			TcpClientConnect(B);
			if(B==2)
			{
				int B=5;
				TcpClientConnect(B);
			}
		}
		else
		{
			int B=3;
			TcpClientConnect(B);
			if(B==3)
			{
				int B=6;
				TcpClientConnect(B);
			}

		}}while(c<1);
	}
	else
	{
		int B=1;
		TcpClientConnect(B);
		if(B==1)
		{
			int B=4;
			TcpClientConnect(B);
		}
	}
}
void TcpClientConnect(int A)
{
	if(Port1!=0)
	{
		t=Port1+2;
	}
	printf("port1: %d\n",Port1);
	sock = lwip_socket(AF_INET, SOCK_STREAM, 0);
	// Set up server address and port

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(t);
	server_addr.sin_addr.s_addr = inet_addr("10.10.141.82");

	    // Connect to the server
	if(lwip_connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
		perror("connect");
	printf("TcpClientConnect connect\n");
	// Send a message to the server
	lwip_write(sock, message, strlen(message));
	switch(A)
	{
			case 1:

				sprintf(message,"[[%s]AAA@%d\n]","KSH_STM",R);
				lwip_write(sock, message, strlen(message));
			case 4:
				sprintf(message,"[[%s]AAA@%d\n]","14",R);
				lwip_write(sock, message, strlen(message));
			case 2:
				sprintf(message,"[[%s]emergency@%d\n]","119",n);
				lwip_write(sock, message, strlen(message));
			case 5:
				sprintf(message,"[[%s]emergency@%d\n]","order",n);
				lwip_write(sock, message, strlen(message));
			case 3:
				sprintf(message,"[[%s]flood@\n]","KSH_STM");
				lwip_write(sock, message, strlen(message));
			case 6:
				sprintf(message,"[[%s]flood@\n]","order");
				lwip_write(sock, message, strlen(message));

	}
	read(sock,msg,10);
	Port1=atoi(msg);
	printf("aaaa: %s\n",msg);
	lwip_close(sock);
}
//void aaaa(int wq)
//{
//
//		switch(wq)
//		{
//				case 1:
//					sprintf(message,"[[%s]AAA@%d\n]","KSH_STM",R);
//					lwip_write(sock, message, strlen(message));
//				case 2:
//					sprintf(message,"[[%s]emergency@%d\n]","119",n);
//					lwip_write(sock, message, strlen(message));
//				case 3:
//					sprintf(message,"[[%s]emergency@%d\n]","order",n);
//					lwip_write(sock, message, strlen(message));
//				case 4:
//					sprintf(message,"[[%s]AAA@%d\n]","KSH_STM",R);
//					lwip_write(sock, message, strlen(message));
//				case 5:
//					sprintf(message,"[[%s]flood@\n]","KSH_STM");
//					lwip_write(sock, message, strlen(message));
//				case 6:
//					sprintf(message,"[[%s]flood@\n]","order");
//					lwip_write(sock, message, strlen(message));
//
//		}
//}
//void close1(void)
//{
//	if(lwip_close(sock)<0)
//	{
//		perror("close");
//	}
//}
//UART 통신 인터페이스에 대한 구성 및 상태 정보를 보유하는 구조체 선언(UART 인스턴스 선언)
extern UART_HandleTypeDef huart3;

//UART로 1개의 문자씩 출력하는 함수
__weak int __io_putchar(int ch)
{
	//UART3를 통해 단일 문자를 전송함
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xffff);
	/*
	 &huart3 : 사용하려는 UART 디바이스에 대한 포인터(이 경우 UART3)
	 (uint8_t *)&ch : 전송할 문자에 대한 포인터, uint8_t 포인터로 캐스트
	 1 : 전송할 바이트 수
	 0xffff : 전송에 대한 시간 제한 값(밀리초)입니다. 이 경우 매우 긴 제한 시간이 지정되어 전송이 완료될 때까지 사실상 무기한 대기함
	 */

	return ch;
}

/*void echo1(void) //에코값 받아오기
{

	volatile uint32_t curr = HAL_GetTick();
	if(curr - prev > 250){
		HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	if (__HAL_TIM_GET_COUNTER (&htim2) < 10);  // wait for 10 us
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

		pMillis = HAL_GetTick();
		while (!(HAL_GPIO_ReadPin (ECHO_PORT, ECHO_PIN)) && pMillis + 10 >  HAL_GetTick());
		val1 = __HAL_TIM_GET_COUNTER (&htim3);

		pMillis = HAL_GetTick();
		while ((HAL_GPIO_ReadPin (ECHO_PORT, ECHO_PIN)) && pMillis + 50 > HAL_GetTick());
		val2 = __HAL_TIM_GET_COUNTER (&htim3);

		distance = (val2-val1)*0.34/2;
		sprintf(string,"%d.%d cm", distance/10, distance%10);
		printf("Distance: %s\r\n", string);

		prev = curr;
	}
	d1=HAL_GPIO_ReadPin(trig_port,trig);
	osDelay(10);
	d2=HAL_GPIO_ReadPin(trig_port,trig);
	duration = pulseIn (ECHO, HIGH);
	distance = duration * 17 / 1000; //거리계산
	idt1=distance;
	Y=1;
	delayTime=millis();
}
void echo2(void) //에코값 비교하기
{
	d1=HAL_GPIO_ReadPin(trig_port,trig);
	osDelay(10);
	d2=HAL_GPIO_ReadPin(trig_port,trig);
	duration = pulseIn (ECHO, HIGH);//
	distance = duration * 17 / 1000; //거리계산
	idt2=distance;
	Y=0;
}*/
/*if(Y==1)//outsensor에서 감지
	{
		delay2Time=millis();
	    if((delay2Time-delayTime)>1000) //1000millis이후에 측정
	    {
	      echo2();
	      delayTime=0;
	    }*/

/*
void fix_order()// 값이 정상적이지 않으면
{
  if(n!=n1) //값이 다르면 관리자 호출
  {
    sprintf(sendBuf, "[%s]CP error@%d,%d\n", "KSH_STM", n,n1);
    BTSerial.write(sendBuf);
  }
}

void aaa() //비교 함수
{
  if(idt1<idt2)//in센서가 마지막
  {
    n1++;
  }
  else if(idt2<idt1)
  {
    n1--;
  }
}


// 인원체크(재실측정) 함수
void CP()
{
  if(Y==1)//outsensor에서 감지
  {
    delay2Time=millis();
    if((delay2Time-delayTime)>1000) //1000millis이후에 측정
    {
      echo2();
      delayTime=0;
    }

  }
//idt1과 idt2로 값의변화를 측정

    // 데이터 전송
    if(setTime % 100)
    {
      Serial.print("val= ");
      Serial.println(val);

      if(val<300)
      {
        sprintf(sendBuf, "[%s]AAA@%d\n", recvId, R);
        BTSerial.write(sendBuf);
        delay(5);
        sprintf(sendBuf, "[%s]AAA@%d\n", "KSH_STM", R);
        BTSerial.write(sendBuf);
      }
      else if(val>300)
      {
        sprintf(sendBuf, "[%s]flood\n", recvId);
        BTSerial.write(sendBuf);
        delay(5);
        sprintf(sendBuf, "[%s]flood\n", "KSH_STM");
        BTSerial.write(sendBuf);
      }
    }
  }
}

void emergency()
{
  int k;

  Serial.begin(115200); // 부저 출력을 위한 변경
  for(k=0;k<n;k++) //부저 이벤트
  {
    tone(Buzzer1, 261);// send pulse
    delay (1) ;// delay 1ms
    noTone(Buzzer1);
    delay (1) ;
  }

  Serial.begin(9600); //통신을 위한 변경

  sprintf(lcdLine1, "emergency : %d", i);
  sprintf(lcdLine2, " emergency : %d", o);
  lcdDisplay(0, 0, lcdLine1);
  lcdDisplay(0, 1, lcdLine2);

  sprintf(sendBuf, "[%s]emergency@%d\n", "119", n);
  BTSerial.write(sendBuf);

  delay(5);
  sprintf(sendBuf, "[%s]emergency@%d\n", "order", n);
  BTSerial.write(sendBuf);
  sprintf(sendBuf, "[%s]emergency@%d\n", "HC_COM", n);
  BTSerial.write(sendBuf);
  sprintf(sendBuf, "[%s]emergency@%d\n", "KSH_STM", n);
  BTSerial.write(sendBuf);
  delay(500);
}

void lcdDisplay(int x, int y, char * str)
{
  int len = 16 - strlen(str);
  lcd.setCursor(x, y);
  lcd.print(str);
  for (int i = len; i > 0; i--)
    lcd.write(' ');
}*/

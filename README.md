# Port_Connect_not-key(port번호를 변경하여 보안을 향상 시키는 코드)

---app.c는 stm32용 파일임 ---



iot시스템의 보안은 key라는 고유의 비밀번호를 설정해 보안을 하는 방식임
그러나 key방식의 보안은 무한한 시도가 가능하다면 100%확률로 해킹이 가능하다는 문제가 존재
이를 해결하기 위해서 주기적으로 랜덤한 port번호를 생성하고 통신하여 보안성을 높이는 방법을 고안함
본 프로젝트는 key코드가 없는 상태에서 보안성 향상을 보여주기 위해 id:passward방식의 client가 아닌 port번호의 연속된 번호를 주어서 통신함

코드는 다음과 같은 방식으로 동작함 server와 client의 통신: client(sock 생성 후 연결 대기) -> server(연결 후 sock을 읽어오고 난수값을 저장하여 전송) -> client1,2반복
-> client(server에서 받아온 난수를 port로 사용하여 연결을 시도함) -> server(난수값을 port로 지정하여 accept) -> client1,2반복 -> 반복
// 추가로 랜덤한 변수를 uint32형으로 전송하여 port번호에 대한 보안성을 향상시킴



![image](https://github.com/JSHTIRED/Port_Connect_not-key/assets/143377935/10f1d66e-5a9a-4bd2-a7b4-f8cd7b7ba137)


![image](https://github.com/JSHTIRED/Port_Connect_not-key/assets/143377935/3ea9b947-cb7f-43f6-8000-e18fa15ec30f)

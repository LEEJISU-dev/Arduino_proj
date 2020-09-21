# 음성인식 리모컨
졸업 프로젝트

### 개발 기간
2017.09 ~ 2017.12 (4개월)

### 팀 구성 및 담당
3명 (상세설계 및 코드 구현)

### 주요 언어
- C언어
- Arduino 
	- IR센서(Infrared Ray Sensor) : 아두이노에서 IR센서용 HEX코드를 IR센서에 전송
	- 1Sheeld : 관련 어플을 통해 스마트폰의 다양한 센서 이용

### 작업 내용
#### 1. 날씨
명령어를 말하면 LCD창에 오늘의 날씨를 출력
```c
const char weatherCommand[] = "오늘 날씨 어때";
```

![image](https://user-images.githubusercontent.com/67365433/93769973-8eb78400-fc56-11ea-849e-53018f3a696a.png)

```c
/* Call an api that will return the weather status in cairo,egypt. */
HttpRequest request1("http://api.openweathermap.org/data/2.5/weather?q=Daejeon,KR&appid=4a31a9b440ac1403c36357cb7c4feaa2");

...

void onSuccess1(HttpResponse & response1)
{
  /* Using the response to query the Json chain till the required value. */
  /* i.e. Get the value of 'main' in the first object of the array 'weather' in the response. */
  /* Providing that the response is in JSON format. */
  response1["weather"][0]["main"].query();
  delay(200);
}

void onJsonReply1(JsonKeyChain & hell, char * output)
{
  if (hell == request1.getResponse()["weather"][0]["main"])
  {
     wt = (String)output;
    /* 1Sheeld responds using text-to-speech shield. */
    //TextToSpeech.say(output);
  }
}
```

#### 2. 시간
명령어를 말하면 LCD창에 지금 시각을 출력
```c
const char clockCommand[] = "지금 몇 시야";
```

![image](https://user-images.githubusercontent.com/67365433/93770268-eeae2a80-fc56-11ea-8a8f-1c2456a2613d.png)

```c
/* check if you asked for the time after calling alexa. */
  if (!strcmp(clockCommand, VoiceRecognition.getLastCommand()) && state == 1)
  { 
      if (hour > 12)
    { 
      hour -= 12;
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(hour,DEC);
      lcd.print(":");
      lcd.print(minute,DEC);
      lcd.print(" ");
      lcd.print("PM");
    }
    else {
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(hour,DEC);
      lcd.print(":");
      lcd.print(minute,DEC);
      lcd.print(" ");
      lcd.print("AM");
    }
    alexoff();
    /* 1Sheeld responds using text-to-speech. */
    /* 시간 lcd출력 넣기*/
    state = 0;
  }
```

#### 3. 리모컨 - 스마트폰 어플 이용
1Sheeld의 관련 어플리케이션을 다운받으면 스마트폰으로도 리모컨 기능을 수행

![image](https://user-images.githubusercontent.com/67365433/93770867-bf4bed80-fc57-11ea-9869-8398b09eefcd.png)

```c
if(isOnePressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E020DF, 32);
      delay(100);
      }
    }else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF8877, 32);
      delay(100);
      }
    }
...
  }else if(isSharpPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E016E9, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF22DD, 32);
      delay(100);
      }
    }
  }

...

boolean isOnePressed(){
  return Keypad.isRowPressed(0)&&Keypad.isColumnPressed(0);
}
...
boolean isSharpPressed(){
  return Keypad.isRowPressed(3)&&Keypad.isColumnPressed(2);
}

```

#### 4. 리모컨 - 음성인식
음성인식으로 리모컨 기능 수행

```c
const char onCommand[] = "전원 켜줘";
const char offCommand[] = "전원 꺼줘";
const char sbsCommand[] = "5번 틀어 줘";
const char kbsCommand[] = "7번 틀어 줘";
const char mbcCommand[] = "11번 틀어 줘";
const char muteCommand[] = "음소거";
const char modeCommand[] = "외부 입력 바꿔줘";
const char comCommand[] = "회사 바꿔줘";

...

/* This function will be invoked each time a new command is given. */
void myFunction (char *commandSpoken)
{
  /* check if any one call alexa or not. */
  if (!strcmp(mainCommand, VoiceRecognition.getLastCommand()))
  {
    alexon();
    state = 1;
  }
  /* check if you asked to play music after calling alexa. */
// 전원켜줘
  if (!strcmp(onCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Power On");
   
    //전원 신호 송신
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E040BF, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF10EF, 32);
      delay(100);
      }
    }
    // 송신종료
    delay(100);
    alexoff();
    state = 0;
  }
...
// 회사 바꿔줘
if (!strcmp(comCommand, VoiceRecognition.getLastCommand()) && state == 1)
  { 
      if (stateswitch == HIGH){    // LED 가 HIGH 면 LOW 로 바꿔준다.
      stateswitch = LOW;
      lcd.clear();
      lcd.print(F("SAMSUNG REMOTE"));
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print("Company Changed");
      }
    else{                  // LED 가 LOW 면 HIGH 로 바꿔준다.
      stateswitch = HIGH;
      lcd.clear();
      lcd.print(F("LG REMOTE"));
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print("Company Changed");
      }
    alexoff();
    /* 1Sheeld responds using text-to-speech. */
    /* 시간 lcd출력 넣기*/
    state = 0;
  }
  /* check if you asked to turn the lamp on after calling alexa. */
  
}
```

### 보완점
- 명령어 수의 한계
- 아두이노의 용량 부족으로 모션인식 추가 불가능


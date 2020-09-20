/* 진행상황
 * 
 * 모션 인식으로 리모컨 신호 송신 가능
 * 좌우 = 채널 + -
 * 상하 = 채널 10 + -
 * 가까이 = 외부입력
 * 흔들기 = 전원
 * 
 * GND = -(음극)
 * 5V = +(양극)
 * 
 * @ GESTURE 센서
 * GND = GND
 * VCC = 5V
 * SDA = A4
 * SCL = A5
 * 
 * @ IR 센서
 * 다리 긴쪽 = D3(PWM 지원 핀)
 * 현재는 +극에 220 옴 설치, 추후 트랜지스터 설치 계획(설치 후에는 5M정도까지 송신 가능)
 * 다리 짧은쪽 = GND
 * 
 * @ 택트 스위치
 * 한쪽 = GND
 * 한쪽 = D10(PWM 지원 핀)
 * 한번 누를때 마다 HIGH와 LOW를 왔다갔다함
 * 초기값은 LOW
 * HIGH일 경우 SAMSUNG, LOW일 경우 LG
 * 
 * @ LCD
 * GND = GND
 * VCC = 5V
 * SDA = A4
 * SCL = A5
 * 
 * @ 초음파센서
 * GND = GND
 * VCC = 5V
 * TRIG = 13
 * ECHO = 12
 * 
 * 현재까지 컴파일 오류 없음
 */
 
#include <Wire.h>
#include <paj7620.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal_I2C.h> // 1206 호성 lcd추가

IRsend irsend;

#define GES_REACTION_TIME    500       // You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME      800       // When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
#define GES_QUIT_TIME     500

int IRledPin =  3;    // LED connected to digital pin 3
int sw = 10;       // 스위치(SW) 핀 설정 1205 추가
LiquidCrystal_I2C lcd(0x3F, 16, 2); // 1206 호성 lcd추가

//1205 추가 시작
int stateswitch = LOW;      // 회사선택 상태
int reading;          // SW 상태
int previous = LOW;   // SW 이전 상태
 
long time = 0;        // LED가 ON/OFF 토글된 마지막 시간
long debounce = 100;  // Debounce 타임 설정
int trig = 13;           // 변수 trig를 생성하고 13를 대입한다
int echo = 12;          // 변수 echo를 생성하고 12을 대입한다
//1205 추가 종료

void setup()
{
  pinMode(IRledPin, OUTPUT); // IR
  pinMode(sw, INPUT_PULLUP); // SW 를 설정, 아두이노 풀업저항 사용    1205 추가
  pinMode(trig, OUTPUT);  // trig(13)핀을 출력모드로 설정한다
  pinMode(echo, INPUT); // echo(12)핀을 입력모드로 설정한다

  // 1206 호성 lcd추가 시작
  lcd.begin();
  lcd.backlight();
  // 1206 호성 lcd추가 종
  
  uint8_t error = 0;
  

  Serial.begin(9600);
  Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

  error = paj7620Init();      // initialize Paj7620 registers
  
  if (error) 
  {
    Serial.print("INIT ERROR,CODE:");
    Serial.println(error);
  }
  else
  {
    Serial.println("INIT OK");
  }
  Serial.println("Remote controller gestures:\n");
}

void loop()
{
  uint8_t data = 0, data1 = 0, error;


  //1205 추가 시작
  reading = digitalRead(sw);  // SW 상태 읽음
  
  //SW 가 눌려졌고 스위치 토글 눌림 경과시간이 Debounce 시간보다 크면 실행
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (stateswitch == HIGH){    // LED 가 HIGH 면 LOW 로 바꿔준다.
      stateswitch = LOW;
      lcd.clear();
      lcd.print("SAMSUNG REMOTE");
      Serial.println("SAMSUNG REMOTE");
      }
    else{                  // LED 가 LOW 면 HIGH 로 바꿔준다.
      stateswitch = HIGH;
      lcd.clear();
      lcd.print("LG REMOTE");
      Serial.println("LG REMOTE");
      }
 
    time = millis();
  }
 
  previous = reading;
  //1205 추가 종료
  
  error = paj7620ReadReg(0x43, 1, &data);       // Read Bank_0_Reg_0x43/0x44 for gesture result.
    
  if (!error) 
  {
    switch (data)                   // When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
    {
      case GES_RIGHT_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          Serial.println("Forward");
          delay(GES_QUIT_TIME);
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          Serial.println("Backward");
          delay(GES_QUIT_TIME);
        }
        else
        {
          Serial.println("CH +");
          lcd.setCursor(0,1);
          lcd.print("                ");
          lcd.setCursor(0,1);
          lcd.print("CH +");
          //1205 추가 시작
          //채널 + 신호 송신
          if(stateswitch==LOW){
            for (int i = 0; i < 3; i++) {
            irsend.sendSAMSUNG(0xE0E048B7, 32);
            delay(40);
            }
          }
          else{
            for (int i = 0; i < 3; i++) {
            irsend.sendLG(0x20DF00FF, 32);
            delay(40);
            }
          }
          // 송신종료
          //1205 추가 끝
        }          
        break;
      case GES_LEFT_FLAG: 
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          Serial.println("Forward");
          delay(GES_QUIT_TIME);
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          Serial.println("Backward");
          delay(GES_QUIT_TIME);
        }
        else
        {
          Serial.println("CH -");
          lcd.setCursor(0,1);
          lcd.print("                ");
          lcd.setCursor(0,1);
          lcd.print("CH -");
          //1205 추가 시작
          //채널 - 신호 송신
          if(stateswitch==LOW){
            for (int i = 0; i < 3; i++) {
            irsend.sendSAMSUNG(0xE0E008F7, 32);
            delay(40);
            }
          }
          else{
            for (int i = 0; i < 3; i++) {
            irsend.sendLG(0x20DF807F, 32);
            delay(40);
            }
          }
          // 송신종료
          //1205 추가 끝
        }          
        break;
      case GES_UP_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          Serial.println("Forward");
          delay(GES_QUIT_TIME);
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          Serial.println("Backward");
          delay(GES_QUIT_TIME);
        }
        else
        {
          Serial.println("Volume +");
          lcd.setCursor(0,1);
          lcd.print("                ");
          lcd.setCursor(0,1);
          lcd.print("CH 10 +");
          /*1205 추가 시작
          //음량 + 신호 송신
          if(stateswitch==LOW){
            for (int i = 0; i < 3; i++) {
            irsend.sendSAMSUNG(0xE0E0E01F, 32);
            delay(40);
            }
          }
          else{
            for (int i = 0; i < 3; i++) {
            irsend.sendLG(0x20DF40BF, 32);
            delay(40);
            }
          }
          // 송신종료
          1205 추가 끝*/
          //1205 추가 시작
          //채널 +10 신호 송신
          if(stateswitch==LOW){
            for (int i = 0; i < 5; i++) {
              for (int r = 0; r < 3; r++) {
              irsend.sendSAMSUNG(0xE0E048B7, 32);
              delay(40);
              }
            }
          }
          else{
            for (int i = 0; i < 5; i++) {
              for (int r = 0; r < 3; r++) {
              irsend.sendLG(0x20DF00FF, 32);
              delay(40);
              }
            }
          }
          // 송신종료
          //1205 추가 끝
        }          
        break;
      case GES_DOWN_FLAG:
        delay(GES_ENTRY_TIME);
        paj7620ReadReg(0x43, 1, &data);
        if(data == GES_FORWARD_FLAG) 
        {
          Serial.println("Forward");
          delay(GES_QUIT_TIME);
        }
        else if(data == GES_BACKWARD_FLAG) 
        {
          Serial.println("Backward");
          delay(GES_QUIT_TIME);
        }
        else
        {
          Serial.println("Volume -");
          lcd.setCursor(0,1);
          lcd.print("                ");
          lcd.setCursor(0,1);
          lcd.print("CH 10 -");
          /*1205 추가 시작
          //음량 - 신호 송신
          if(stateswitch==LOW){
            for (int i = 0; i < 3; i++) {
            irsend.sendSAMSUNG(0xE0E0D02F, 32);
            delay(40);
            }
          }
          else{
            for (int i = 0; i < 3; i++) {
            irsend.sendLG(0x20DFC03F, 32);
            delay(40);
            }
          }
          // 송신종료
          1205 추가 끝*/
          //1205 추가 시작
          //채널 -10 신호 송신
          if(stateswitch==LOW){
            for (int i = 0; i < 5; i++) {
              for (int r = 0; r < 3; r++) {
              irsend.sendSAMSUNG(0xE0E008F7, 32);
              delay(40);
              }
            }
          }
          else{
            for (int i = 0; i < 5; i++) {
              for (int r = 0; r < 3; r++) {
              irsend.sendLG(0x20DF807F, 32);
              delay(40);
              }
            }
          }
          // 송신종료
          //1205 추가 끝
        }          
        break;
      case GES_FORWARD_FLAG:
      
        Serial.println("Source");
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("Source");
          //1205 추가 시작
          //외부입력 신호 송신
          if(stateswitch==LOW){
            for (int i = 0; i < 3; i++) {
            irsend.sendSAMSUNG(0xE0E0807F, 32);
            delay(40);
            }
          }
          else{
            for (int i = 0; i < 3; i++) {
            irsend.sendLG(0x20DFD02F, 32);
            delay(40);
            }
          }
          // 송신종료
          //1205 추가 끝

        delay(GES_QUIT_TIME);
        break;
      case GES_BACKWARD_FLAG:     
        Serial.println("Backward");
       
        delay(GES_QUIT_TIME);
        break;
      case GES_CLOCKWISE_FLAG:
        Serial.println("Clockwise");
        break;
      case GES_COUNT_CLOCKWISE_FLAG:
        Serial.println("anti-clockwise");
        break;  
      default:
        paj7620ReadReg(0x44, 1, &data1);
        if (data1 == GES_WAVE_FLAG) 
        {
          Serial.println("Power");
          lcd.setCursor(0,1);
          lcd.print("                ");
          lcd.setCursor(0,1);
          lcd.print("Power");
         
          //1205 추가 시작
          //전원 신호 송신
          if(stateswitch==LOW){
            for (int i = 0; i < 3; i++) {
            irsend.sendSAMSUNG(0xE0E040BF, 32);
            delay(40);
            }
          }
          else{
            for (int i = 0; i < 3; i++) {
            irsend.sendLG(0x20DF10EF, 32);
            delay(40);
            }
          }
          // 송신종료
          //1205 추가 끝
        }
        break;
    }
  }
  

  digitalWrite(trig, HIGH);
  // trig(13)핀에 HIGH신호를 보낸다
  delayMicroseconds(10);
  // 10마이크로초(1/100,000초) 동안
  digitalWrite(trig, LOW);
  // trig(13)핀에 LOW신호를 보낸다
  int distance1 = pulseIn(echo, HIGH) * 17 / 1000;
  delay(100);
  // 딜레이 0.1초
  digitalWrite(trig, HIGH);
  // trig(13)핀에 HIGH신호를 보낸다
  delayMicroseconds(10);
  // 10마이크로초(1/100,000초) 동안
  digitalWrite(trig, LOW);
  // trig(13)핀에 LOW신호를 보낸다
  int distance2 = pulseIn(echo, HIGH) * 17 / 1000;
  if(distance1<30&&distance2<30) {
    if(distance1==distance2) {
      Serial.println("-----");
    } else if(distance1<distance2) {
      Serial.println("Volume +");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print("Volume +");
      //1205 추가 시작
      //음량 + 신호 송신
      if(stateswitch==LOW) {
        for (int i = 0; i < 3; i++) {
          irsend.sendSAMSUNG(0xE0E0E01F, 32);
          delay(40);
        }
      } else {
        for (int i = 0; i < 3; i++) {
          irsend.sendLG(0x20DF40BF, 32);
          delay(40);
        }
      }
    } else if(distance1>distance2) {
      Serial.println("Volume -");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print("Volume -");
      //1205 추가 시작
      //음량 - 신호 송신
      if(stateswitch==LOW) {
        for (int i = 0; i < 3; i++) {
          irsend.sendSAMSUNG(0xE0E0D02F, 32);
          delay(40);
        }
      } else {
        for (int i = 0; i < 3; i++) {
          irsend.sendLG(0x20DFC03F, 32);
          delay(40);
        }
      }
    }
  }
}

/*

  OPTIONAL:
  To reduce the library compiled size and limit its memory usage, you
  can specify which shields you want to include in your sketch by
  defining CUSTOM_SETTINGS and the shields respective INCLUDE_ define.

*/

#define CUSTOM_SETTINGS
#define INCLUDE_INTERNET_SHIELD
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_CLOCK_SHIELD
#define INCLUDE_KEYPAD_SHIELD

//1119추가 시작
#define IRledPin  3    // LED connected to digital pin 3
//#define sw  10       // 스위치(SW) 핀 설정 
#define bu 2
#define led 13
//1119추가 종료


/* Include 1Sheeld library. */
#include <OneSheeld.h>

//1119추가 시작
#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal_I2C.h>

IRsend irsend;
//1119추가 종료


/* create an initial value for the millis counter. */
unsigned long previousMillis = 0;
/* create an intial state for calling alexa. */
bool state = 0;

/* Call an api that will return the weather status in cairo,egypt. */
HttpRequest request1("http://api.openweathermap.org/data/2.5/weather?q=Daejeon,KR&appid=4a31a9b440ac1403c36357cb7c4feaa2");


/* define the variable that will hold the hours and miniuts . */
int hour, minute;

/* Create 2 charachter arrys to catch the string you want from the sentence. */
char b[4];
char c[4];

/* define variables to hold the hour and miniuts in string. */
String strh, strm ;
String wt;

/* Voice commands set by the user. */

const char mainCommand[] = "아리야";

const char weatherCommand[] = "오늘 날씨 어때";

const char onCommand[] = "전원 켜줘";

const char offCommand[] = "전원 꺼줘";

const char clockCommand[] = "지금 몇 시야";

const char sbsCommand[] = "5번 틀어 줘";

const char kbsCommand[] = "7번 틀어 줘";

const char mbcCommand[] = "11번 틀어 줘";

const char muteCommand[] = "음소거";

const char modeCommand[] = "외부 입력 바꿔줘";

const char comCommand[] = "회사 바꿔줘";

//1119 추가 시작

LiquidCrystal_I2C lcd(0x3F, 16, 2); // lcd추가

int stateswitch = LOW;      // 회사선택 상태
int reading;          // SW 상태
int previous = LOW;   // SW 이전 상태
 
long time = 0;        // LED가 ON/OFF 토글된 마지막 시간
long debounce = 100;  // Debounce 타임 설정
//1119 추가 종료



void setup() {
  /* Start Communication. */
  OneSheeld.begin();
  /* set the led strips and the lamp to output. */
  //1119추가 시작
  pinMode(IRledPin, OUTPUT); // IR
  //pinMode(sw, INPUT_PULLUP); // SW 를 설정, 아두이노 풀업저항 사용
  pinMode(bu,OUTPUT);
  pinMode(led,OUTPUT);
  lcd.begin();
  lcd.backlight();
  if (stateswitch == LOW){    // LED 가 HIGH 면 LOW 로 바꿔준다.
      lcd.clear();
      lcd.print(F("SAMSUNG REMOTE"));
      }
    else{                  // LED 가 LOW 면 HIGH 로 바꿔준다.
      lcd.clear();
      lcd.print(F("LG REMOTE"));
      }
  //1119 추가 종료


  /* Subscribe to success callback for the request. */
  request1.setOnSuccess(&onSuccess1);
  /* Subscribe to json value replies. */
  request1.getResponse().setOnJsonResponse(&onJsonReply1);

  Internet.performGet(request1);
  /* on a voice recogniced call the function "myfunction". */
  VoiceRecognition.setOnNewCommand(&myFunction);
}

void loop ()
{
  /* make a delay for 5 seconds using millis counter to keep the voice detection alive. */
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 3000) {

    previousMillis = currentMillis;

    /* get the current time in your phone. */
    Clock.queryDateAndTime();
    /* start voice recognition. */
    VoiceRecognition.start();
  }
  /* save the smartphone hour and minutes in variables. */
  hour = Clock.getHours();
  minute = Clock.getMinutes();

  /* detect the time status if it am or pm. */
  


  /* save the hour and minutes as string. */
  strh = String(hour);
  strm = String(minute);

  /* save the string in a charchter array. */
  strh.toCharArray(b, 4);
  strm.toCharArray(c, 4);

  //1202시작

  /*reading = digitalRead(sw);  // SW 상태 읽음
  
  //SW 가 눌려졌고 스위치 토글 눌림 경과시간이 Debounce 시간보다 크면 실행
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (stateswitch == HIGH){    // LED 가 HIGH 면 LOW 로 바꿔준다.
      stateswitch = LOW;
      lcd.clear();
      lcd.print(F("SAMSUNG REMOTE"));
      }
    else{                  // LED 가 LOW 면 HIGH 로 바꿔준다.
      stateswitch = HIGH;
      lcd.clear();
      lcd.print(F("LG REMOTE"));
      }
 
    time = millis();
  }*/
  if(isOnePressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E020DF, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF8877, 32);
      delay(100);
      }
    }
  }
  else if(isTwoPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0A05F, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF48B7, 32);
      delay(100);
      }
    }
  }
  else if(isThreePressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0609F, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DFC837, 32);
      delay(100);
      }
    }
  }
  else if(isFourPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E010EF, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF28D7, 32);
      delay(100);
      }
    }
  }
  else if(isFivePressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0906F, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DFA857, 32);
      delay(100);
      }
    }
  }
  else if(isSixPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E050AF, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF6897, 32);
      delay(100);
      }
    }
  }
  else if(isSevenPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E030CF, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DFE817, 32);
      delay(100);
      }
    }
  }
  else if(isEightPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0B04F, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF18E7, 32);
      delay(100);
      }
    }
  }
  else if(isNinePressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0708F, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF9867, 32);
      delay(100);
      }
    }
  }
  else if(isZeroPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E08877, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF08F7, 32);
      delay(100);
      }
    }
  }
  else if(isAPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E048B7, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF00FF, 32);
      delay(100);
      }
    }
  }
  else if(isBPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E008F7, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF807F, 32);
      delay(100);
      }
    }
  }
  else if(isCPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0E01F, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF40BF, 32);
      delay(100);
      }
    }
  }
  else if(isDPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0D02F, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DFC03F, 32);
      delay(100);
      }
    }
  }
  else if(isStarPressed()){
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0807F, 32);
      delay(100);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DFD02F, 32);
      delay(100);
      }
    }
  }
  else if(isSharpPressed()){
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

  
  previous = reading;
  
}

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
  if (!strcmp(offCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Power Off");
   
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
  if (!strcmp(sbsCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Change SBS");
   
    //전원 신호 송신
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0906F, 32);
      delay(500);
      }
      irsend.sendSAMSUNG(0xE0E016E9, 32);
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DFA857, 32);
      delay(500);
      }
      irsend.sendLG(0x20DF22DD, 32);
    }
    // 송신종료
    delay(100);
    alexoff();
    state = 0;
  }

  if (!strcmp(kbsCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Change KBS");
   
    //전원 신호 송신
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E030CF, 32);
      delay(500);
      }
      irsend.sendSAMSUNG(0xE0E016E9, 32);
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DFE817, 32);
      delay(500);
      }
      irsend.sendLG(0x20DF22DD, 32);
    }
    // 송신종료
    delay(100);
    alexoff();
    state = 0;
  }

  if (!strcmp(mbcCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Change MBC");
   
    //전원 신호 송신
    if(stateswitch==LOW){
      for (int i = 0; i < 2; i++) {
      irsend.sendSAMSUNG(0xE0E020DF, 32);
      delay(1000);
      }
      irsend.sendSAMSUNG(0xE0E016E9, 32);
    }
    else{
      for (int i = 0; i < 2; i++) {
      irsend.sendLG(0x20DF8877, 32);
      delay(1000);
      }
      irsend.sendLG(0x20DF22DD, 32);
    }
    // 송신종료
    delay(100);
    alexoff();
    state = 0;
  }

  if (!strcmp(muteCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("mute");
   
    //전원 신호 송신
    if(stateswitch==LOW){
      for (int i = 0; i < 1; i++) {
      irsend.sendSAMSUNG(0xE0E0F00F, 32);
      delay(500);
      }
    }
    else{
      for (int i = 0; i < 1; i++) {
      irsend.sendLG(0x20DF906F, 32);
      delay(500);
      }
    }
    // 송신종료
    delay(100);
    alexoff();
    state = 0;
  }

  if (!strcmp(modeCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Input Change");
   
    //전원 신호 송신
    if(stateswitch==LOW){
      for (int i = 0; i < 2; i++) {
      irsend.sendSAMSUNG(0xE0E0807F, 32);
      delay(500);
      }
    }
    else{
      for (int i = 0; i < 2; i++) {
      irsend.sendLG(0x20DFD02F, 32);
      delay(500);
      }
    }
    // 송신종료
    delay(100);
    alexoff();
    state = 0;
  }
  
  /* check if you asked to get the weather status after calling alexa. */
  if (!strcmp(weatherCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    Internet.performGet(request1);
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("it's ");
    lcd.print(wt);
    lcd.print(" now");
    /* Wait for 5 seconds. */
    OneSheeld.delay(5000);
    alexoff();
    state = 0;
  }
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

void alexon() {
  /* Turn on the strips in order like the same oder of amazon echo. */
      lcd.setCursor(0,1);
      lcd.print(F("                "));
      digitalWrite(led,HIGH);
      lcd.setCursor(0,1);
      lcd.print(F("I'm here"));
      digitalWrite(bu,HIGH);
      delay(100);
      digitalWrite(bu,LOW);
      delay(2000);
      lcd.setCursor(0,1);
      lcd.print(F("                "));
}



void alexoff() {

  /* Turn off the strips. */

      digitalWrite(led,LOW);
      digitalWrite(bu,HIGH);
      delay(100);
      digitalWrite(bu,LOW);
      delay(100);
      digitalWrite(bu,HIGH);
      delay(100);
      digitalWrite(bu,LOW);
      delay(2000);
      lcd.setCursor(0,1);
      lcd.print(F("                "));
}

boolean isOnePressed(){
  return Keypad.isRowPressed(0)&&Keypad.isColumnPressed(0);
}

boolean isTwoPressed(){
  return Keypad.isRowPressed(0)&&Keypad.isColumnPressed(1);
}

boolean isThreePressed(){
  return Keypad.isRowPressed(0)&&Keypad.isColumnPressed(2);
}

boolean isFourPressed(){
  return Keypad.isRowPressed(1)&&Keypad.isColumnPressed(0);
}

boolean isFivePressed(){
  return Keypad.isRowPressed(1)&&Keypad.isColumnPressed(1);
}

boolean isSixPressed(){
  return Keypad.isRowPressed(1)&&Keypad.isColumnPressed(2);
}

boolean isSevenPressed(){
  return Keypad.isRowPressed(2)&&Keypad.isColumnPressed(0);
}

boolean isEightPressed(){
  return Keypad.isRowPressed(2)&&Keypad.isColumnPressed(1);
}

boolean isNinePressed(){
  return Keypad.isRowPressed(2)&&Keypad.isColumnPressed(2);
}

boolean isZeroPressed(){
  return Keypad.isRowPressed(3)&&Keypad.isColumnPressed(1);
}

boolean isAPressed(){
  return Keypad.isRowPressed(0)&&Keypad.isColumnPressed(3);
}

boolean isBPressed(){
  return Keypad.isRowPressed(1)&&Keypad.isColumnPressed(3);
}

boolean isCPressed(){
  return Keypad.isRowPressed(2)&&Keypad.isColumnPressed(3);
}

boolean isDPressed(){
  return Keypad.isRowPressed(3)&&Keypad.isColumnPressed(3);
}

boolean isStarPressed(){
  return Keypad.isRowPressed(3)&&Keypad.isColumnPressed(0);
}

boolean isSharpPressed(){
  return Keypad.isRowPressed(3)&&Keypad.isColumnPressed(2);
}


/*
 
OPTIONAL:
To reduce the library compiled size and limit its memory usage, you
can specify which shields you want to include in your sketch by
defining CUSTOM_SETTINGS and the shields respective INCLUDE_ define. 

*/

#define CUSTOM_SETTINGS
#define INCLUDE_INTERNET_SHIELD
#define INCLUDE_VOICE_RECOGNIZER_SHIELD
#define INCLUDE_MUSIC_PLAYER_SHIELD
#define INCLUDE_CLOCK_SHIELD
#define IRledPin  3    // LED connected to digital pin 3
#define sw  10       // 스위치(SW) 핀 설정 

/* Include 1Sheeld library. */
#include <OneSheeld.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal_I2C.h>

IRsend irsend;
/* create an initial value for the millis counter. */
unsigned long previousMillis = 0;
/* create an intial state for calling alexa. */
bool state = 0;

/* Call an api that will return the weather status in cairo,egypt. */
HttpRequest request1("http://api.openweathermap.org/data/2.5/weather?q=Cairo,EG&appid=43d95b4cf5d0573e2dfe5186c160017a");

/* define the variable that will hold the hours and miniuts . */
int hour, minute;

/* Create 2 charachter arrys to catch the string you want from the sentence. */
char b[4];
char c[4];

/* define variables to hold the hour and miniuts in string. */
String strh, strm ;
/* define variable  to hold the day status am or pm. */
String noon;

/* Voice commands set by the user. */

const char mainCommand[]  = "아리야";

const char weatherCommand[] = "오늘 날씨 어때";

const char onCommand[]  = "TV 켜줘";

const char offCommand[] = "TV 꺼줘";

const char clockCommand[] = "지금 몇 시야?";

LiquidCrystal_I2C lcd(0x3F, 16, 2); // lcd추가

int stateswitch = LOW;      // 회사선택 상태
int reading;          // SW 상태
int previous = LOW;   // SW 이전 상태
 
long time = 0;        // LED가 ON/OFF 토글된 마지막 시간
long debounce = 100;  // Debounce 타임 설정

void setup() {
  /* Start Communication. */
  OneSheeld.begin();
  pinMode(IRledPin, OUTPUT); // IR
  pinMode(sw, INPUT_PULLUP); // SW 를 설정, 아두이노 풀업저항 사용

  lcd.begin();
  lcd.backlight();

      

 /* Subscribe to success callback for the request. */
 request1.setOnSuccess(&onSuccess1);
 /* Subscribe to json value replies. */
  request1.getResponse().setOnJsonResponse(&onJsonReply1);
/* on a voice recogniced call the function "myfunction". */
  VoiceRecognition.setOnNewCommand(&myFunction);
}

void loop ()
{
  /* make a delay for 5 seconds using millis counter to keep the voice detection alive. */
  unsigned long currentMillis = millis();

  reading = digitalRead(sw);  // SW 상태 읽음
  
  //SW 가 눌려졌고 스위치 토글 눌림 경과시간이 Debounce 시간보다 크면 실행
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (stateswitch == HIGH){    // LED 가 HIGH 면 LOW 로 바꿔준다.
      stateswitch = LOW;
      lcd.clear();
      lcd.print("SAMSUNG REMOTE");
      }
    else{                  // LED 가 LOW 면 HIGH 로 바꿔준다.
      stateswitch = HIGH;
      lcd.clear();
      lcd.print("LG REMOTE");
      }
 
    time = millis();
  }
 
  previous = reading;

  if (currentMillis - previousMillis >= 5000) {

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
  if (hour > 12)
  { hour -= 12;
    noon = "P M";
  }
  else {
    noon = "A M";
  }


/* save the hour and minutes as string. */
  strh = String(hour);
  strm = String(minute);

/* save the string in a charchter array. */
  strh.toCharArray(b, 4);
  strm.toCharArray(c, 4);
}

/* This function will be invoked each time a new command is given. */
void myFunction (char *commandSpoken)
{
/* check if any one call alexa or not. */
  if (!strcmp(mainCommand, VoiceRecognition.getLastCommand()))
  {
    alexon();  
    MusicPlayer.setVolume(5);
    /* if yes make the state =1 . */
    state = 1;
  }
  /* check if you asked to play music after calling alexa. */

/* check if you asked to get the weather status after calling alexa. */
   else if(!strcmp(weatherCommand,VoiceRecognition.getLastCommand())&& state == 1)
    {

     alexoff();
    Internet.performGet(request1);
    /* Wait for 5 seconds. */
    OneSheeld.delay(5000);
     state = 0;
    }
/* check if you asked for the time after calling alexa. */
  if(!strcmp(clockCommand,VoiceRecognition.getLastCommand()) && state == 1)
     {

      alexoff();
       /* 1Sheeld responds using text-to-speech. */
       TextToSpeech.say("time in cairo is");
       delay(1300);
       TextToSpeech.say(b);
        delay(600);
        TextToSpeech.say(c);
         delay(800);
         TextToSpeech.say(noon);
         delay(500);
          state = 0;
     }
/* check if you asked to turn the lamp on after calling alexa. */
  if (!strcmp(onCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Power On");
   
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
    delay(100);
    alexoff();
    state = 0;
  }
/* check if you asked to turn the lamp off after calling alexa. */
  else if (!strcmp(offCommand, VoiceRecognition.getLastCommand()) && state == 1)
  {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Power Off");
   
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
    delay(100);
    alexoff();
    state = 0;
  }
}

void onSuccess1(HttpResponse & response1)
{
  /* Using the response to query the Json chain till the required value. */
  /* i.e. Get the value of 'main' in the first object of the array 'weather' in the response. */
  /* Providing that the response is in JSON format. */
       response1["weather"][0]["main"].query();
delay(200);    
}

void onJsonReply1(JsonKeyChain & hell,char * output)
{
  if(hell == request1.getResponse()["weather"][0]["main"])
  {
   TextToSpeech.say("the weather today in cairo is");
  delay(2000);
  /* 1Sheeld responds using text-to-speech shield. */
  TextToSpeech.say(output);
    } 
}

void alexon(){
  /* Turn on the strips in order like the same oder of amazon echo. */
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("I'm here");
}

 

  void alexoff(){

    /* Turn off the strips. */
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Done!");
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("                ");
}

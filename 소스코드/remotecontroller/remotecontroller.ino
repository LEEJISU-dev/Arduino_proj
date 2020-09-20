#include <IRremote.h>
#include <IRremoteInt.h>

IRsend irsend;

int IRledPin =  3;    // LED connected to digital pin 3
int inputPin = 10;
int volUpPin = 12;
int volDownPin = 11;

 
void setup()   {                
  // initialize the IR digital pin as an output:

  pinMode(IRledPin, OUTPUT);      
  pinMode(inputPin, INPUT);
  pinMode(volUpPin, INPUT);
  pinMode(volDownPin, INPUT);
 
  Serial.begin(9600);
  Serial.println("Turn on TV");
  delay(1000);
  for (int i = 0; i < 3; i++) {
    irsend.sendSony(0xa90, 12); // Sony TV power code
    delay(40);
  }
  Serial.println("Done");
  delay(5000);
}
 
void loop()                     
{
  digitalWrite(inputPin, HIGH);
  digitalWrite(volUpPin, HIGH);
  digitalWrite(volDownPin, HIGH);
  if ((digitalRead(inputPin) == 0) && (digitalRead(volUpPin) == 1) && (digitalRead(volDownPin) == 1)){
   /* SelectInput(); */ 
    Serial.println("Change Input");
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xA50, 12);
      delay(40);
    }
    delay(500);
  }
  else if((digitalRead(inputPin) == 1) && (digitalRead(volUpPin) == 0) && (digitalRead(volDownPin) == 1)){
   /* VolumeUp(); */ 
    Serial.println("Volume Up");
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0x490, 12);
      delay(40);
    }
    delay(150);
  }
  else if ((digitalRead(inputPin) == 1) && (digitalRead(volUpPin) == 1) && (digitalRead(volDownPin) == 0)){
    /* VolumeDown(); */
    Serial.println("Volume Down");
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xC90, 12);
      delay(40);
    }
    delay(150);
  }  
  else {
    Serial.println("no command received");
  }
}

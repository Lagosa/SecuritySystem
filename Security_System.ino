#include "Keypad.h"
#include "Servo.h"

const int buzzer = 12;
const int light = 11;
const int pir = 2;
const int servo = 13;

char keys[4][4] =
{
{'1','2','3','a' },
{'4','5','6','b' },
{'7','8','9','c' },
{'*','0','#','d' }
};
byte colPins[4] = {7, 8, 9, 10}; 
byte rowPins[4] = {3, 4, 5, 6}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, 4, 4 );

Servo myServo;
const int PASSWORD_LENGTH = 4;
char password[PASSWORD_LENGTH] = {'0','0','0','1'};
char keysPressed[8];
int nrKeysPressed=0;


int isResetPassword=0;
int isSystemArmed = 0;

void armSystem()
{
  digitalWrite(light, HIGH);
  myServo.write(0);
  isSystemArmed = 1;
}

void setup() {
  
  Serial.begin(9600);
  myServo.attach(servo);
  pinMode(light,OUTPUT);
  pinMode(pir,INPUT);
  armSystem(); 
}

void resetNumbers()
{
  for(int i=0;i<8;i++)
  {
    keysPressed[i] = '0';
  }
  nrKeysPressed = 0;
}

void openDoor()
{
  myServo.write(110);
}
void lightsOn()
{
  digitalWrite(light,LOW);
}
void successBuzz()
{
  for(int i=0;i<2;i++)
  {
    tone(buzzer,500);
    delay(300);
    noTone(buzzer);
    delay(300);
  }
}

void wrongBuzz()
{
  for(int i=0;i<3;i++)
  {
    tone(buzzer,300);
    delay(500);
    noTone(buzzer);
    delay(300);
  }
}



void checkPassword()
{
  int match = 1;
  for(int i=0;i<PASSWORD_LENGTH;i++)
  {
    if(password[i] != keysPressed[i])
    {
      match = 0;
    }
  }
  if(match == 1)
  {
    openDoor();
    lightsOn();
    successBuzz();
    isSystemArmed = 0;
  }else
  {
    wrongBuzz();
  }
  resetNumbers();
}
void alarm()
{
  int alarm = 1;
  char stopAlarmKey;
  while(alarm)
  {
    stopAlarmKey = keypad.getKey();
    if(stopAlarmKey && stopAlarmKey == 'b')
    {
       alarm = 0;
    }
    tone(buzzer,1000);
    lightsOn();
    delay(500);
     stopAlarmKey = keypad.getKey();
    if(stopAlarmKey && stopAlarmKey == 'b')
    {
       alarm = 0;
    }
    noTone(buzzer);
    digitalWrite(light,HIGH);
    delay(500);
  }
  
}
void loop() {
  char keyPressed = keypad.getKey();
  if(keyPressed)
  {
    tone(buzzer,200);
    delay(100);
    noTone(buzzer);
    Serial.println(keyPressed);
    if(keyPressed=='*')
    {
      resetNumbers();
    }else if(keyPressed == '#')
    {
       checkPassword();
    }
    else if(keyPressed == 'a')
    {
      armSystem();
    }
    else
    {
      keysPressed[nrKeysPressed] = keyPressed;
      nrKeysPressed++;
    }
  }
  int pirState = digitalRead(pir);
  if(pirState == HIGH && isSystemArmed == 1)
  {
    alarm();   
  }

}

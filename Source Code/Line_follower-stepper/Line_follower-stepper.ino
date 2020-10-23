// if ENA is LOW motor is locked at last position
// this can lead to overheating of motor
// program assumes common cathode connection
#include <SoftwareSerial.h>
SoftwareSerial BT(11, 12); 


#define SW1 A0
#define SW2 A1
#define SW3 A2
#define SW4 A3
#define SW5 A4
#define SW6 A5
#define CLP 2

#define RELAY 3
#define RELAY2 7
#define DIR 5
#define PUL 6
#define ENA 4 // HIGH disables motor power
#define DIR2 9
#define PUL2 10
#define ENA2 8 // HIGH disables motor power


#define CW 1
#define CCW 0

// indicates motor is on
// this is built in LED on DP13.
#define motorOn 13

// HIGH on PUL will keep motor locked in position
// note motor will get hot after some time.

const unsigned long  M1_STEP_PERIOD_MS = 0.1;
const unsigned long M2_STEP_PERIOD_MS = 2;
const unsigned int normal = 0.1;
const float slow  = 1; // 2 / 4 e shpejtesise normale (50% me ngadal)
const float slowest = 2; // 1 / 4 e shpejtesise normnale (75% me ngadal)

unsigned long m1NextStepTime;
unsigned long m2NextStepTime;
const unsigned int StartupTime = 120000;
const unsigned int EndingTime = 120000*15;


void setup() {
  // DP13 LED

  BT.begin(9600);
  BT.println("Hello from Arduino");

  pinMode(motorOn, OUTPUT);
  digitalWrite(motorOn, LOW);

  pinMode(CLP, INPUT_PULLUP);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  pinMode(SW5, INPUT);
  pinMode(SW6, INPUT);
  
  pinMode(DIR, OUTPUT);
  digitalWrite(DIR, LOW);
  pinMode(PUL, OUTPUT);
  digitalWrite(PUL, LOW);
  pinMode(ENA, OUTPUT);
  digitalWrite(ENA, LOW); 
  pinMode(DIR2, OUTPUT);
  digitalWrite(DIR2, LOW);
  pinMode(PUL2, OUTPUT);
  digitalWrite(PUL2, LOW);
  pinMode(ENA2, OUTPUT);
  digitalWrite(ENA2, LOW); 
  digitalWrite(RELAY,HIGH);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY2,HIGH);
  pinMode(RELAY2, OUTPUT);

 
  m1NextStepTime = millis();
  m2NextStepTime = millis();
}

  char a;

void loop() {

// Activation of relay with bluetooth 
  if (BT.available())
  // if text arrived in from BT serial...
  {
    a=(BT.read());
    if (a=='1')
    {
      digitalWrite(RELAY, LOW);// Turn LIGH ON
      digitalWrite(RELAY ,LOW);
      BT.println("Light on");
    }
    if (a=='2')
    { 
      digitalWrite(RELAY, HIGH);// Turn LIGHT OFF
      digitalWrite(RELAY2,HIGH);
      BT.println("Light off");
    }
      if (a=='3')
    { 
      BT.println("Light off");
    }
    
    
  }// End of activation of relay with bluetooth 

// Delayed activation of relay

  if (millis() >= StartupTime)
    {
      digitalWrite(motorOn,HIGH);
    }
    else if (millis() >= EndingTime)
    {
      digitalWrite(motorOn,LOW);
    }// End of delayed activation of relay

  unsigned long time = millis();

  // nese senzori mesem gjen sinjal
  if ((digitalRead(SW1) == 1) && (digitalRead(SW2) == 1) && (digitalRead(SW3) == 0) && (digitalRead(SW4) == 1) && (digitalRead(SW5) == 1) )
  {
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + normal;
      LeftMotor(1);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + normal;
      RightMotor(0);
    }
  }

  // nese senzori majtas gjen sinjal
  else if ((digitalRead(SW1) == 1) && (digitalRead(SW2) == 0) && (digitalRead(SW3) == 1) && (digitalRead(SW4) == 1) && (digitalRead(SW5) == 1) )
  {
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + slow;
      LeftMotor(1);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + normal;
      RightMotor(0);
    }
  } 

  // nese senzori i fundit majtas gjen sinjal
  else if ((digitalRead(SW1) == 0) && (digitalRead(SW2) == 1) && (digitalRead(SW3) == 1) && (digitalRead(SW4) == 1) && (digitalRead(SW5) == 1) )
  {
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + slowest;
      LeftMotor(1);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + normal;
      RightMotor(0);
    }
  }

  // nese senzori djathtas gjen sinjal
  else if ((digitalRead(SW1) == 1) && (digitalRead(SW2) == 1) && (digitalRead(SW3) == 1) && (digitalRead(SW4) == 0) && (digitalRead(SW5) == 1) )
  {
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + normal;
      LeftMotor(1);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + slow;
      RightMotor(0);
    }
  }

  // nese senzori i fundit djathtas gjen sinjal
  else if ((digitalRead(SW1) == 1) && (digitalRead(SW2) == 1) && (digitalRead(SW3) == 1) && (digitalRead(SW4) == 1) && (digitalRead(SW5) == 0) )
  {
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + normal;
      LeftMotor(1);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + slowest;
      RightMotor(0);
    }
  }

  // nese senzori mesem dhe majtas gjen sinjal
  else if ((digitalRead(SW1) == 1) && (digitalRead(SW2) == 0) && (digitalRead(SW3) == 0) && (digitalRead(SW4) == 1) && (digitalRead(SW5) == 1) )
  {
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + slow;
      LeftMotor(1);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + normal;
      RightMotor(0);
    }
  } 

  // nese senzori mesem dhe djathtas gjen sinjal
  else if ((digitalRead(SW1) == 1) && (digitalRead(SW2) == 1) && (digitalRead(SW3) == 0) && (digitalRead(SW4) == 0) && (digitalRead(SW5) == 1) )
  {
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + normal;
      LeftMotor(1);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + slow;
      RightMotor(0);
    }
  } 

  // nese senzori mesem majtas dhe djathtas gjen sinjal
  else if ((digitalRead(SW1) == 1) && (digitalRead(SW2) == 0) && (digitalRead(SW3) == 0) && (digitalRead(SW4) == 0) && (digitalRead(SW5) == 1) )
  {
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + normal;
      LeftMotor(1);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + normal;
      RightMotor(0);
    }
  }

  // nese butoni shtypet
  else if (digitalRead(CLP) == 1)
  { 
  
    if ((long)(m1NextStepTime - time) < 0)  // basically "if (time > m1NextStepTime)"
    {
      m1NextStepTime = time + normal;
      LeftMotor(0);
          digitalWrite(motorOn, HIGH);
    }
    if ((long)(m2NextStepTime - time) < 0)  // basically "if (time > m2NextStepTime)"
    {
      m2NextStepTime = time + normal;
      RightMotor(1);
    }
  }
  
} // end loop


void lockMotor(void) {
  digitalWrite(motorOn, HIGH);
  digitalWrite(ENA, LOW);
  digitalWrite(ENA2, LOW);
}

void unlockMotor(void) {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENA2, HIGH);
}

void LeftMotor(int a) 
{
    digitalWrite(DIR, a); 
    digitalWrite(ENA, LOW);
    digitalWrite(PUL, 1);
    digitalWrite(PUL, 0);

}

void RightMotor(int a) 
{
    digitalWrite(DIR2, a);  
    digitalWrite(ENA2, LOW);
    digitalWrite(PUL2, 1);
    digitalWrite(PUL2, 0);

}



/*
Ex.
motorRun(10, CW);
motorRun(10, CCW);
motorRun(100, CW);
motorRun(50, CCW);
motorRun(50, CCW);

*/

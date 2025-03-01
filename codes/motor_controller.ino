#include "BTS7960.h"
#include <SoftwareSerial.h>

//silnik 1
#define ren1 9
#define len1 8
#define lPwm1 2
#define rPwm1 3

//silnik 2
#define ren2 7
#define len2 6
#define lPwm2 5
#define rPwm2 4

//bluetooth
#define btTX 10
#define btRX 11 //tutaj było wpisane 9 ale pod ten pin jest ren1 podpięte

BTS7960 motorController1(len1, ren1, lPwm1, rPwm1);
BTS7960 motorController2(len2, ren2, lPwm2, rPwm2);
SoftwareSerial bt(btTX, btRX);

char cmd[10]; //tablica do poleceń
char c; //wpisywany znak do tablicy
int cmdIndex = 0; //indeks do tablicy



void setup() {
  Serial.begin(9600);
  bt.begin(9600);
}

void cmdExe()
{
  //ruch lewym sliderem -> silnik 1 przód/tył
  if(cmd[0] == 'l')
  {
    float val = atof(cmd + 2);
    //Serial.println(val);
    if(val > 0) //jazda w przód
    {
      motorController1.TurnLeft(val*2);
    }
    else if(val < 0) //jazda w tył
    {
      motorController1.TurnRight((-val)*2);
    }
    else
    {
      motorController1.Stop();
    }
  }
  else if(cmd[0] == '1') //przycisk do zatrzymania silnika 1
  {
    motorController1.Stop();
  }

  //ruch prawym sliderem -> silnik 2 przód/tył
  if(cmd[0] == 'r')
  {
    float val = atof(cmd + 2);
    //Serial.println(val);
    if(val > 0) //jazda w przód
    {
      motorController2.TurnLeft(val*2);
    }
    else if(val < 0) //jazda w tył
    {
      motorController2.TurnRight((-val)*2);
    }
    else
    {
      motorController2.Stop();
    }
  }
  else if(cmd[0] == '3') //przycisk do zatrzymania silnika 1
  {
    motorController2.Stop();
  }
}

void loop() {
  motorController1.Enable();
  motorController2.Enable();

  //odbieranie poleceń przez bt
  if(bt.available() > 0)
  {
    char c = (char)bt.read();

    if(c == '\n') //jeśli polecenie skończone
    {
      cmd[cmdIndex] = c;
      cmdExe();
      cmdIndex = 0;
      for(int i = 0; i < cmdIndex; i++)
      {
        cmd[i] = 0;
      }
    }
    else //jeśli polecenie w trakcie
    {
      cmd[cmdIndex] = c;
      if(cmdIndex < 9)
      {
        cmdIndex++;
      }
    }
  }

  motorController1.Disable();
  motorController2.Disable();
}

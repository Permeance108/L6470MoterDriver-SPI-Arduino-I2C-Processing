//import Wire.h for I2C
#include <Wire.h>

//pin # for arduino nano SPI
#define AC100_N 9
#define AC100_L 10
#define DC24_N 11
#define DC24_L 12

int rec[50];
int states;
int ArdBUSY=999;
boolean IniI2C=true;

void setup() {
  pinMode(AC100_N, OUTPUT);
  pinMode(AC100_L, OUTPUT);
  pinMode(DC24_N, OUTPUT);
  pinMode(DC24_L, OUTPUT);
  digitalWrite(AC100_N, HIGH);
  digitalWrite(DC24_N, HIGH);
  digitalWrite(AC100_L, HIGH);
  digitalWrite(DC24_L, HIGH);
  Serial.begin(19200);
  Serial.write("init \n");
}

void loop() {
  if(IniI2C){
    Wire.begin(0x10);// Slave addressの宣言をする
    Wire.onReceive(processMessage); //Raspberry Piから何かを受け取るたび、processMessage関数を呼び出す 
    delay(1000);
    }
}

void processMessage(int n){
  if(n%3==0){
    for(int i=0; i<n; i++ ){
      rec[i]=Wire.read();
    }
    for(int i=0; i<n; i=i+3){
      int var=makeWord(rec[i+1],rec[i+2]);//2バイト分を合成する
      switch(rec[i]){
        case 0:
          digitalWrite(AC100_N, HIGH);
          delay(10000);
          digitalWrite(AC100_L, HIGH);
          break;
        case 1:
          digitalWrite(AC100_N, LOW);
          delay(10000);
          digitalWrite(AC100_L, LOW);
          break;
        case 14:
          digitalWrite(DC24_N, HIGH);
          delay(10000);
          digitalWrite(DC24_L, HIGH);
          break;
        case 15:
          digitalWrite(DC24_N, LOW);
          delay(10000);
          digitalWrite(DC24_L, LOW);
          break;
      }
      Serial.println(rec[i]);
      Serial.println(var);
    }
  }else{
    Serial.println(-1);
  }
  IniI2C=false;
}


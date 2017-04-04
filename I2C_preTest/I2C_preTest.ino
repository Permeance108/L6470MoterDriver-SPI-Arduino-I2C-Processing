//import Wire.h for I2C, SPI.h for SPI
#include <Wire.h>
#include <SPI.h>

//pin # for arduino nano SPI
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10
#define PIN_BUSY 9

int rec[50];
int command=0;
int states;
long temp=0;
int ArdBUSY=999;
boolean IniI2C=true;

void setup() {
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT);
  digitalWrite(PIN_SPI_SS, HIGH);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
    L6470_resetdevice(); //L6470リセット
    L6470_setup();  //L6470を設定
  Serial.begin(19200);
  Serial.write("init \n");
}

void loop() {
  if(IniI2C){
    Wire.begin(0x3D);// Slave addressの宣言をする
    Wire.onReceive(processMessage); //Raspberry Piから何かを受け取るたび、processMessage関数を呼び出す 
    Wire.onRequest(requestEvent);//Raspberry Piから要求された場合に、sendAnalogReading関数を呼び出す
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
        case 1:
          states=1;
          break;
        case 5:
          L6470_setparam_acc(var);
          break;
        case 6:
          L6470_setparam_dec(var);
          break;
        case 7:
          L6470_setparam_maxspeed(var);
          break;
        case 8:
          L6470_setparam_minspeed(var);
          break;
        case 9:
          L6470_setparam_kvalhold(var);
          break;
        case 10:
          L6470_setparam_kvalrun(var);
          break;
        case 11:
          L6470_setparam_kvalacc(var);
          break;
        case 12:
          L6470_setparam_kvaldec(var);
          break;
        case 99:
          command=99;
          temp=var;
          break;
      }
      Serial.println(rec[i]);
      Serial.println(var);
    }
  }else{
    Serial.println(-1);
  }
}

void requestEvent() {
  if(command==99){
    Wire.write(0);
    Wire.write(0);  
    L6470_move(0,25600*temp);
    delay(100);
    command=0;
  }else if(states=1){
    if(!digitalRead(PIN_BUSY)){
      Wire.write(ArdBUSY);
      Wire.write(ArdBUSY);
    }else{
      Wire.write(0);
      Wire.write(0);
    }
    states=0;
  }
  temp=0;
  IniI2C=false;
}

void L6470_setup() {
  L6470_setparam_acc(0x8A); //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
  L6470_setparam_dec(0x8A); //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
  L6470_setparam_maxspeed(0xc0); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
  L6470_setparam_minspeed(0x01); //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val[step/s])
  L6470_setparam_fsspd(0x027); //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
  L6470_setparam_kvalhold(0x29); //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
  L6470_setparam_kvalrun(0x9f); //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
  L6470_setparam_kvalacc(0x9f); //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
  L6470_setparam_kvaldec(0x9f); //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
  L6470_setparam_stepmood(0x07); //ステップモードdefault 0x07 (1+3+1+3bit)
}


#include <SPI.h>
#include <MsTimer2.h>
//#include <Boards.h>
#include <stdio.h>
//#include <math.h>
#include <SoftwareSerial.h>

SoftwareSerial temp(0, 1); //(rx,tx)

#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10
#define PIN_BUSY 9
#define OR_SW 17

int InVal;
int Speed=10;
unsigned int Dist=20000;
int Wait=5000;
int RepN=5;
int Flag=0;
int Origin=0;
int Move=0;
int Pos=100;
int last=100;
unsigned int delt=0;

void setup() {
  //Serial for BT start(RN-42 default:9600)
  //※when RN-42 dip_sw No.4:ON = 9600bps
  temp.begin(9600);

  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT);
  pinMode(OR_SW, INPUT);
  digitalWrite(PIN_SPI_SS, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  Serial.begin(19200);

    L6470_resetdevice(); //L6470リセット
    L6470_setup();  //L6470を設定
    MsTimer2::set(50, fulash);//シリアルモニター用のタイマー割り込み
    MsTimer2::start();
  
  Serial.write("init \n");
}

void loop() {
  if (temp.available()) {
    InVal = temp.read();
    Serial.println(InVal);
    if (InVal == 1) {
      Flag=1;
      Origin=1;
    }else if(InVal == 2){
      Flag=0;
      Origin=0;
      L6470_resetdevice(); //L6470リセット
      L6470_setup();  //L6470を設定
    }else if(InVal < 8){
      Speed=(InVal-2)*2;
    }else if(InVal < 13){
      Dist=InVal*10000-70000;
      Serial.println(Dist);
    }else if(InVal <41){
      Wait=(InVal-20)*1000;
    }else if(InVal <61){
      RepN=InVal-40;
    }else if(InVal==99){
      Flag=1;
      Origin=1;
      Move=0;
      RepN=1;
      Dist=0;
      Wait=1;
      Pos=100;
      last=100;
    }else if(InVal <250){
      Move=1;
      Pos=InVal;
     }
  }
  
  if(Flag==1){
    if(Origin==1){
    L6470_setparam_maxspeed(2);
      while(analogRead(OR_SW) < 200){
        L6470_move(0,1000);
        L6470_busydelay(1);
        Serial.println(analogRead(OR_SW));
        }
        Origin=0;
        L6470_move(1,20000);
        L6470_busydelay(1000);
    }
    L6470_setparam_maxspeed(Speed); //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
    L6470_move(1,Dist*2);//指定方向に指定数ステップする
    L6470_busydelay(Wait);//delay(Wait);
    L6470_move(0,Dist*2);//指定方向に指定数ステップする
    L6470_busydelay(Wait);//delay(Wait);
    RepN-=1;
    Serial.println(RepN);
    if(RepN==0){
      Flag=0;
      L6470_resetdevice(); //L6470リセット
      L6470_setup();  //L6470を設定
    }
  }
  
  if(Move==1){
    L6470_setparam_maxspeed(5);
    delt=abs(Pos - last);
    Move=0;
    if(Pos>last){
      L6470_move(1,500*delt);
      Serial.println(delt);
      L6470_busydelay(10);
      last=Pos;
    }else if(Pos < last){
      L6470_move(0,500*delt);
      Serial.println(delt);
      L6470_busydelay(10);
      last=Pos;
    }else if(Pos == last){
      Move=0;
    }
  }  
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

void fulash() {
//  Serial.print("0x");
//  Serial.print( L6470_getparam_abspos(), HEX);
//  Serial.print("  ");
//  Serial.print("0x");
//  Serial.println( L6470_getparam_speed(), HEX);
}

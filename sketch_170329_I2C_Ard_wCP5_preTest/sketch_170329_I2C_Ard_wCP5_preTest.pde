import processing.io.*;
import controlP5.*;

I2C i2c;
ControlP5 cp5;

byte[] rv=new byte[2];
int sig=0, command=0;
int Acc=80, Dec=80, VelMax=12, VelMin=12;
int VoltHold=100, VoltRun=100, VoltAcc=100, VoltDec=100;
int MoveTime=2;
int AccR, DecR, VelMaxR, VelMinR,VoltHoldR, VoltRunR, VoltAccR, VoltDecR;

void setup() {
  size(500, 440);
  cp5 = new ControlP5(this);
  cp5.addSlider("Acc").setPosition(20, 25).setSize(20, 250).setRange(0, 100).setNumberOfTickMarks(51);
  cp5.addSlider("Dec").setPosition(80, 25).setSize(20, 250).setRange(0, 100).setNumberOfTickMarks(51);
  cp5.addSlider("VelMax").setPosition(140, 25).setSize(20, 250).setRange(0, 100).setNumberOfTickMarks(51);
  cp5.addSlider("VelMin").setPosition(200, 25).setSize(20, 250).setRange(0, 100).setNumberOfTickMarks(51);
  cp5.addSlider("VoltHold").setPosition(260, 25).setSize(20, 250).setRange(0, 100).setNumberOfTickMarks(51);
  cp5.addSlider("VoltRun").setPosition(320, 25).setSize(20, 250).setRange(0, 100).setNumberOfTickMarks(51);
  cp5.addSlider("VoltAcc").setPosition(380, 25).setSize(20, 250).setRange(0, 100).setNumberOfTickMarks(51);
  cp5.addSlider("VoltDec").setPosition(440, 25).setSize(20, 250).setRange(0, 100).setNumberOfTickMarks(51);
  cp5.addSlider("MoveTime").setPosition(20, 300).setSize(20, 120).setRange(1, 10).setNumberOfTickMarks(10);
  cp5.addButton("MoveB").setPosition(80, 300).setSize(120, 80).setValue(0);

  i2c = new I2C(I2C.list()[0]);
  frameRate(6);
  print(i2c + "\n");
  //i2c.beginTransmission(0x3d);
  //i2c.write(0);
  //i2c.write(0);
  //i2c.write(0);
  //i2c.endTransmission();
}

void draw() {
  if (command==99) {
    i2c.beginTransmission(0x3d);
    i2c.write(1);
    i2c.write(0);
    i2c.write(0);
    delay(10);
    rv=i2c.read(2);
    int sig =int(rv[1])*256+int(rv[0]);
    print("in:" + sig +"\n");   
    i2c.endTransmission();
    delay(10);
    
    scal();
    
    i2c.beginTransmission(0x3d);
    if (sig!=999) {
      i2c.write(5);
      i2c.write(AccR >> 8);
      i2c.write(AccR & 255);
      i2c.write(6);
      i2c.write(DecR >> 8);
      i2c.write(DecR & 255);
      i2c.write(7);
      i2c.write(VelMaxR >> 8);
      i2c.write(VelMaxR & 255);
      i2c.write(8);
      i2c.write(VelMinR >> 8);
      i2c.write(VelMinR & 255);
      i2c.write(9);
      i2c.write(VoltHoldR >> 8);
      i2c.write(VoltHoldR & 255);
      i2c.write(10);
      i2c.write(VoltRunR >> 8);
      i2c.write(VoltRunR & 255);
      i2c.write(11);
      i2c.write(VoltAccR >> 8);
      i2c.write(VoltAccR & 255);
      i2c.write(12);
      i2c.write(VoltDecR >> 8);
      i2c.write(VoltDecR & 255);
      i2c.write(command);
      i2c.write(MoveTime >> 8);
      i2c.write(MoveTime & 255);
    }
    command=0;
    //print("out:" + command +"\n");
    delay(10);
    rv=i2c.read(2);
    //int sig =int(rv[1])*256+int(rv[0]);
    //print("in:" + sig +"\n");    
    i2c.endTransmission();
  }
}

void MoveB() {
  command=99;
}

void scal(){
  AccR=int(Acc*2.5);
  DecR=int(Dec*2.5);
  VelMaxR=int(VelMax*2.5);
  VelMinR=int(VelMin*2.5);
  VoltHoldR=int(VoltHold*2.5);
  VoltRunR=int(VoltRun*2.5);
  VoltAccR=int(VoltAcc*2.5);
  VoltDecR=int(VoltDec*2.5); 
}
import processing.io.*;
import controlP5.*;

ControlP5 cp5;
//CH340 ch;

int numCH=1;
CH340[] ch = new CH340[numCH];

int address=0,stByte=0,ndByte=0,rdByte=0;
boolean sndBflag=false;//,stflag=false;

void setup() {
  size(500, 400);
  background(0);
  cp5 = new ControlP5(this);
  cp5.addRadioButton("Address")
         .setPosition(20,20)
         .setSize(40,20)
         .setColorForeground(color(120))
         .setColorActive(color(255))
         .setColorLabel(color(255))
         .addItem("0x10",0).addItem("0x11",1).addItem("0x12",2).addItem("0x13",3).addItem("0x14",4)
         .addItem("0x15",5).addItem("0x16",6).addItem("0x17",7).addItem("0x18",8).addItem("0x19",9)
         ;
  cp5.addRadioButton("STByte")
         .setPosition(120,20)
         .setSize(40,20)
         .setColorForeground(color(120))
         .setColorActive(color(255))
         .setColorLabel(color(255))
         .addItem("0x0--",0).addItem("0x1--",1).addItem("0x2--",2).addItem("0x3--",3)
         .addItem("0x4--",4).addItem("0x5--",5).addItem("0x6--",6).addItem("0x7--",7)
         .addItem("0x8--",8).addItem("0x9--",9).addItem("0xa--",10).addItem("0xb--",11)
         .addItem("0xc--",12).addItem("0xd--",13).addItem("0xe--",14).addItem("0xf--",15)
         ;
  cp5.addRadioButton("NDByte")
         .setPosition(220,20)
         .setSize(40,20)
         .setColorForeground(color(120))
         .setColorActive(color(255))
         .setColorLabel(color(255))
         .addItem("0x-0-",0).addItem("0x-1-",1).addItem("0x-2-",2).addItem("0x-3-",3)
         .addItem("0x-4-",4).addItem("0x-5-",5).addItem("0x-6-",6).addItem("0x-7-",7)
         .addItem("0x-8-",8).addItem("0x-9-",9).addItem("0x-a-",10).addItem("0x-b-",11)
         .addItem("0x-c-",12).addItem("0x-d-",13).addItem("0x-e-",14).addItem("0x-f-",15)
         ;
  cp5.addRadioButton("RDByte")
         .setPosition(320,20)
         .setSize(40,20)
         .setColorForeground(color(120))
         .setColorActive(color(255))
         .setColorLabel(color(255))
         .addItem("0x--0",0).addItem("0x--1",1).addItem("0x--2",2).addItem("0x--3",3)
         .addItem("0x--4",4).addItem("0x--5",5).addItem("0x--6",6).addItem("0x--7",7)
         .addItem("0x--8",8).addItem("0x--9",9).addItem("0x--a",10).addItem("0x--b",11)
         .addItem("0x--c",12).addItem("0x--d",13).addItem("0x--e",14).addItem("0x--f",15)
         ;
  cp5.addButton("sendB")
         .setValue(0)
         .setPosition(410, 20)
         .setSize(80, 60);
  //ch = new CH341();
  //ch = new CH340(I2C.list()[0]);
  for(int i=0;i<ch.length; i++){
    int xAd=0x10+i;
    ch[i] = new CH340(I2C.list()[i],xAd);
  }
  frameRate(6);
  //printArray(I2C.list());
}

void Address(int adS){address=adS;}
void STByte(int stB){stByte=stB;}
void NDByte(int ndB){ndByte=ndB;}
void RDByte(int rdB){rdByte=rdB;}

void draw() {
  if(sndBflag){
    //if(!stflag){
    //  stflag=true;
    //}
    //ch.send3byte(address,stByte,ndByte,rdByte); 
    ch[address].send3byte(stByte,ndByte,rdByte);
    sndBflag=false;
  }
}

void sendB(){
  sndBflag=true;
}
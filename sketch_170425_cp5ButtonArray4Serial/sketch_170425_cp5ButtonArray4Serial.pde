import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
Serial ser;

DropdownList d1, d2;
Textfield tx1, tx2;

PImage img;
int val_low,val_high,valR;
final int heightSep=140, d1num=10,d2num=30;
boolean flag=false;
//float[] d1List = new float[d1num];
//float[] d2List = new float[d2num];

void setup() {
  size(600, 400);
  colorMode(HSB);
  backGroundGradation();

  //PFont myFont1 = loadFont("MS-PGothic-24.vlw");
  //PFont myFont2 = loadFont("MS-PGothic-16.vlw");
  //textFont(myFont1);

  PImage[]iconPlay={loadImage("img_play1.png"), loadImage("img_play2.png"), loadImage("img_play3.png")};
  PImage[]iconPause={loadImage("img_pause1.png"), loadImage("img_pause2.png"), loadImage("img_pause3.png")};
  PImage[]iconSStop={loadImage("img_ss1.png"), loadImage("img_ss2.png"), loadImage("img_ss3.png")};
  PImage[]iconFStop={loadImage("img_sf3.png"), loadImage("img_sf4.png"), loadImage("img_sf5.png")};  

  cp5 = new ControlP5(this);

  cp5.addButton("systemPlay")
    .setPosition(100, 80)
    .setValue(0)
    .setImages(iconPlay)
    .updateSize()
    .setOff()
    ;
  cp5.addButton("systemPause")
    .setPosition(220, 80)
    .setValue(1)
    .setImages(iconPause)
    .updateSize()
    ;
  cp5.addButton("systemSoftStop")
    .setPosition(340, 80)
    .setValue(2)
    .setImages(iconSStop)
    .updateSize()
    ;
  cp5.addButton("systemForcedStop")
    .setPosition(460, 80)
    .setValue(3)
    .setImages(iconFStop)
    .updateSize()
    ;

  d1 = cp5.addDropdownList("axisSelect")
    .setPosition(40, 180)
    .setSize(100, 220)
    .setLabel("Axis")
    //.setFont(myFont2)
    .close()
    ;
  d2 = cp5.addDropdownList("commandSelect")
    .setPosition(150, 180)
    .setSize(100, 220)
    .setLabel("Command")
    //.setFont(myFont2)
    .close()
    ;
  customizeD(d1);
  customizeD(d2);

  tx1=cp5.addTextfield("speed")
    .setPosition(320, 200)
    .setSize(100, 30)
    //.setFont(myFont1)
    ;
  tx2=cp5.addTextfield("position")
    .setPosition(440, 200)
    .setSize(100, 30)
    //.setFont(myFont1)
    ;
  customizeT(tx1);
  customizeT(tx2);


  cp5.addButton("electPlay")
    .setPosition(340, 340)
    .setValue(10)
    .setImages(iconPlay)
    .updateSize()
    ;
  cp5.addButton("electSoftStop")
    .setPosition(460, 340)
    .setValue(12)
    .setImages(iconSStop)
    .updateSize()
    ;  

  cp5.setAutoDraw(false);

  ser=new Serial(this, "/dev/ttyUSB0", 9600);
  flag=true;
}

void draw() {
  gui();
}

void customizeD(DropdownList ddl) {
  ddl.setBackgroundColor(color(110));
  ddl.setItemHeight(30);
  ddl.setBarHeight(35);

  if (ddl.equals(d1)) {
    for (int i=0; i<10; i++) {
      ddl.addItem("axis "+ i, i);
      //d1List[i]=i+20;
    }
    //ddl.setArrayValue(d1List);
  } else if (ddl.equals(d2)) {
    for (int i=0; i<30; i++) {
      ddl.addItem("command "+i, i);
      //d2List[i]=i+30;
    }
    ddl.setValue(10);
    //ddl.setArrayValue(d2List);
  }
  ddl.setColorBackground(color(100, 200, 100));
  ddl.setColorActive(color(0, 200, 250));
}

void customizeT(Textfield txt) {
  txt.setColor(color(0, 0, 250));
  txt.setColorActive(color(0, 200, 250));
  txt.setColorBackground(color(100, 200, 100));
  txt.setColorLabel(color(50));
  if (txt.equals(tx1)) {
    txt.setLabel("Speed");
  } else if (txt.equals(tx2)) {
    txt.setLabel("Position");
  }
}

void gui() {
  backGroundGradation();
  textSet();
  logoSet();
  cp5.draw();
}

void backGroundGradation() {
  noStroke();
  color c1 = color(30, 50, 250);
  color c2 = color(30, 100, 250);
  for (float w = 0; w < heightSep; w += 5) {
    color c = lerpColor(c1, c2, w / heightSep);
    fill(c);
    rect(0, w, width, 5);
  }
  for (float w = heightSep; w < height; w += 5) {
    color c = lerpColor(c1, c2, (w - heightSep) / (height - heightSep));
    fill(c);
    rect(0, w, width, 5);
  }
}

void logoSet() {
  img = loadImage("TOWA_LOGO_640x320.png");
  image(img, 0.05*width, 0.98*height - 0.1*width, 0.2*width, 0.1*width);
}

void textSet() {
  fill(0);
  text("System Memu", 240, 25);
  text("Play", 100, 60);
  text("Pause", 200, 60);
  text("Stop", 340, 60);
  text("Reset", 440, 60);
  fill(50);
  text("Each Axis Menu", 240, 165);
  text("Play", 340, 320);
  text("Stop", 460, 320);
}
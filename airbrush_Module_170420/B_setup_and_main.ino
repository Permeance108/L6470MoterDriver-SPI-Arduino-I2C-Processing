/**********************************
 * 
 * First setup and Main rootin
 * 
 **********************************/

/********************
/*headder
********************/
#include <SPI.h>
#include <Wire.h>
#include <Bounce2.h> /*Chattering prevention*/

/***************
* Instantiate
****************/
Bounce origin_sw  = Bounce();  /*input sw*/
Bounce L6470_busy = Bounce();  /*data input *notuse*/

/****************
 * Set up
 *****************/
void setup(){

//PC debug  serial----------------------------
  Serial.begin(19200);
  Serial.write("init_start \n");

//Endstop-------------------------------------
  pinMode(ENDSTOP_1,INPUT_PULLUP);

//After setting up the button, setup the Bounce instance :
  origin_sw.attach(ENDSTOP_1);
  origin_sw.interval(DEBOUNCER_INTERVAL);     //interval in ms

//L6470---------------------------------------
//pinmode
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS, OUTPUT);
  pinMode(PIN_BUSY, INPUT);
  pinMode(OR_SW, INPUT);
  
//SPI-----------------------------------------
  digitalWrite(PIN_SPI_SS, HIGH);
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);

//L6470set up--------------------------------------
  L6470_resetdevice();
  L6470_setup(); 
  
//L6470 busy pin  --------------------------------*not use
  L6470_busy.attach(PIN_BUSY);
  L6470_busy.interval(DEBOUNCER_INTERVAL);     //interval in ms

//I2C-----------------------------------------
//setup
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(I2cRcv_interrupt);    //マスタ→スレーブ書き込み割り込み関数の指定
  Wire.onRequest(I2cRqst_interrupt);  //マスタ→スレーブ読み出し割り込み

  Serial.write("init_cmp \n");
  return;
}

/*****************
 * Main loop
 ******************/
void loop() 
{
  static byte next_module_act = START_UP;
  static byte act_status = START_UP;
    
    /*input check (Update the Bounce instances)*/ 
    origin_sw.update();
    L6470_busy.update();

    /*module action select*/
    noInterrupts();
    next_module_act = Module_act_selector(act_status);
      
    /*module action*/
    act_status = Module_action(next_module_act);
    interrupts();

    /*strage for i2c request*/
    Store_now_status_to_i2c(act_status);  
}

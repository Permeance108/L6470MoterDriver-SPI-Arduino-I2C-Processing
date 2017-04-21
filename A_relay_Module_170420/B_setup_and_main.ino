/**********************************
 * 
 * First setup and Main rootin
 * 
 **********************************/

/********************
/*headder
********************/
#include <MsTimer2.h>
#include <Wire.h>
#include <Bounce2.h> /*Chattering prevention*/

/***************
* Instantiate
****************/
Bounce glass_sw  = Bounce();  /*input sw*/

/****************
 * Set up
 *****************/
void setup(){

/* PC debug  serial */
  Serial.begin(19200);
  Serial.write("init_start \n");

/* pin mode */
  pinMode(GLASS_SW,INPUT_PULLUP);
  pinMode(AC100_L, OUTPUT);
  pinMode(AC100_N, OUTPUT);
  pinMode(DC24_H, OUTPUT);
  pinMode(DC24_G, OUTPUT);

/* first pin status*/
  digitalWrite(AC100_L, HIGH);
  digitalWrite(AC100_N, HIGH);
  digitalWrite(DC24_H,  HIGH);
  digitalWrite(DC24_G,  HIGH);

/* setup the Bounce instance : */
  glass_sw.attach(GLASS_SW);
  glass_sw.interval(DEBOUNCER_INTERVAL);     //interval in ms
  
/* I2C set up */
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(I2cRcv_interrupt);    /*マスタ→スレーブ書き込み割り込み関数の指定*/
  Wire.onRequest(I2cRqst_interrupt);  /*マスタ→スレーブ読み出し割り込み*/

/* Timer handler */
  MsTimer2::set(TIMER_INTERVAL, Time_counter); /* timer interrput */
  MsTimer2::start();

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
    glass_sw.update();

    /*module action select*/
    noInterrupts();
    next_module_act = Module_act_selector(act_status);
      
    /*module action*/
    act_status = Module_action(next_module_act);
    interrupts();

    /*strage for i2c request*/
    Store_now_status_to_i2c(act_status);  
}

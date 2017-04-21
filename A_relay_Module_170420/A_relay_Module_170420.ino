/******************************
 * 
 * A_relay_Module 17/04/20
 * 
 * 「もしコードとコメントが一致しないならば、きっと両方間違いだろう」 --- Norm Schryer
 * 
 *****************************/

/******************************
 * i2c address
 ******************************/
#define SLAVE_ADDR            0x13    //i2c hardware slave address "0x13 = relay_module"

/*******************
* Machine parametar
********************/
#define TIMER_INTERVAL 50 /* 50msec interval */ 
#define RELAY_ON  0x55
#define RELAY_OFF 0x00
#define RELAY_DELAY_TIME 2

/*********************************************
 * I2c_input_cmd = internal_status　define
 *********************************************/
//NO RESERVED(USER DEFINE) -------------------

#define HEATER_CNT   0x10
#define SOLENOID_CNT 0x11

#define TEST_ERR     0x99

//RESERVED ------------------------------------
//status
#define START_UP     0x00  //shared_use
#define RCV_WAIT     0x01  //
#define IN_ACT       0x02
#define IN_ERR       0x03

//cmd
#define NO_CMD      0x00
#define NORMAL_CMD  0x01
#define ERR_CMD     0x02
#define RESET_CMD   0x03

//Prohibited address
#define NORMAL_START  0x10
#define NORMAL_END    0x7F
#define ERR_START     0x80
#define ERR_END       0xEF
#define ERR_RESET     0xF0

/*********************
 * I2c data define
 *********************/

#define I2C_RCVDATA_LENGTH    7
#define I2C_RQSTDATA_LENGTH   2

#define BYTE_1ST 0
#define BYTE_2ND 1
#define BYTE_3RD 2
#define BYTE_4TH 3
#define BYTE_5TH 4
#define BYTE_6TH 5
#define BYTE_7TH 6

/****************************
 * arduino nano pin define
 ****************************/
//Sw or Sensor-----------------
#define GLASS_SW PD2
#define DEBOUNCER_INTERVAL 5 //msec

/*relay out*/
#define AC100_L   9
#define AC100_N   10
#define DC24_H    11
#define DC24_G    12

/********************************
 * Gloval Constant  and Variable
 ********************************/

/*グローバルは極力持つべきでないが、arduinoのハンドラは戻り値が持てないので、その代替*/
/*Global variable for I2c */

/*各種モード時のデータストレージ*/
volatile static byte  Heater_onoff = 0;
volatile static byte  Solenoid_onoff = 0;

/* I2c 受信　1stバイト */
volatile  static byte I2c_rcv_cmd = NO_CMD;
volatile  static byte I2c_rcv_data[I2C_RCVDATA_LENGTH] = {};

/* I2c　リクエスト用 */
volatile  static byte I2c_rqst_data[I2C_RQSTDATA_LENGTH] = {}; 

/* for ms2timer */
volatile static int Timecounter = 0; 


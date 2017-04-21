/******************************
 * 
 * Airbrush_Module 17/04/
 * 「もしコードとコメントが一致しないならば、きっと両方間違いだろう」 --- Norm Schryer
 *****************************/

/******************************
 * i2c address
 ******************************/
#define SLAVE_ADDR            0x12    //i2c hardware slave address "0x12 = air_brush_module"

/*******************
* Machine parametar
********************/

#define COEF_MM_CNVT_STEP         2560  //64ustep 200step/rot  0.5mm/rot   1mm =  25600step 0.1mm = 2560step
#define COEF_SPDBYTE_CNVT_MAXPPS  1     //スルー
#define COEF_SPDBYTE_CNVT_PPS     67    //MAXSPEEDとの値の是正
#define R2H_MINSPEED              0x10  //原点復帰（スイッチ探知後の逆回転）の最小速度
#define PLUS_DIRECTION            0x01
#define MINUS_DIRECTION           0x55
#define CW                        0x01
#define CCW                       0x00


/*********************************************
 * I2c_input_cmd = internal_status　define
 *********************************************/
//NO RESERVED(USER DEFINE) -------------------

#define MOT_STOP     0x10
#define R2H          0x11
#define MOVE_INC     0x12
#define MOVE_ABS     0x13
#define MOVE_RCP     0x14

#define MOT_STOP_ERR 0x90
#define R2H_ERR      0x91
#define MOVE_INC_ERR 0x92
#define MOVE_ABS_ERR 0x93
#define MOVE_RCP_ERR 0x94
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

#define MOT_RELEASE  0x01
#define MOT_HOLD     0x55

/********************
 * L6470 parameter
 *********************/
#define START_ACC_VAL     0x320 //[R, WS] 加速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
#define START_DEC_VAL     0x300 //[R, WS] 減速度default 0x08A (12bit) (14.55*val+14.55[step/s^2])
#define START_MAX_SPD_VAL 0x3FF //[R, WR]最大速度default 0x041 (10bit) (15.25*val+15.25[step/s])
#define START_MIN_SPD     0x001 //[R, WS]最小速度default 0x000 (1+12bit) (0.238*val[step/s])
#define START_FSSPD       0x3FF //[R, WR]μステップからフルステップへの切替点速度default 0x027 (10bit) (15.25*val+7.63[step/s])
#define START_KVALHOLD    0x3F  //[R, WR]停止時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
#define START_KVALRUN     0x88  //[R, WR]定速回転時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
#define START_KCALACC     0x88  //[R, WR]加速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
#define START_KVALDEC     0x32  //[R, WR]減速時励磁電圧default 0x29 (8bit) (Vs[V]*val/256)
#define START_STEPMODE    0x06  //ステップモードdefault 0x07 (1+3+1+3bit) 1/64u
#define START_OCDTH       0xF   //[R, WR]過電流しきい値default 0x8 (4bit) (375*val+375[mA])
#define START_STALLTH     0x7F  //[R, WR]失速電流しきい値default 0x40 (7bit) (31.25*val+31.25[mA])
#define START_STATUS      0x2e88// 初期ステータス　
#define START_INTSPD      0x3E8 //[R, WH]逆起電力補償切替点速度default 0x0408 (14bit) (0.238*val[step/s])
#define START_STSLP       0x50  //[R, WH]逆起電力補償低速時勾配default 0x19 (8bit) (0.000015*val[% s/step])
#define START_FNSLPACC    0x28  //[R, WH]逆起電力補償高速時加速勾配default 0x29 (8bit) (0.000015*val[% s/step])
#define START_FNSLPDEC    0x00  //[R, WH]逆起電力補償高速時減速勾配default 0x29 (8bit) (0.000015*val[% s/step]) 


/****************************
 * arduino nano pin define
 ****************************/
//Sw or Sensor-----------------
#define ENDSTOP_1 PD2
#define DEBOUNCER_INTERVAL 5 //msec

//spi setting 
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SPI_SCK 13
#define PIN_SPI_SS 10
#define PIN_BUSY 9
#define OR_SW 17


/********************************
 * Gloval Constant  and Variable
 ********************************/

//グローバルは極力持つべきでないが、arduinoのハンドラは戻り値が持てないので、その代替
//Global variable for I2c 

//各種モード時のデータストレージ
  static byte          Hold_or_release      = 0;
  static byte          Target_direction     = 0;
  static byte          Target_speed         = 0;
  static unsigned int  Target_length        = 0;
  static byte          Target_position      = 0;
  static long          Target_startpos      = 0;
  static long          Target_endpos        = 0;
  static byte          Target_cycle_number  = 0;

  //I2c 受信　1stバイト
  static byte     I2c_rcv_cmd     = NO_CMD; //★ fordebug
  static byte     I2c_rcv_data[I2C_RCVDATA_LENGTH] = {};

  //I2c　リクエスト用
  static byte     I2c_rqst_data[I2C_RQSTDATA_LENGTH] = {}; 


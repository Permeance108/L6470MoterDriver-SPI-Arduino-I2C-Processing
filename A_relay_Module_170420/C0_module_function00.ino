/*****************************************
 * 
 * 入力コマンドの監視と内部モード実行判断
 * 
 *****************************************/
 
/*******************
 * 実行関数選定
 *******************/ 
  byte Module_act_selector(byte act_status){

   static byte input_cmd = NO_CMD;
   static byte next_act_mode = START_UP;
   static byte moduleAct_cmd = NO_CMD,moduleAct_stat = START_UP;
   
   static const byte moduleStatus_table[4][4]={
    {RCV_WAIT,  IN_ACT,  IN_ERR,  RCV_WAIT},
    {RCV_WAIT,  IN_ACT,  IN_ERR,  RCV_WAIT},
    {IN_ACT,    IN_ERR,  IN_ERR,  RCV_WAIT},
    {IN_ERR,    IN_ERR,  IN_ERR,  START_UP}
    };

    input_cmd = I2c_rcv_cmd; //入力の更新

    //入力コマンドの圧縮
    if (input_cmd <  NORMAL_START)                                { moduleAct_cmd = input_cmd;    }
    if((input_cmd >= NORMAL_START)  && (input_cmd <= NORMAL_END)) { moduleAct_cmd = NORMAL_CMD;   }
    if((input_cmd >= ERR_START)     && (input_cmd <= ERR_END))    { moduleAct_cmd = ERR_CMD;      }
    if(input_cmd > ERR_END)                                       { moduleAct_cmd = RESET_CMD;    }
    
    //現在ステータスの圧縮
    if (act_status <  NORMAL_START)                               { moduleAct_stat = act_status;  } //START_UP or RCV_WAIT 
    if((act_status >= NORMAL_START) && (act_status <= NORMAL_END)){ moduleAct_stat = IN_ACT;      }
    if (act_status >= ERR_START)                                  { moduleAct_stat = IN_ERR;      }

    
    //次回実行モードの切り分け
      next_act_mode = moduleStatus_table[moduleAct_stat][moduleAct_cmd];

     //復元 ※過去状態との信号比較。状態に変化があったら信号取り込み。
          if((next_act_mode == IN_ACT)&&(act_status <= RCV_WAIT)){
            next_act_mode = input_cmd;
            I2c_rcv_cmd = NO_CMD;
            I2c_rcv_data[BYTE_1ST] = NO_CMD;
      } 
     else if((next_act_mode == IN_ACT)&&((act_status >= NORMAL_START) && (act_status <= NORMAL_END))) { next_act_mode = act_status; }
    
    //if(next_act_mode == IN_ERR) { next_act_mode = act_status;} //★リセットはどうする？
    
    return next_act_mode;
}

/*****************************************
 * 
 * 実行アクションの割り付け
 *
 *****************************************/
 
byte Module_action(byte act_mode_number){

  static byte act_status = START_UP;

    switch(act_mode_number){
      
      case START_UP:
        act_status = Rcv_wait();
      break;

      case RCV_WAIT:
        act_status = Rcv_wait();
      break;
    
      case HEATER_CNT:
        act_status = Heater_cnt();
      break;

       case SOLENOID_CNT:
        act_status = Solenoid_cnt();
      break;     
    
      default:
        Serial.write("default "); /*★ for debug test */ 
       act_status = TEST_ERR;     /*★ for debug test */
      break;      
    }
  return act_status;
}

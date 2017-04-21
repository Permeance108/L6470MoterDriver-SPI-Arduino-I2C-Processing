/*******************************
 * 
 * 最終的な実行関数
 * 
 * ユーザー側で自由に書く。
 * どれも形としては同じにし、
 * 内部の実行段階を　byte  execOrder_flag  で管理して
 * switch case文で分岐する。
 * 
 * 戻り値は大まかに
 * モード実行中(それぞれのコマンドナンバーにイコール対応させる）
 * モード正常終了（RCV_WAIT)
 * 異常（対応異常ナンバー）
 * の３種。
 * 
 ************************/

byte Rcv_wait()
{
  static byte act_status;

  //clean parameter
    Heater_onoff    = 0;
    Solenoid_onoff  = 0;
    Timecounter     = 0; 
    I2c_rcv_cmd     = NO_CMD;
    act_status      = RCV_WAIT;
    
  return act_status;
}

byte Heater_cnt()
{
  static  byte  heater_onoff = 0;
  static  byte  act_status = HEATER_CNT; 
  static  byte  execOrder_flag = 0;
  static  int   timer_tmp = 0,timer_tmp_old = 0;  

        switch (execOrder_flag){
        
          case 0:
                heater_onoff = Heater_onoff;
                if(heater_onoff == RELAY_ON){
                    digitalWrite(AC100_N, LOW);
                }
                else if(heater_onoff == RELAY_OFF){
                    digitalWrite(AC100_N, HIGH);   
                }
             timer_tmp_old = Timecounter;   
             execOrder_flag = 1;
             act_status = HEATER_CNT;
          
          break;

          case 1:
                timer_tmp = Timecounter;
                if((abs(timer_tmp - timer_tmp_old)) >= RELAY_DELAY_TIME){
                    if(heater_onoff == RELAY_ON){
                      digitalWrite(AC100_L, LOW);
                    }
                    else if(heater_onoff == RELAY_OFF){
                      digitalWrite(AC100_L, HIGH);
                    }
                    execOrder_flag = 0;
                    act_status  = RCV_WAIT;                    
                }
                else{
                    execOrder_flag = 1;
                    act_status  = HEATER_CNT;  
                }
          break;

          default:
            act_status = TEST_ERR;    //★改変中
          break;
        }        
  return act_status;
}

byte Solenoid_cnt()
{
  static  byte  sorenoid_onoff = 0;
  static  byte  act_status = SOLENOID_CNT; 
  static  byte  execOrder_flag = 0;
  static  int timer_tmp = 0,timer_tmp_old = 0, timer_tmp_debug = 0;  

        switch (execOrder_flag){
        
          case 0:
                sorenoid_onoff = Solenoid_onoff;
                if(sorenoid_onoff == RELAY_ON){
                    digitalWrite(DC24_G, LOW);
                }
                else if(sorenoid_onoff == RELAY_OFF){
                    digitalWrite(DC24_G, HIGH);   
                }
             timer_tmp_old = Timecounter;   
             execOrder_flag = 1;
             act_status = SOLENOID_CNT;
          break;

          case 1:
                timer_tmp = Timecounter;                                      
                if(abs(timer_tmp - timer_tmp_old) >= RELAY_DELAY_TIME){
                    
                    if(sorenoid_onoff == RELAY_ON){
                      digitalWrite(DC24_H, LOW);
                    }
                    else if(sorenoid_onoff == RELAY_OFF){
                      digitalWrite(DC24_H, HIGH);
                    }
                    execOrder_flag = 0;
                    act_status = RCV_WAIT;  
                }
                else{
                   execOrder_flag = 1;
                   act_status = SOLENOID_CNT;
                }
          break;

          default:
            act_status = TEST_ERR;    //★改変中
          break;
        }        
  return act_status;
}


//store status to i2c_rqst--------------------------------------★仮
void Store_now_status_to_i2c(byte act_status){

  I2c_rqst_data[BYTE_1ST] = act_status;
  I2c_rqst_data[BYTE_2ND] = glass_sw.read();
  
}

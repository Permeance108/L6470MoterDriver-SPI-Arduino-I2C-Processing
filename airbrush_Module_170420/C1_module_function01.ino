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

  //clean i2c strage
    Hold_or_release      = 0;
    Target_direction     = 0;
    Target_speed         = 0;
    Target_length        = 0;
    Target_position      = 0;
    Target_startpos      = 0;
    Target_endpos        = 0;
    Target_cycle_number  = 0;
    I2c_rcv_cmd          = NO_CMD;
    act_status           = RCV_WAIT;
    
  return act_status;
}

byte Mot_stop()
{
  static  boolean first_flag = false;
  static  byte  act_status = MOT_STOP; 
  static  byte  execOrder_flag = 0;  

        switch (execOrder_flag){
          
          case 0: 
              if(Hold_or_release == MOT_RELEASE){
                      L6470_softhiz();
                  }
                  else if(Hold_or_release == MOT_HOLD){
                      L6470_softstop();
                  }             

             execOrder_flag = 1;
             act_status = MOT_STOP;
          break;

          case 1:
             execOrder_flag = 0;
             act_status  = RCV_WAIT;
          break;

          default:
            act_status = TEST_ERR;    //★改変中
          break;
        } 
        
return act_status;
}

byte Ret2home(byte target_direction ,byte target_speed)
{
  static byte act_status = R2H;  
  static byte execOrder_flag;
  static byte dia = 0, dia_inv = 0;     
  static int  testspeed = 0;       //★

  switch (execOrder_flag){
          /*原点復帰方向決め＆原点復帰速度決定*/
          case 0:
              if(target_direction == PLUS_DIRECTION){
                  dia = CW;
                  dia_inv = CCW;
               }
               else if(target_direction == MINUS_DIRECTION){
                  dia = CCW;
                  dia_inv = CW;
               }

               if(origin_sw.read() == LOW){
                  execOrder_flag = 2;
                  act_status = R2H;
                }
                else{
                  testspeed =  COEF_SPDBYTE_CNVT_PPS * int(target_speed);
                  
                  if(testspeed <= (COEF_SPDBYTE_CNVT_PPS * R2H_MINSPEED)){
                      testspeed = COEF_SPDBYTE_CNVT_PPS * R2H_MINSPEED;
                   }
                  
                  L6470_run(dia,testspeed);
                  execOrder_flag = 1;
                  act_status = R2H;
                }
          break;
          
          /*origin limit sw on wait*/
          case 1:          
                if(origin_sw.read() == LOW){ 
                    L6470_softstop();
                    execOrder_flag = 2;
                    act_status = R2H;
                }
                else{
                    //★本来ならばここでタイマー回して、原点復帰エラーを作る
                    execOrder_flag = 1;
                    act_status = R2H;
                }
          break;
          
          /* reverse */
          case 2:
              testspeed = COEF_SPDBYTE_CNVT_PPS * R2H_MINSPEED;
              L6470_run(dia_inv,testspeed);
              execOrder_flag = 3;
              act_status = R2H;          
          break;

          /*waiting for origin sw off*/
          case 3: 
               if(origin_sw.read() == HIGH){
                  L6470_softstop();
                  L6470_resetpos();
                  execOrder_flag = 0;
                  act_status = RCV_WAIT;
                }
                else{
                    //★本来ならばここでタイマー回して、原点復帰エラーを作る
                  execOrder_flag = 3;
                  act_status = R2H;
                }
          break;

          default:
             act_status = TEST_ERR; //★　デバッグ中 
          break;
        }
  return act_status;
}

byte Move_inc(byte target_speed, byte target_direction, int target_length)
{
  static  byte  act_status = MOVE_INC;
  static  byte  execOrder_flag = 0;  
  static  long  moveInc_distance = 0; /* unit:pulse */ 
  static  int   moveInc_speed = 0;    /* unit:step/sec */
  static  byte  moveInc_dia = 0;      /* unit: cw or ccw */
        
        switch (execOrder_flag){
          
          case 0:            
              moveInc_speed = COEF_SPDBYTE_CNVT_MAXPPS * int(target_speed);
              L6470_setparam_maxspeed(moveInc_speed);
                  
                  if( target_direction == PLUS_DIRECTION){
                    moveInc_dia = CW; 
                  }
                  else if( target_direction == MINUS_DIRECTION){
                    moveInc_dia = CCW;
                  } 
           
              moveInc_distance  = COEF_MM_CNVT_STEP * long(target_length);
              L6470_move(moveInc_dia,moveInc_distance);
    
              execOrder_flag = 1;
              act_status = MOVE_INC;
            break;

          case 1:   
              execOrder_flag = 0;
              act_status = RCV_WAIT;
            break;
          
          default:
              act_status = TEST_ERR; //★　デバッグ中 
            break;
        }
  return act_status;
}

byte Move_abs(byte target_speed, byte target_position)
{
  static  byte  act_status = MOVE_ABS;
  static  byte  execOrder_flag = 0; 
  static  int   moveAbs_speed = 0;
  static  long  moveAbs_position = 0;

        switch (execOrder_flag){

          case 0:
              moveAbs_speed = COEF_SPDBYTE_CNVT_MAXPPS * target_speed;
              L6470_setparam_maxspeed(moveAbs_speed);
              execOrder_flag = 1;
              act_status = MOVE_ABS;    
          break;

          case 1: 
              moveAbs_position = COEF_MM_CNVT_STEP * target_position;
              L6470_goto(moveAbs_position);
              execOrder_flag = 2;
              act_status = MOVE_ABS;        
          break;
          
          case 2:
              execOrder_flag  = 0;
              act_status     = RCV_WAIT;
          break;

          default:
              act_status = TEST_ERR; //★　デバッグ中 
          break;
        }
  return act_status;
}

byte Move_rcp(byte cycle_number, byte target_speed, int target_startpos, int target_endpos)
{
    static  byte act_status = MOVE_RCP;
    static  byte execOrder_flag = 0;
    static  byte cycle_tmp = 0; 
    static  int  moveRcp_speed = 0;
    static  byte dia = CW,dia_inv = CCW;
    static  long moveRcp_startpos = 0;
    static  long moveRcp_endpos = 0;
    static  long tmppos = 0;
    
        switch (execOrder_flag){
          /* speed set & move start point */
          case 0:   
              cycle_tmp = cycle_number;
              moveRcp_speed = COEF_SPDBYTE_CNVT_MAXPPS * target_speed;
              L6470_setparam_maxspeed(moveRcp_speed);
              moveRcp_startpos = long(COEF_MM_CNVT_STEP * Target_startpos);
              tmppos = L6470_getparam_abspos();              
                
                if(tmppos == moveRcp_startpos){
                      execOrder_flag = 3;
                 }
                 else{
                      execOrder_flag = 1;
                 }
              act_status = MOVE_RCP;
          break;

          case 1:
              moveRcp_startpos = long(COEF_MM_CNVT_STEP * Target_startpos);              
              L6470_goto(moveRcp_startpos); 
              execOrder_flag = 2;
              act_status = MOVE_RCP;
          break;

          /*ビジ―チェック&回転方向指定*/
          case 2:
               if(digitalRead(PIN_BUSY) == LOW){/*★本来はここでタイマカウントを回してエラー処理*/
                  execOrder_flag = 2;                   
               }
               else if(digitalRead(PIN_BUSY) == HIGH){
                  moveRcp_endpos = long(COEF_MM_CNVT_STEP * Target_endpos);
                  tmppos = L6470_getparam_abspos();
                        
                  if(tmppos == moveRcp_endpos){
                    execOrder_flag = 5;
                  }
                  else{
                    execOrder_flag = 3;
                  }
               }
              act_status = MOVE_RCP;
          break;

          /*ビジー抜け、終了位置へ移動*/
          case 3:
                moveRcp_endpos = long(COEF_MM_CNVT_STEP * Target_endpos);
                L6470_goto(moveRcp_endpos);
                execOrder_flag = 4;
                act_status = MOVE_RCP;
          break;

          /*ビジ―チェック&回転方向指定*/
          case 4:
                 if(digitalRead(PIN_BUSY) == LOW){/*★need timer error */
                    execOrder_flag = 4;
                 }
                 else if(digitalRead(PIN_BUSY) == HIGH){
                    moveRcp_startpos = long(COEF_MM_CNVT_STEP * Target_startpos);
                    tmppos = L6470_getparam_abspos();
     
                      if(tmppos == moveRcp_startpos){
                        execOrder_flag = 7;
                      }
                      else{
                        execOrder_flag = 5;
                      }
                 }
                act_status = MOVE_RCP;
          break;

          /*return start point*/
          case 5: 
              moveRcp_startpos = long(COEF_MM_CNVT_STEP * Target_startpos);
              L6470_goto(moveRcp_startpos);
              execOrder_flag = 6;
              act_status = MOVE_RCP;
          break;

          case 6:
              if(digitalRead(PIN_BUSY) == LOW){/* ★need timer error */ 
                  execOrder_flag = 6;
               }
               else if(digitalRead(PIN_BUSY) == HIGH){
                  execOrder_flag = 7;
               }
              act_status = MOVE_RCP;
          break;

          case 7:
              cycle_tmp--;

                   if(cycle_tmp > 0){ execOrder_flag = 1;}
              else if(cycle_tmp <= 0){ execOrder_flag = 8; }
              
              act_status = MOVE_RCP;
          break;

          /* terminater */
          case 8:
            execOrder_flag = 0;
            act_status = RCV_WAIT;
          break;
        }
  return act_status;
}

//store status to i2c_rqst--------------------------------------★仮
void Store_now_status_to_i2c(byte act_status){

  I2c_rqst_data[BYTE_1ST] = act_status;
  I2c_rqst_data[BYTE_2ND] = 
  
}

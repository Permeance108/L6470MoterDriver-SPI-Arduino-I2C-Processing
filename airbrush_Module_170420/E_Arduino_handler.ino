/**********************************
 * 
 * Arduino handler
 * 
 ***********************************/

/* I2c signal receive interrupt */
void I2cRcv_interrupt(int data_length)
{
    if((data_length % I2C_RCVDATA_LENGTH)==0){
        for(byte i=0; i < I2C_RCVDATA_LENGTH; i++ ){
          I2c_rcv_data[i] = Wire.read();
        }
    }
    else{/* ★need error */
    }
   
    /* data distribution */
    I2c_rcv_cmd = I2c_rcv_data[BYTE_1ST];
    
      if((I2c_rcv_data[BYTE_1ST]) == MOT_STOP){ 
        Hold_or_release = I2c_rcv_data[BYTE_2ND];
      }
      else if((I2c_rcv_data[BYTE_1ST]) == R2H){   
        Target_direction  = I2c_rcv_data[BYTE_2ND];
        Target_speed      = I2c_rcv_data[BYTE_3RD];           
      }
      else if(I2c_rcv_data[BYTE_1ST] == MOVE_INC){        
        Target_direction  = I2c_rcv_data[BYTE_2ND];
        Target_speed      = I2c_rcv_data[BYTE_3RD];
        Target_length     = (I2c_rcv_data[BYTE_4TH] << 8) + I2c_rcv_data[BYTE_5TH];          
      }
      else if(I2c_rcv_data[BYTE_1ST] == MOVE_ABS){ 
        Target_speed      = I2c_rcv_data[BYTE_3RD];
        Target_position   = (I2c_rcv_data[BYTE_4TH] << 8) + I2c_rcv_data[BYTE_5TH];          
      }
      else if(I2c_rcv_data[BYTE_1ST] == MOVE_RCP){
        Target_cycle_number= I2c_rcv_data[BYTE_2ND];
        Target_speed       = I2c_rcv_data[BYTE_3RD] * ;
        Target_startpos    = (I2c_rcv_data[BYTE_4TH] << 8) + I2c_rcv_data[BYTE_5TH];
        Target_endpos      = (I2c_rcv_data[BYTE_6TH] << 8) + I2c_rcv_data[BYTE_7TH];  
      }
  return;  
}

void I2cRqst_interrupt()
{
    for(byte i=0; i<I2C_RQSTDATA_LENGTH; i++ ){
        Wire.write(I2c_rqst_data[i]);
    }
  return;
}

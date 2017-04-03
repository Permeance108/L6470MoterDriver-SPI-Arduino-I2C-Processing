import processing.io.I2C;

class CH340 extends I2C{
int address;
byte[] rv=new byte[2];

  CH340(String dev,int address) {
    super(dev);
    this.address=address;
  }
  
  void send3byte(int stByte, int ndByte, int rdByte){    
    beginTransmission(address);
    //if (sig!=999) {
      write(stByte);
      write(ndByte);
      write(rdByte);
    //}
    //delay(10);
    //rv=read(2);  
    endTransmission();
    //sig=0;
  }
  
}
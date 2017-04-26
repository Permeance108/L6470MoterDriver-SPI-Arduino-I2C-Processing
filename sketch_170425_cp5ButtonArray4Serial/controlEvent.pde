void systemPlay(int val) {
  if (flag) {
    println("a button event from buttonB: "+val);
  }
}
void axisSelect(int val) {
  if (flag) {
    println("axisiSelct: "+val);
  }
}
void electPlay(int val) {
  if (flag) {
    println("axisiSelct: "+val);
  }
}

void controlEvent(ControlEvent theEvent) {
  if (flag && theEvent.isController()) {
    float valS=theEvent.getController().getValue();
    println("event from controller : "+ valS +" from "+theEvent.getController());
    ser.write(byte(valS));
  } else if (flag && theEvent.isGroup()) {
    println("event from group : "+theEvent.getGroup().getValue()+" from "+theEvent.getGroup());
  }
  delay(100);
  if (ser.available()>= 2) {
    val_low=ser.read();
    val_high=ser.last();
    valR=(val_high<<8)+val_low;
    println("recieve :" + valR);
  }
}
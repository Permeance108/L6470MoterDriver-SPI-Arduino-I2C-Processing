char data[30];
int i=0;
#define TCST 17
int anSig=0;

void setup() {
  Serial.begin(9600);
  pinMode(TCST, INPUT);
  Serial.println("Initialized \n");
}

void loop() {
  if(Serial.available()){
    int i=Serial.read();
    if(i==1){
      anSig=analogRead(TCST);
      Serial.write(anSig);
      Serial.println(anSig);
    }
  }
//  if(Serial.available()){
//    data[i]=Serial.read();
//    if(i>30||data[i]=='\0'){
//      data[i]='s';
//      data[i+1]='\0';
//      Serial.write(data);
//      i=0;
//    }
//    else{i++;}
//  }else{
//    anSig=analogRead(TCST);
//    Serial.write(lowByte(anSig));
//    Serial.write(highByte(anSig));
//    Serial.println(anSig);
//    delay(20);
//  }
}

 #include <SoftwareSerial.h> 
 SoftwareSerial RCV(7, 8);
 
 String msg = "";
 
 #define W_CLK 12       // Pin 8 - connect to AD9850 module word load clock pin (CLK)
 #define FQ_UD 11       // Pin 9 - connect to freq update pin (FQ)
 #define DATA 10       // Pin 10 - connect to serial data load pin (DATA)
 #define RESET 9      // Pin 11 - connect to reset pin (RST).
 
 #define pulseHigh(pin) {digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }
 
 int FgenPin = 13;
 int WaveSwitch = 2;
 
 // transfers a byte, a bit at a time, LSB first to the 9850 via serial DATA line
void tfr_byte(byte data)
{
  for (int i=0; i<8; i++, data>>=1) {
    digitalWrite(DATA, data & 0x01);
    pulseHigh(W_CLK);   //after each bit sent, CLK is pulsed high
  }
}

 // frequency calc from datasheet page 8 = <sys clock> * <frequency tuning word>/2^32
void sendFrequency(double frequency) {
  int32_t freq = frequency * 4294967295/125000000;  // note 125 MHz clock on 9850
  for (int b=0; b<4; b++, freq>>=8) {
    tfr_byte(freq & 0xFF);
  }
  tfr_byte(0x000);   // Final control byte, all 0 for 9850 chip
  pulseHigh(FQ_UD);  // Done!  Should see output
}

void setup() {
 // configure arduino data pins for output
  RCV.begin(115200);
  Serial.begin(115200);
  
  pinMode(FQ_UD, OUTPUT);
  pinMode(W_CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(FgenPin, OUTPUT);
  pinMode(WaveSwitch, OUTPUT);
   
  pulseHigh(RESET);
  pulseHigh(W_CLK);
  pulseHigh(FQ_UD);  // this pulse enables serial mode - Datasheet page 12 figure 10
  
}

void loop() {
  
  //digitalWrite(FgenPin, 1);
  delay(1000);
  sendFrequency(100);  // freq
  
  while(1){ 
    
  if(RCV.available()){
    
    while(RCV.available() > 0){
    
    char incoming_char = RCV.read();
    
    Serial.println(incoming_char);
    
    //Serial.println("\r");
    
    msg = msg + incoming_char;
 
    }
    
    Serial.println(msg);
    String wave = msg.substring(0, 3);
    String freq = msg.substring(3);
    
    Serial.println(wave);
    Serial.println(freq);
    
    if(wave == "SIN"){
      
      digitalWrite(WaveSwitch, 1);
    
    }else if(wave == "SQU"){
      
      digitalWrite(WaveSwitch, 0);
    
    }
    
    double new_freq = freq.toInt();
    
    Serial.println(new_freq);
    
    sendFrequency(new_freq);
    
    msg = "";
    
  }
  
}
}



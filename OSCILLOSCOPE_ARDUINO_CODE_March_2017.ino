#include <SoftwareSerial.h>

SoftwareSerial SND(7, 8);

void setup() {
  
  //Serial.begin(115200);
  SND.begin(115200);
}



void loop() {
  
  int ChannelA = analogRead(A0);
  int ChannelB = analogRead(A4);
  int ReferenceValue = analogRead(A5);
  // print out the value you read:
  SND.print("A");
  SND.print(ChannelA);
  //Serial.println("\r");
  
  SND.print("B");
  SND.print(ChannelB);
  //Serial.println("\r");
  
  SND.print("C");
  SND.println(ReferenceValue);
  
  delay(1);        // delay in between reads for stability
}

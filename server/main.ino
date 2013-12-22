/**
 * ControlMatrixviaMirf
 *
 * Mirf pins:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 * CE -> 8
 * CSN -> 7
 *
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

String tweetBuffer = "Hello hi, good evening. Eating time. Ready the dinging table and television.";
  
void setup(){
  Serial.begin(9600);

  // Mirf.payload = 16;  
  // Mirf.channel = 10;

  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"serv1");    
  Mirf.config();
  
  Serial.println("Beginning..."); 
}

void loop(){
  byte dataBuffer[16] = {0};  
  byte packetsRequired = (tweetBuffer.length() - 1) / 14;
  
  for(int i=0; i <= packetsRequired; i++) {
    dataBuffer[0] = i; //packet address
    
    for(int j=0; j<14; j++) {
    dataBuffer[j+1] = tweetBuffer[(14*i) + j];
    }
    
    if (i == 0) {
      dataBuffer[15] = 0x00; //first packet
    } else if(i != packetsRequired) {
      dataBuffer[15] = 0x55; //data packet
    } else if (i == packetsRequired) {
      dataBuffer[15] = 0xAA; //last packet   
    }
      
    Mirf.setTADDR((byte *)"clie1");
    Mirf.send(dataBuffer);
    while(Mirf.isSending());
    Serial.print("Sent packet: ");
    Serial.println(i);
    delay(1000);
  }
  
  Serial.println("Finished sending");

  while(1);    
} 

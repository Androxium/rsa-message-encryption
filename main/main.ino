#include <RF24.h>
#include <SPI.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "commonRSA.h"
#include "printf.h"
#include "allThePrimes.h"
byte  read_addrGeneral[6] = "00100";
byte  read_private[6];
byte  write_addr[6];
long k1 = -1;
long k2 = -2;
RF24 radio = RF24(7,8); 
int tx_flag = 1;
int handshake_complete =0;
int rx_stage = 0;
RSA enc=RSA(13,11);
char* byteToString(byte* in){
  char* out = ((char*)malloc(7*sizeof(char)));
  for(int i = 0; i<6; i++){
    out[i] = ((int)in[i])+'0';
    //Serial.print(out[i]);
  }
  out[6] = '\0';
  //Serial.println(out);
  return out;
}
void setup() {
  Serial.begin(9600);
  printf_begin();
  radio.begin();
  randomSeed(analogRead(0));
  radio.powerUp();
  for(int i = 0; i<6; i++){
    read_private[i] = random(0,9);
  }
  radio.openReadingPipe(1,read_addrGeneral);
  radio.openReadingPipe(2,read_private);
  radio.openWritingPipe(read_addrGeneral);
  char* addrStr = byteToString(read_private);
  Serial.print("Configuration of radio: \n Addr:");
  Serial.print(addrStr);
  Serial.print("\nPublic Key: (");
  Serial.print(enc.getPubKey().l1);
  Serial.print(",");
  Serial.print(enc.getPubKey().l2);
  Serial.print("\n");
  Serial.print("Private Key: (");
  Serial.print(enc.getPrivKey().l1);
  Serial.print(",");
  Serial.print(enc.getPrivKey().l2);
  Serial.print("\n");  //Serial.print(")");
  //free(addrStr);
  addrStr = NULL;
  radio.setAutoAck(1,false);
  radio.setAutoAck(0,false);
  radio.printDetails();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(handshake_complete == 0){
    Serial.print("Attempting to send private address to ");
    Serial.write((read_addrGeneral),6*sizeof(char));
    k1 = enc.getPubKey().l2;
    k2 = enc.getPubKey().l1;
    Serial.println();
    radio.stopListening();
    radio.write(k1,sizeof(long));
    radio.write(k2,sizeof(long));
    radio.write(read_private,sizeof(read_private));
    radio.startListening();
    if(radio.available()){
      radio.read(&k1,sizeof(long));
      radio.read(&k2,sizeof(long));
      radio.read(&write_addr,sizeof(write_addr));
      Serial.println("Handshake complete");
      handshake_complete = 1;
    }
    //Serial.println("Attempting handshake");
    }
  // Serial.println("Attempting handshake");
  else{
    radio.startListening();
    if(radio.available()){
      //Serial.print("There is stuff");
      int input = 0;
      radio.read(&input,sizeof(int));
      int plainText = enc.rsa(input,enc.getPrivKey().l2,enc.getPrivKey().l1);
      //Serial.print("\n");
      //Serial.print(input);
      char letter = ((char)(plainText+'0'));
      Serial.print(letter);
    }
    if(Serial.available()){
      radio.stopListening();
      int a = Serial.read()-'0';
      int c = enc.rsa(a,enc.getPubKey().l2,enc.getPubKey().l1);
      Serial.print("Sendign some message\n");
      radio.write(&c,sizeof(int));
      Serial.print(c);
      Serial.print("\n");
    }
  } 
}

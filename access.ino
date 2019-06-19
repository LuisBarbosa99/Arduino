/*Pino SDA ligado na porta 10 do Arduino
Pino SCK ligado na porta 13 do Arduino
Pino MOSI ligado na porta 11 do Arduino
Pino MISO ligado na porta 12 do Arduino
Pino NC – Não conectado
Pino GND  ligado no pino GND do Arduino
Pino RST ligado na porta 9 do Arduino
Pino 3.3 – ligado ao pino 3.3 V do Arduino
*/
#include <SPI.h>
#include <RFID.h>
#include <Time.h> //biblioteca para hora
#include <TimeLib.h> //biblioteca para hora
#define SS_PIN 10
#define RST_PIN 9
const int vermelho  = 6; //vermelho: a entrada esta bloqueada para este usuário
const int amarelo = 7; //amarelo: em espera 
const int verde = 8; //verde : entrada liberada para o usuario
int estado;
int estadoLed = 0;
unsigned long padrao = 1560955800 ; 
RFID RC522(SS_PIN, RST_PIN);
String conteudo;
void setup(){
  pinMode (vermelho,OUTPUT);
  pinMode (amarelo,OUTPUT);
  pinMode (verde,OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  setTime(padrao);
  estado = 0;
  RC522.init();
  adjustTime(hour()-10800);
  Serial.println("aproxime essa porra de cartao nesse carai");
  Serial.println();
}


void loop(){
  switch(estado){
    case 0:
      horario();
      break;
    case 1:
      acesso();
      break;
    case 2:
      permitido();
      break;
    case 3:
      negado();
      break;
  }
}
void horario(){
    Serial.println(hour());
    if (hour()>=8 && hour()<12 || hour()>=14 && hour()<18){
        
        estadoLed = HIGH;
        digitalWrite(amarelo,estadoLed);
        estado = 1;
        Serial.println("aguardando leitura");
    }else{
        estadoLed = LOW;
        digitalWrite(amarelo,estadoLed);
    }
}
void acesso(){
  Serial.println("Acesso: em espera");
if (RC522.isCard()){
  
  conteudo = "";
  RC522.readCardSerial();
  Serial.println("Cartão selecionado");
  for(int i=0;i<5;i++){ 
      conteudo.concat(String(RC522.serNum[i],HEX));
      
    }
    conteudo.toUpperCase();
    Serial.println(conteudo);  
    estado  = 2;
}
delay (1000);

}
void permitido(){
  Serial.println("Esperando cartao");
if (conteudo == "B542147B1") 
  {
    Serial.println("Ola My !");
    Serial.println();
    digitalWrite(verde,HIGH);
    delay(3000);
    digitalWrite(verde,LOW);
    digitalWrite(amarelo,HIGH);
    delay(1000);
    estado =1;
    
  }else estado = 3;
  delay(500);
}

void negado(){
  Serial.println("Cartao negado");
  if (conteudo == "7D84573E9") 
  {
    Serial.println("Ola Cartao !");
    Serial.println();
    digitalWrite(vermelho,HIGH);
    delay(3000);
    digitalWrite(vermelho,LOW);
    digitalWrite(amarelo,HIGH);
    delay(1000);
    
  }
  estado = 1;
  delay(500);
}


/* Sala de Aula Inteligente 

    Funções:

    >> Reconhecimento de usuário por RFID;
    >> Display de notificações por LCD (Liquid Crystal);
    >> Se ativado, abrir porta, ligar ar condicionado e projetor (LEDs); 
    
    Porta e Reconhecimento de usuário:

    >> Funcionamento da sala em horário delimitado;
    >> Enquanto estiver no horário, o led amarelo é aceso;
    >> Se o usuário NÃO estiver cadastrado, a sala não abre e o led vermelho é aceso;
    >> Se o usuário estiver cadastrado, a sala abre e o led verde é aceso;
    >> Reconhecimento de usuário por RFID e notificação no Display LCD;


   */ 
#include <LiquidCrystal.h>
#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int vermelho  = 6; //vermelho: a entrada esta bloqueada para este usuário
const int amarelo = 7; //amarelo: em espera 
const int verde = 8; //verde : entrada liberada para o usuario
const int accessButton = 9; //botão
int buttonState = 0; 
int status = turnOn;
int lastButtonState = 0;
long tempo = 0;
long debounce = 250;
int estadoLed = 0;
String conteudo = "";
enum {
  turnOn,
  standby,
  access,
  allow,
  deny
};



RFID RC522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();
  RC522.init();
  
  pinMode (vermelho,OUTPUT);
  pinMode (amarelo,OUTPUT);
  pinMode (verde,OUTPUT);
  pinMode (accessButton,INPUT);

}
void loop(){
  switch (status)
  {
    case turnOn:
      onOff();
      break;
    case standby:
      mensagemInicial();
      break;
    case access:
      acesso(); 
      break;
    case allow:
      permitido();
      break;
    case deny:
      negado();
      break;    
  }

}
void onOff(){
buttonState = digitalRead(accessButton);
if(buttonState == HIGH && lastButtonState == LOW && millis() - tempo > debounce) {
        		if(estadoLed == HIGH){
          		estadoLed = LOW;
                digitalWrite(amarelo,estadoLed);
                
        		} else {
            	estadoLed = HIGH;
                digitalWrite(amarelo,estadoLed);
                status = standby;
                }
    		tempo = millis();
      		}
        
      	
        lastButtonState == buttonState;
  delay(1000);
}
void mensagemInicial()
{
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Aproxime o seu");  
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor");  
  delay(1000);
  status = access;

}
void permitido(){
if (conteudo.substring(1) == "ED 78 03 CA") 
  {
    Serial.println("Ola Luis !");
    Serial.println();
    digitalWrite(verde,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Luis !");
    lcd.setCursor(0,1);
    lcd.print("Acesso liberado!");
    delay(3000);
    status = standby;
  }else status = deny;

}
void negado(){
  if (conteudo.substring(1) == "BD 9B 06 7D") 
  {
    Serial.println("Ola Cartao !");
    Serial.println();
    digitalWrite(vermelho,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola Cartao !");
    lcd.setCursor(0,1);
    lcd.print("Acesso Negado !");
    delay(3000);
    digitalWrite(vermelho,LOW);
    digitalWrite(amarelo,HIGH);
    delay(1000);
    status = standby;
  }
}
void acesso(){
  
if (RC522.isCard()){
  RC522.readCardSerial();
  Serial.println("Cartão selecionado");
  for(int i=0;i<5;i++){ 
      conteudo.concat(RC522.serNum[i],HEX);
      Serial.println(conteudo);
    }

  Serial.println();
  
}delay (1000);
status = allow;
}

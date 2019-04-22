#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int blocked  = 6; //vermelho: a entrada esta bloqueada para este usuário
const int azul = 7; //azul: em espera 
const int verde = 8; //verde : entrada liberada para o usuario
const int accessButton = 9; //botão
int buttonState = 0; 
int lastButtonState = 0;
long tempo = 0;
long debounce = 250;
int estadoLed = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode (blocked,OUTPUT);
  pinMode (azul,OUTPUT);
  pinMode (verde,OUTPUT);
  pinMode (accessButton,INPUT);
  digitalWrite(azul,HIGH); //standby

}
void loop(){
  acesso();
}
void acesso(){
Serial.println(digitalRead(azul));
buttonState = digitalRead(accessButton);
if(buttonState == HIGH && lastButtonState == LOW && millis() - tempo > debounce) {
        		if(estadoLed == HIGH){
          		estadoLed = LOW;
                digitalWrite(azul,!estadoLed);
                Serial.println(digitalRead(azul));   
                digitalWrite(verde,estadoLed);  
        		} else {
            	estadoLed = HIGH;
                digitalWrite(azul,!estadoLed);
                Serial.println(digitalRead(azul));
                digitalWrite(verde,estadoLed); 
        		}
    		tempo = millis();
      		}
        
      	//display();
        lastButtonState == buttonState;
  

 }
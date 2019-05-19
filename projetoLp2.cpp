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
#include <IRremote.h>
#include <Time.h>
#include <TimeLib.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 9
LiquidCrystal_I2C lcd(0x27,16,2);
const int azul = 4; //azul: ar condicionado
const int amarelo2 = 5; //amarelo2: referente ao projetor
const int vermelho  = 6; //vermelho: a entrada esta bloqueada para este usuário
const int amarelo = 7; //amarelo: em espera 
const int verde = 8; //verde : entrada liberada para o usuario
int estado;
int estadoLed = 0;
String conteudo;
int RECV_PIN = 3;
unsigned long padrao = 1558001422 ; // data e hora em 16/05/2019 10:10 UTC

enum {
  turnOn,
  standby,
  access,
  allow,
  deny,
  infraRed
};


IRrecv irrecv(RECV_PIN);  //cria o objeto receptor do IR
decode_results results;   //identifica a entrada de IR e guarda o valor em results
RFID RC522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  RC522.init();
  estado = turnOn;
  pinMode (vermelho,OUTPUT);
  pinMode (amarelo,OUTPUT);
  pinMode (verde,OUTPUT);
  pinMode (amarelo2, OUTPUT);
  pinMode (azul, OUTPUT);
  setTime(padrao);
  irrecv.enableIRIn();
  adjustTime(hour()-10800); //ajustando o fuso horário
}
void loop(){
  switch (estado)
  {
    case turnOn:
      horario();
      //onOff();
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
    case infraRed:
      infravermelho();
      break;
    case deny:
      negado();
      break;    
  }

}


void horario(){
    if (hour()>8 && hour()<12 || hour()>14 && hour()<18){
        estadoLed = HIGH;
        digitalWrite(amarelo,estadoLed);
        estado = standby;
        Serial.println("aguardando leitura");
    }else{
        estadoLed = LOW;
        digitalWrite(amarelo,estadoLed);
    }
}
 void mensagemInicial(){
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Aproxime o seu");  
  Serial.println("aproxime o seu cartao");
  lcd.setCursor(0,1);
  lcd.print("cartao do leitor");  
  delay(1000);
  estado = access;

} 
void permitido(){
if (conteudo.substring(1) == "8410552EEF") 
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
    //estado = infraRed;
    
  }else estado = deny;

}
void infravermelho(){     

String resultado;

  if (irrecv.decode(&results)){   
  
    Serial.print("Valor lido foi: ");
    Serial.println(results.value, HEX);
    resultado = (results.value, HEX);
    resultado.toUpperCase();  
    
    if(resultado == "FFA25D"){      //para ligar o ar condicionado (tecla 1)
      digitalWrite(amarelo2, HIGH);
      Serial.print("ar condicionado ligado");
    }
    
    if(resultado == "FF629D"){      //para desligar o ar condicionado (tecla 2)
      digitalWrite(amarelo2, LOW);
      Serial.print("ar condicionado desligado");
    }
    
    if(resultado == "FFE21D"){      //para aumentar a temperatura do ar condicionado (tecla 3)
      digitalWrite(amarelo2, HIGH);
      Serial.print("vc aumentou a temperatura");
    }
    
    if(resultado == "FF22DD"){      //para diminuir a temperatura ar condicionado (tecla 4)
      digitalWrite(amarelo2, HIGH);
      Serial.print("vc diminuiu a temperatura");
    }
    
    if(resultado == "FF02FD"){      //para ligar o projetor (tecla 5)
      digitalWrite(azul, HIGH);
      Serial.print("projetor ligado");
    }
   
    if(resultado == "FFC23D"){      //para desligar o projetor (tecla 6)
      digitalWrite(azul, LOW);
      Serial.print("projetor desligado");
    }
  
  }
  
}
void negado(){
  if (conteudo.substring(1) == "F483A2D50") 
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
    estado = standby;
  }
}
void acesso(){
if (RC522.isCard()){
  
  conteudo = "";
  RC522.readCardSerial();
  Serial.println("Cartão selecionado");
  for(int i=0;i<5;i++){ 
      conteudo.concat(String(RC522.serNum[i],HEX));
      
    }
    conteudo.toUpperCase();
    Serial.println(conteudo);  
  
estado = allow;
}delay (1000);

}
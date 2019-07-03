#include<Ultrasonic.h>

#define pino_echo 5
#define pino_trigger 4
#define vermelho 3
#define verde 2

float cmMsec;
int estado;
float nivel_caixa_baixo = 12.00; //ajustar valores de niveis de caixa d'água 
float nivel_caixa_minimo = 13.00; //ajustar valores de niveis de caixa d'água  
float nivel_caixa_ideal = 15.00; //ajustar valores de niveis de caixa d'água 

Ultrasonic ultrasonic(pino_trigger, pino_echo);

void setup() {    

  Serial.begin(9600);
  pinMode (vermelho,OUTPUT);
  pinMode (verde,OUTPUT);
  estado = 0;
}

void loop() {     

  cmMsec = ultrasonic.read(); //adicionado chamada de método ultrasonic.read()
  Serial.print("distancia em cm: ");
  Serial.println(cmMsec);
  delay(1500);

  switch(estado){
      case 0:
        nivel_baixo();
        break;
  
       case 1:
        nivel_minimo();
        break;
  
       case 2:
        nivel_ideal();
        break;
        
    }
}

void nivel_baixo(){

  if(cmMsec <= nivel_caixa_baixo){
    Serial.println("ALERTA! caixa com nivel abaixo do adequado"); 
    digitalWrite(3, HIGH);
    digitalWrite(2, LOW);
    delay(300);
    digitalWrite(3, LOW);
    }else estado = 1;
}

void nivel_minimo(){
  if(cmMsec <= nivel_caixa_minimo){
  Serial.println("caixa com nivel minimo");
  digitalWrite(3, HIGH);
  digitalWrite(2, LOW);
  delay(300);
  }else estado = 2;
}

void nivel_ideal(){
  if(cmMsec >= nivel_caixa_ideal){
  Serial.println("nivel de agua otimo!");
  digitalWrite(3, LOW);
  digitalWrite(2, HIGH);
  delay(300);
  }else estado = 0;
}

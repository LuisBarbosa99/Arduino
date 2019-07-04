#include <Ultrasonic.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int pino_rele = 5;
const int pino_echo = 1;
const int pino_trigger = 2;
const int vermelho = 4;
const int verde = 3;

const char* ssid = "moto g(6) play"; 
const char* password =  "Mortadela1";
const char* mqttServer = "postman.cloudmqtt.com"; 
const int mqttPort =  13222; 
const char* mqttUser = "onyxigle"; 
const char* mqttPassword = "Rzn4QDqQX6q6"; 
float cmMsec;
int estado;
float nivel_caixa_baixo = 12.00;
float nivel_caixa_minimo = 13.00;
float nivel_caixa_ideal = 16.00;

Ultrasonic ultrasonic(pino_trigger, pino_echo);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {    

  Serial.begin(115200);
  pinMode(pino_rele, OUTPUT);
  pinMode (vermelho,OUTPUT);
  pinMode (verde,OUTPUT);
  WiFi.begin(ssid, password);
  estado = 0;
  
  while (WiFi.status() != WL_CONNECTED) 
  {   
    delay(100);
    Serial.println("Conectando a WiFi..");
  }
  Serial.println("Conectado!"); 
  client.setServer(mqttServer, mqttPort);
  
 
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    
    if (client.connect("Projeto", mqttUser, mqttPassword ))
    {
 
      Serial.println("Conectado ao servidor MQTT!");  
 
    } else {
 
      Serial.print("Falha ao conectar ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("Status ","Reiniciado!");
  client.publish("Placa","Em funcionamento!");
}

void loop() {     

  cmMsec = ultrasonic.read(); 
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

void sole(){  //quando o nível da agua não for adequado abre até que ele fique

  digitalWrite(pino_rele, HIGH);
  if(cmMsec == 15.00){
    digitalWrite(pino_rele, LOW);
    Serial.print("Caixa com nível correto");
  }
  
}

void nivel_baixo(){   //função verifica se o nível da caixa está abaixo do ideal

  if(cmMsec <= nivel_caixa_baixo){
    Serial.println("ALERTA! caixa com nivel abaixo do adequado"); 
    digitalWrite(vermelho, HIGH);
    sole(nivel_caixa_baixo);
    digitalWrite(verde, LOW);
    delay(300);
    digitalWrite(vermelho, LOW);
    }else estado = 1;
}

void nivel_minimo(){
  if(cmMsec <= nivel_caixa_minimo){
    
    Serial.println("caixa com nivel minimo");
    digitalWrite(vermelho, HIGH);
    sole(nivel_caixa_minimo);
    digitalWrite(verde, LOW);
    delay(300);
    digitalWrite(pino_rele, HIGH);
    delay(5000);
    }else estado = 2;
}

void nivel_ideal(){
  if(cmMsec >= nivel_caixa_ideal){
  Serial.println("nivel de agua otimo!");
  digitalWrite(vermelho, LOW);
  digitalWrite(verde, HIGH);
  delay(300);
  }else estado = 0;
}
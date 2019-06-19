/*
 Ligar ar condicionado e projetor 
 */


#include <IRremote.h>


int state;
int estadoAr;
int estadoPrj;
int khz = 38; // frequencia da tag enviada pelo emissor

IRsend irsend;

  // NEC 20640000 desligaAr
  unsigned int desligaAr[]  = {8900,4400, 600,600, 550,550, 600,1650, 600,500, 600,550, 600,550, 600,550, 650,500, 650,450, 700,1550, 600,1700, 550,500, 600,550, 700,1550, 600,500, 600,550, 600,550, 700,450, 700,450, 650,450, 700,450, 700,450, 700,450, 700,450, 650,450, 700,450, 700,450, 700,450, 700,450, 650,500, 650,450, 700,450, 700,450, 700,450, 650,500, 650,450, 700,450, 700,450, 700,450, 700,450, 650,450, 700,450, 700,1600, 550,1700, 550};  
  // NEC 28640000 ligaAr
  unsigned int ligaAr[] = {8900,4400, 600,550, 650,500, 650,1650, 600,450, 650,1650, 650,450, 650,500, 650,500, 600,500, 650,1650, 600,1600, 600,500, 650,500, 650,1650, 600,450, 650,500, 650,500, 650,500, 650,500, 650,450, 650,500, 650,500, 650,500, 650,500, 650,450, 650,500, 650,500, 650,500, 650,500, 650,500, 650,450, 650,500, 650,500, 650,500, 650,500, 650,450, 650,500, 650,500, 650,500, 650,500, 650,1600, 600,500, 650,1650, 600,1600, 600};
  // NEC C1AA09F6 projetor
  unsigned int prj[] = {8900,4400, 600,1700, 550,1650, 550,500, 600,550, 600,500, 600,500, 600,550, 600,1600, 600,1700, 550,500, 600,1650, 600,500, 600,1600, 600,550, 600,1600, 600,550, 600,500, 600,500, 600,500, 600,550, 600,1600, 600,550, 600,500, 600,1650, 600,1650, 550,1700, 550,1650, 550,1700, 550,500, 600,1650, 600,1650, 550,500, 600};
void setup()
{
   
  state = 1; //iniciar ligando o ar
  estadoAr = LOW;
  estadoPrj = LOW;
  
}
void ar(){
  if(estadoAr==LOW){
    irsend.sendRaw(ligaAr,sizeof(ligaAr)/sizeof(ligaAr[0]),khz);
    estadoAr == HIGH;
    state = 2 ;//altera estado para que ligue o projetor após ligar o ar condicionado
    delay(2000);
  }
}

void desligarProj(){ //função para desligar projetor
  if(estadoPrj==HIGH){
    for(int i = 0; i<2;i++){ 
      irsend.sendRaw(prj,sizeof(prj)/sizeof(prj[0]),khz);
      delay(2000);
      state = 4;
    }  
  estadoPrj = LOW;
  
  }
    
 }
void ligarProj(){
  if(estadoPrj==LOW){
    irsend.sendRaw(prj,sizeof(prj)/sizeof(prj[0]),khz); //envia codigo para ligar projetor 
    estadoPrj = HIGH;
    state = 3;
    delay(15000);     
  }
  
  delay(2000);
}
void arOff(){
  if(estadoAr==HIGH){
    irsend.sendRaw(desligaAr, sizeof(desligaAr) / sizeof(desligaAr[0]), khz); 
    estadoAr = LOW;
    delay(500);
  }
}
void loop() {
  switch(state){
    case 1:
      ar();
      break;
    case 2:
      ligarProj();
      break;
    case 3:
      desligarProj();
      break;
    case 4:
      arOff();
      break;
  }
  
}

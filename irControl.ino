/*
 Ligar ar condicionado e projetor 
 */


#include <IRremote.h>

int onOff;
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
    onOff = 0;
    state = 3;
    estadoAr = LOW;
    estadoPrj = LOW;
}
void ar(){
    
  if (estadoAr == HIGH){
  irsend.sendRaw(desligaAr, sizeof(desligaAr) / sizeof(desligaAr[0]), khz); 
  estadoAr = LOW;
  delay(500);
}else{
  irsend.sendRaw(ligaAr,sizeof(ligaAr)/sizeof(ligaAr[0]),khz);
  estadoAr == HIGH;
  delay(500);
}
}

void desligarProj(){
 
        for(int i = 0; i<3;i++){
            irsend.sendRaw(prj,sizeof(prj)/sizeof(prj[0]),khz);
            delay(500);
        }
    
    estadoPrj = LOW;
    
 }
void ligarProj(){
    
        irsend.sendRaw(prj,sizeof(prj)/sizeof(prj[0]),khz);
        estadoPrj = HIGH;
        delay(500);
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
  }
}

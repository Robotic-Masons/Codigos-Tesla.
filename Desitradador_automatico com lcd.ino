#include <LiquidCrystal.h>

LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);

#define potenciometro  A0
#define pinoRele 7 
#define pinoBotaoTempo  12
#define pinoBotaoPronto 13 

int tempo = 0;
int tempoEscolhido;
int potenciometroRead;
int tempLcd; 

bool buttonSetTemp = false;

bool comandoTempSetting = false;
bool comandoPower = false;

bool status = false;

void setup() {
  Serial.begin(9600);
  pinMode(pinoRele, OUTPUT); 
  pinMode(pinoBotaoTempo, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  pinMode(pinoBotaoPronto, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO

  digitalWrite(pinoRele, HIGH); //MÓDULO RELÉ INICIA DESLIGADO

  lcd.begin(16, 2);
}

void loop() {


  lcd.setCursor(2, 0);  
  if(status == false){
  int tempLcd = (tempoPonteciometro(potenciometroRead) * 3600);
  lcd.print("setTemp:" + tempLcd);
  }else if(status == true){
    char div = '/';
    char div1 = ' ';
    char div2 = ' ';
    lcd.print("Temp:" + tempoEscolhido + div1 + div + div2 + tempo);
  }

  lcd.setCursor(2, 1);
  lcd.print("Power: " + status);


  Serial.print("Power(1 = on, 2 = off): ");
  Serial.println(status);


  potenciometroRead = analogRead(potenciometro);
  setTemp(potenciometroRead);
  
  if(comandoTempSetting == false)
  {
    drawn(potenciometroRead, "Tempo de duração");
  }else if (comandoTempSetting == true)
  {
    drawn(tempoEscolhido, "Tempo de escolhido");
  }
  
  comandoPower = digitalRead(pinoBotaoPronto);

  if(comandoPower == true && comandoTempSetting == true){
    start(true);

  }
  if(tempo >= tempoEscolhido && status == true){
    start(false);
  }

}

void start(bool start){
  if(start == true){
  contarTempo(true);
  power(true);
  drawn(tempo, "Tempo decorrido");
  }else{
    contarTempo(false);
    power(false);
    zerarTempo();
  }
}

void contarTempo(bool comando){
 if(comando == true){
    tempo = tempo + 1;
    delay(1000);
 }else{
  drawn(tempo, "Tempo ACABOU");
 }
}

void zerarTempo(){
  tempo = 0;
  comandoTempSetting = false;
  status = false;
}

int  tempoPonteciometro(int value){
  return map(value, 0, 1023, 1, 24);
}

void setTemp(int potenciometroRead){
  buttonSetTemp = digitalRead(pinoBotaoTempo);

  if(buttonSetTemp == true){
    tempoEscolhido = (tempoPonteciometro(potenciometroRead) * 3600);
    comandoTempSetting = true;
  }
}

void power(bool comand){
  if(comand == true){
    digitalWrite(pinoRele, LOW); //LIGA O MÓDULO RELÉ (LÂMPADA ACENDE)
    status = true; 
  }else{
      digitalWrite(pinoRele, HIGH); //DESLIGA O MÓDULO RELÉ (LÂMPADA APAGA)
      status = false; 
  }

}

void drawn(int val, char name[25]){
  Serial.println(name + val);
}

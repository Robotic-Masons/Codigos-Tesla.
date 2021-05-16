int tempo = 0;
int tempoFinal;
const int potenci = A0;

const int pinoRele = 7; //PINO DIGITAL UTILIZADO PELO MÓDULO RELÉ SSR
const int pinoBotaoTemp = 12; //PINO DIGITAL UTILIZADO PELO BOTÃO
const int pinoBotaoPronto = 13; //PINO DIGITAL UTILIZADO PELO BOTÃO

int statusLamp = 0; //VARIÁVEL QUE CONTROLA O ESTADO DO MÓDULO RELÉ (LIGADO / DESLIGADO)

bool comandoPronto;
bool comandoTempo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinoRele, OUTPUT); //DEFINE O PINO COMO SAÍDA
  pinMode(pinoBotaoTemp, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  pinMode(pinoBotaoPronto, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO

  digitalWrite(pinoRele, HIGH); //MÓDULO RELÉ INICIA DESLIGADO
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(potenci));
  
  comandoTempo = digitalRead(pinoBotaoTemp);
  if(comandoTempo == true){
    tempoFinal = analogRead(potenci);
    comandoTempo = true;
  }

  comandoPronto = digitalRead(comandoPronto);

  if(comandoPronto == true && comandoTempo == true){
    contarTempo(true);
    ligar(true);
    if(tempo > tempoFinal){
      comandoPronto = false;
    }
  }else{
    contarTempo(false);
    ligar(false);
    zerarTemo();
  }
}

void contarTempo(bool comando){
 if(comando == true){
    Serial.println(tempo);
    tempo = tempo + 1;
    delay(1000);
 }else{
  Serial.println("Parando de contar TEMPO!");
 }
}

void zerarTemo(){
  tempo = 0;
}

void ligar(bool comand){
  if(comand == true){
    digitalWrite(pinoRele, LOW); //LIGA O MÓDULO RELÉ (LÂMPADA ACENDE)
    statusLamp = 1; //VARIÁVEL RECEBE O VALOR 1
    delay(200); //INTERVALO DE 200 MILISSEGUNDOS
  }else{
      digitalWrite(pinoRele, HIGH); //DESLIGA O MÓDULO RELÉ (LÂMPADA APAGA)
      statusLamp = 0; //VARIÁVEL RECEBE O VALOR 0
      delay(200); //INTERVALO DE 200 MILISSEGUNDOS
  }

}

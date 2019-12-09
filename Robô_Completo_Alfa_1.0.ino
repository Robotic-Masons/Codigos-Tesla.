/*
Robotic Masons ©2019. project in extension - IFTO - Institudo Federal de ciencias e tecnologia do tocantins - Campus Gurupi.(dicentes).
Code by:Robotic Masons ©2019 - Co - Colgate, Gabriel Colgate.
Colgate(https://github.com/Colgate13);//SoftWare Enginner, Sensors, Motors and base of code.
Cadu();//Servants and System of hardware.
Matheus(Mephisto);//Debugger and AUX_create.
*/

//Teste

//Biblioteca necessarias.{
#include <Ultrasonic.h>
#include <Servo.h>
//}



//Variaveis Definitivas.

//Minima refletancia;
#define fita_1 980 //refletancia da linha preta. AJUSTAR COM A Refletancia REAL

//Maxima refletancia.
#define fita_2 1023// refletancia da linha preta. AJUSTAR COM A Refletancia REAL
#define SERVO 6//Variavel do Servo
//


//Variaveis do TIPO Sensor Ultrasonico e do Servo.
Ultrasonic ultrassom(12, 13);//Varievel do tipo, Ultrassonico.
Servo myservo;//Variavel do tipo Servo.
Servo myservo;//Variavel do tipo Servo.



//Pinagem
//Ultrasonico TRIG 12 ECHO 13
//Servo Porta 6.
int controlar = 7;//Porta 7. Modulo de Rele Solido SSR.
//O rele e um interruptor de energia, quando ele esta LOW a energia passa, quando ele esta HIGH a energia e cortada
int pinoSensor = A5;//Porta A5. Sensor de Refletancia;
int led_1 = 7;//Led 1. Porta 7.
int led_2 = 4;//Led 2. Porta 4.
//



//Variaveis AUXILIARES.
int valor = 0;//Sensor de Refletancia. Vai receber o INPUT DE Refletancia.
float sensorUltra;//Sensor Ultrassonico. Vai receber o INPUT do sensro Ultrasonico.
//



void Ultrasonico()
{
  sensorUltra = ultrassom.read();//Retornar a distancia. NÃO VAI SER EM CM.
  Serial.print(sensorUltra);
  Serial.println(" ");
  //delay(1000);
  if(sensorUltra <= 20)//distancia Para O servo fechar a garra.
  { //
    digitalWrite(led_1, LOW);//Led vai acender
    //   myservo.write(val);
    for (pos = 0; pos < 120; pos++)//120° que o servo vai fechar.
    {
      myservo.write(pos);
    //  delay(15);
    }

// NÃO MEXA SEM AUTORIZAÇÃO DO COLGATE KLR.
/*     CODIGO AUX SE PRECISAR TESTAR ALGO. OU IMPREMENTAR ASSIM.
else if(sensorUltra > 20)//distancia Para O o led Desligar. e o Servo abrir.
{
  digitalWrite(led_1, HIGH);
  for (pos = 90; pos >= 0; pos--)
  {
    myservo.write(pos);
//    delay(15);
//    delay(15);
  }
}
*/
  //  delay(1000);
    for (pos = 90; pos >= 0; pos--)
    {
      myservo.write(pos);
  //    delay(15);
  //    delay(15);
    }
  }
  else if(sensorUltra > 20)//distancia Para O o led Desligar.
  {
    digitalWrite(led_1, HIGH);
  }
}



void SensorRefletancia_MotorAndar_MotorParar()//Para ligar e Desligar o Motor com a refletancia.
{
  valor = analogRead(pinoSensor);//Variavel Valor recebe o INPUT do sensor de refletancia.
  //int valorCorrigido = valor;
  Serial.print("refletancia:");//Imprimir no Monitor Serial;
  Serial.println(valor);
  delay(100);

 while(valor > fita_1 && valor < fita_2)
 //Enqunto valor estiver entre FITA_1 E FITA_2. Motor funcionar.
 {
   digitalWrite(led_1, HIGH);
   digitalWrite(controlar, LOW);//Motor ON
 }
 if(valor > fita_1 && valor < fita_2)//Se o valor não estiver entre FITA_1 E FITA_2 Motor parar.
 {
   digitalWrite(led_2, LOW);
   digitalWrite(controlar, HIGH);//Motor OFF.
 }

 /*
 if(tal tal tal la)
Motor ajustar quando sair.
 */


/* Parte Esquecida do CODIGO. NÃO MEXA SEM AUTORIZAÇÃO DO COLGATE KLR.
  if(valor <= fita_1)
  {// || //valor <= fita_2){
  digitalWrite(led_2, HIGH);
  }else {
    digitalWrite(led_2, LOW);
  }
  */
}



void setup()
{
  Serial.begin(9600);
  myservo.attach(SERVO);//Inicialização da Variavel Servo;
  myservo.write(0);//
  pinMode(led_1, OUTPUT);//Inicialização da porta
  pinMode(led_2, OUTPUT);//Inicialização da porta
  pinMode(controlar, OUTPUT);//Inicialização da porta, como Saida = OUTPUT
}


void loop()
{
  Ultrasonico();//Chamado a função Ultrasonico
  SensorRefletancia_MotorAndar_MotorParar();//Chamando a função SensorRefletancia_MotorAndar_MotorParar.
}

/*
Pinagem: Modulo de Rele Solido SSR.
//Lado que tem tres lugares
– CH1: Sinal Relé 1
– DC+: 5V
– DC-: GND
//Lado que tem dois lugares
outPut
SW1 = LADO POSITIVO//da bateria
OUTRO Negativo//do objeto a ser ligado
*/
//Robotic Masons ©2019. project in extension - IFTO - Institudo Federal de ciencias e tecnologia do tocantins - Campus Gurupi.(dicentes)

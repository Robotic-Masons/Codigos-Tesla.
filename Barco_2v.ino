/*
  Codigo por, Gabriel Barros Feitosa Sá, Estudante do IFTO- Campus Gurupi, 2° Ano com curso tecnico em administração; Para o robo - Ya_Rescue Water, Jice 2020;
  Github: https://github.com/Colgate13
  Curriculo e meios de contato: https://docs.google.com/document/d/1n1fT8HZiJeGDnf8ANTzSK1vN8pbBSZNgp-VigQ4nwyw/edit?usp=sharing
*/

// Bibliotecas {
#include <SerialRelay.h> // Biblioteca do Rele Serial
#include <String.h> // Biblioteca de Strings
#include <SPI.h> //Biblioteca Shield Ethernet
#include <Ethernet.h> //Biblioteca Shield Ethernet
#include <SD.h> //Biblioteca Shield Ethernet + SD 
// Fim das Bibliotecas }

const byte NumModules = 1;

SerialRelay relays(4, 5, NumModules); // (data, clock, numeros de modulos)

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };// Mac para conectar com roteador
IPAddress ip(192, 168, 0, 102); // IP address
EthernetServer server(80); // conexão pela porta 80

String readString  = String(30);//Uma string, para ler inputs

File webFile;//Variavel da biblioteca SD.

String statusMotor;//Variavel Auxiliar para motor

void setup()
{
  Ethernet.begin(mac, ip);// Inicializar o shield Ethernet
  //server.begin();
  Serial.begin(9600);//Inicializar Monitor serial

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("O shield Ethernet não foi encontrado. Desculpe, não pode funcionar sem hardware. :(");
    while (true) {
      delay(1); // não faça nada por 1 m/s
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();//Iniciando o servidor
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());//Mostra em qual Ip a pagina web vai ser mostrada

  // inicialização SD card
  Serial.println("Carregando SD card...");
  if (!SD.begin(4)) {// Inicializando conexão com cartao SD
    Serial.println("ERROR - SD card FAIL!");//SD não conectado
    return;
  }
  Serial.println("SUCCESS - SD card carregado.");
  // arquivo index.htm
  if (!SD.exists("index.htm")) {//Procurando arquivo index.htm no cartão SD
    Serial.println("ERROR - FAIL index.htm file!");//Arquivo não encontrado
    return; // sem localização do cartão SD
  }
  Serial.println("SUCCESS - arquivo index.htm .");//Tudo certo
}
void loop()
{
  EthernetClient client = server.available(); // tentativa de conexão com o client
  if (client) {//Se cliente for igual a "true"(verdadeiro) então executar todo o codigo abaixo, se for igual a "false" (falso) Não executar o codigo abaixo
    boolean currentLineIsBlank = true;
    while (client.connected()) {//Enguanto client estiver conectado, o todo abaixo sera executado
      if (client.available()) {//Stado do cliente

        char c = client.read();//Variavel c vai receber client.read(), vai receber as informaçoes sobre qual dispositivo o cliente ta acessando

        if (readString.length() < 30) {
          readString += (c);//ReadString pega as informaçoes da variavel C, soma cada uma delas com ele mesmo
        }

        if (c == '\n' && currentLineIsBlank) {//Cliente conectado, tudo certo

          client.println("HTTP/1.1 200 OK");//Cabecario HTTP, passando que a conexão esta toda certa
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          webFile = SD.open("index.htm");//Carregando pagina htm no navegador do client
          if (webFile) {
            while (webFile.available()) {
              client.write(webFile.read());
            }
            webFile.close();
          }
          break;

          if (readString.indexOf("Frente") > 0)//Se client aperta o botão Frente, então executar codigo, cada fez que o client clica no butão esse codigo é executado.
          { //Se o client clicar a cada 3 segundos, o motor nunca vai parar
            Serial.println("client: Frente");
            relays.SetRelay(1, SERIAL_RELAY_ON, 1);//Motor 1 para frente
            relays.SetRelay(2, SERIAL_RELAY_ON, 1);//Motor 2 para frente
            delay(3000); //Apos 3 faça o codigo abaixo
            Serial.println("client: STOP");
            relays.SetRelay(1, SERIAL_RELAY_OFF, 1);//Apos 3 segundos rodando motor 1 desliga
            relays.SetRelay(2, SERIAL_RELAY_OFF, 1);//Apos 3 segundos rodando motor 1 desliga

          }
          if (readString.indexOf("Esquerda") > 0)
          { //Se o client clicar a cada 1 segundos, o motor 2 nunca vai parar
            Serial.println("client: Esquerda");
            relays.SetRelay(1, SERIAL_RELAY_OFF, 1);//Motor 1 para Parado
            relays.SetRelay(2, SERIAL_RELAY_ON, 1);//Motor 2 para Frente, com o motor 1 parado, o barco vai virar para Esquerda
            delay(1000); //Apos 1 segundo executar codigo abaixo
            Serial.println("client: STOP");
            relays.SetRelay(1, SERIAL_RELAY_OFF, 1);//Apos 1 segundos rodando motor 1 desliga
            relays.SetRelay(2, SERIAL_RELAY_OFF, 1);//Apos 1 segundos rodando motor 1 desliga

          }
          if (readString.indexOf("Direita") > 0)
          {
            Serial.println("client: Direita");
            relays.SetRelay(1, SERIAL_RELAY_ON, 1);//Motor 1 para Frente, com o motor 2 parado, o barco vai virar para Direita
            relays.SetRelay(2, SERIAL_RELAY_OFF, 1);//Motor 2 para Parado
            delay(1000); //Apos 1 segundo executar codigo abaixo
            Serial.println("client: STOP");
            relays.SetRelay(1, SERIAL_RELAY_OFF, 1);//Apos 1 segundos rodando motor 1 desliga
            relays.SetRelay(2, SERIAL_RELAY_OFF, 1);//Apos 1 segundos rodando motor 2 desliga
          }
          if (readString.indexOf("UpAncorar") > 0)
          {

            relays.SetRelay(3, SERIAL_RELAY_ON, 1);//Motor da ancora que tem um ima para regatar, puxa ancora para cima
            delay(2000);

          }
          if (readString.indexOf("DownAncorar") > 0)
          {

            relays.SetRelay(3, SERIAL_RELAY_OFF, 1);//Motor da ancora que tem um ima para regatar, Desse ancora para resgatar algo

          }
        }

        if (c == '\n') {
          currentLineIsBlank = true;//Se a conecão for igual a "\n" currentLineIsBlank, currentLineIsBlank = Uma linha por vez
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }

      }
    }
    delay(1);//Apos 1 segundo executar codigo abaixo
    readString = "";//ReadString fica sem valor
    client.stop();//Fecha conexao com o client


  }

}

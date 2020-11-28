#include <SerialRelay.h>
#include <String.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

const byte NumModules = 1;

SerialRelay relays(4, 5, NumModules); 

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 102); // IP address
EthernetServer server(80); // conexão pela porta 80

String readString  = String(30);

File webFile;

String statusMotor;

void setup()
{
  Ethernet.begin(mac, ip);
  //server.begin();
  Serial.begin(9600);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  // inicialização SD card
  Serial.println("Carregando SD card...");
  if (!SD.begin(4)) {
    Serial.println("ERROR - SD card FAIL!");
    return;
  }
  Serial.println("SUCCESS - SD card carregado.");
  // arquivo index.htm
  if (!SD.exists("index.htm")) {
    Serial.println("ERROR - FAIL index.htm file!");
    return; // sem localização do cartão SD
  }
  Serial.println("SUCCESS - arquivo index.htm .");
}
void loop()
{
  EthernetClient client = server.available(); // tentativa de conexão com o client
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (readString.length() < 30) {
          readString += (c);
        }

        if (c == '\n' && currentLineIsBlank) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          webFile = SD.open("index.htm");
          if (webFile) {
            while (webFile.available()) {
              client.write(webFile.read());
            }
            webFile.close();
          }
          break;

      if (readString.indexOf("Frente") > 0)
          {
            Serial.println("client: Frente");
            relays.SetRelay(1, SERIAL_RELAY_ON, 1);
            relays.SetRelay(2, SERIAL_RELAY_ON, 1);
            delay(3000); 
         Serial.println("client: STOP");
            relays.SetRelay(1, SERIAL_RELAY_OFF, 1);
            relays.SetRelay(2, SERIAL_RELAY_OFF, 1);

          }
          if (readString.indexOf("Esquerda") > 0)
          {
            Serial.println("client: Esquerda");
            relays.SetRelay(1, SERIAL_RELAY_OFF, 1);
            relays.SetRelay(2, SERIAL_RELAY_ON, 1);
            delay(1000); 
            Serial.println("client: STOP");
            relays.SetRelay(1, SERIAL_RELAY_OFF, 1);
            relays.SetRelay(2, SERIAL_RELAY_OFF, 1);

          }
          if (readString.indexOf("Direita") > 0)
          {
            Serial.println("client: Direita");
            relays.SetRelay(1, SERIAL_RELAY_ON, 1);
            relays.SetRelay(2, SERIAL_RELAY_OFF, 1);
            delay(1000); 
            Serial.println("client: STOP");
            relays.SetRelay(1, SERIAL_RELAY_OFF, 1);
            relays.SetRelay(2, SERIAL_RELAY_OFF, 1);
          }
           if (readString.indexOf("UpAncorar") > 0)
          {
             
            relays.SetRelay(3, SERIAL_RELAY_ON, 1);
            delay(2000); 

           }
            if (readString.indexOf("DownAncorar") > 0)
          {
             
            relays.SetRelay(3, SERIAL_RELAY_OFF, 1);

           }
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }

      }
    }
    delay(1);
    readString = "";
    client.stop();


  }

}

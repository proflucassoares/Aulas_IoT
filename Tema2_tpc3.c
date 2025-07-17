// Programa: Envio de dados para o Arduino via Ethernet
// Bibliotecas necessárias
#include <SPI.h>         // Comunicação SPI entre o Arduino e o W5100
#include <Ethernet.h>    // Comunicação em rede e transmissão de dados

#define PINO_LED 3       // Macro para o pino 3 (LED)

byte mac[]    = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};   // Endereço MAC
IPAddress ip        (192,168,1,1);      // Endereço IP estático
IPAddress gateway   (192,168,1,254);    // Gateway
IPAddress subnet    (255,255,255,0);    // Máscara de sub-rede

EthernetServer server(80);  // Servidor web na porta 80

int led = 0;  // Estado do LED: 0 = desligado, 1 = ligado

void setup() {
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);      // LED inicialmente desligado

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  Serial.begin(9600);
  Serial.println("Server iniciado!");
}

void loop() {
  EthernetClient client = server.available();
  if (!client) return;

  Serial.println("Novo cliente conectado");
  boolean currentLineIsBlank = true;
  String request = "";
  String msg = "LED DESLIGADO";

  // Lê a requisição HTTP
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      request += c;

      // Quando chegar ao fim dos headers (linha em branco), envia resposta
      if (c == '\n' && currentLineIsBlank) {
        // Processa comando GET /on ou GET /off
        if (request.indexOf("GET /on")  >= 0) {
          digitalWrite(PINO_LED, HIGH);
          msg = "LED LIGADO";
        }
        if (request.indexOf("GET /off") >= 0) {
          digitalWrite(PINO_LED, LOW);
          msg = "LED DESLIGADO";
        }

        // Envia resposta HTTP e página HTML
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        client.println("<h1>Controle do LED</h1>");
        client.print  ("<p>Estado atual: ");
        client.print  (msg);
        client.println("</p>");
        client.println("<form action=\"/on\"><button>LED ON</button></form>");
        client.println("<form action=\"/off\"><button>LED OFF</button></form>");
        client.println("</html>");
        break;
      }

      // Detecta linha em branco
      if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
  }

  // Fecha conexão
  delay(1);
  client.stop();
  Serial.println("Cliente desconectado");
}

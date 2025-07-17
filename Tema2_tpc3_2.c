// Bibliotecas necessárias
#include <SPI.h>         // Para comunicação SPI entre o Arduino e o W5100
#include <Ethernet.h>    // Para configuração da rede e transmissão de dados

#define BOTAO_D1 5       // Cria uma Macro para o pino 5

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // Endereço MAC
IPAddress ip(192, 168, 1, 15);     // Define o endereço IP
IPAddress gateway(192, 168, 1, 1); // Define o gateway
IPAddress subnet(255, 255, 255, 0); // Define a máscara de sub‑rede

EthernetServer server(80);          // Inicializa o servidor web na porta 80

void setup() {
  pinMode(BOTAO_D1, INPUT);         // Pino D5 como entrada digital

  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
} // Fim da função setup()

void loop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // Ao encontrar uma linha em branco após o header, responde
        if (c == '\n' && currentLineIsBlank) {
          // Cabeçalho HTTP padrão
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 2"); // Recarrega a página a cada 2 s
          client.println();
          
          // Conteúdo HTML
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<h1>Envio de informações pela rede utilizando Arduino</h1>");
          client.print  ("Porta Digital 5 - Estado: ");
          
          int estado = digitalRead(BOTAO_D1);
          client.print(estado);
          client.print(" (0 = Desligado, 1 = Ligado)");
          
          client.println("</html>");
          break;
        }

        // Detecta fim de linha em branco
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);             // Dá tempo para o navegador receber os dados
    client.stop();
    Serial.println("client disconnected");
  }
}

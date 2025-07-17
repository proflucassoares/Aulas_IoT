/******************************************************
 * Programa: Leitura de dados na serial do Arduino
 ******************************************************/

#define LED_PINO 13  // Cria uma Macro para o pino 13

/* Função: void setup()
 * Descrição: Configurações de hardware utilizadas no programa.
 */
void setup() {
  // Pino 13 como saída digital
  pinMode(LED_PINO, OUTPUT);
  // Inicializa a comunicação Serial (9600 bps, 8N1)
  Serial.begin(9600, SERIAL_8N1);
  // Aguarda conexão da porta serial
  while (!Serial);
  // Mensagem inicial
  Serial.println("Enviando dados!");
} // Fim da função setup()

// Variável para receber dados pela serial
int valor_recebido = 's';

void loop() {
  // Verifica se há dados no buffer serial
  if (Serial.available() > 0) {
    // Lê o valor recebido
    int valor_recebido = Serial.read();

    // Se for 'L' ou 'l', liga o LED
    if (valor_recebido == 'L' || valor_recebido == 'l') {
      digitalWrite(LED_PINO, HIGH);
      Serial.println("Estado do LED Ligado!");
    }
    // Se for 'D' ou 'd', desliga o LED
    if (valor_recebido == 'D' || valor_recebido == 'd') {
      digitalWrite(LED_PINO, LOW);
      Serial.println("Estado do LED Desligado!");
    }
  } // Fim do if Serial.available()
} // Fim da função loop()

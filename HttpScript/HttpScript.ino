#include <WiFi.h>
#include <HTTPClient.h>
#include <WifiClientSecure.h>

// Credenciais da sua rede industrial
const char* ssid = "Esp32WifiBravo";
const char* password = "Senhateste";

// URL de teste (JSONPlaceholder é um serviço comum para testes de API)
const char* serverName = "https://jsonplaceholder.typicode.com/todos/1";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado com sucesso!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Criamos o objeto cliente seguro
    WiFiClientSecure* client = new WiFiClientSecure;

    if (client) {
      // Em um cenário real, você usaria client ->setCACert(rotCACertificate);
      //Para este exemplo simples, vamos ignorar a validação do certificado : 
      client->setInsecure();

      HTTPClient https;

      Serial.print("[HTTPS] Iniciando requisição... \n");
      if (https.begin(*client, serverName)) {

        // Realiza o GET
        int httpCode = https.GET();

        // httpCode será positivo se o servidor responder
        if (httpCode > 0) {
          Serial.printf("[HTTPS] Código de resposta: %d\n", httpCode);

          if (httpCode == HTTP_CODE_OK) {
            String payload = https.getString();
            Serial.println("Conteúdo recebido:");
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTPS] Falha na requisição, erro: %s\n", https.errorToString(httpCode).c_str());
        }

        https.end();
      } else {
        Serial.printf("[HTTPS] Não foi possível conectar ao servidor\n");
      }

      delete client;  // Libera memória
    }
  }
  // Aguarda 10 segundos para a próxima leitura (evita sobrecarga no servidor)
  delay(10000);
}
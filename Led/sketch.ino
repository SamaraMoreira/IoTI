#include <WiFi.h>           // Biblioteca para conectar à rede WiFi com a ESP32
#include "PubSubClient.h"    // Biblioteca para usar o protocolo MQTT (Publicar e Assinar mensagens)

// Credenciais da rede WiFi e servidor MQTT
const char* ssid = "Wokwi-GUEST";      // Nome da rede WiFi (SSID)
const char* password = "";             // Senha da rede (vazio para rede aberta)
const char* mqtt_server = "test.mosquitto.org"; // Servidor MQTT público (broker)

WiFiClient espClient;          // Cria uma instância do cliente WiFi
PubSubClient client(espClient); // Cria uma instância do cliente MQTT que usa o WiFiClient

// Definições do hardware
const int ledPin = 2;          // Pino GPIO 2 é o pino do LED integrado na ESP32
float receivedTemp = 0;        // Variável para armazenar a temperatura recebida via MQTT

// Função para configurar a conexão WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);        // Imprime o nome da rede WiFi

  WiFi.mode(WIFI_STA);         // Configura a ESP32 no modo "Station" (cliente WiFi)
  WiFi.begin(ssid, password);  // Inicia a conexão com a rede WiFi

  // Aguarda até que a ESP32 esteja conectada ao WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");         // Mostra progresso enquanto tenta se conectar
  }

  Serial.println("");
  Serial.println("WiFi connected");     // Conexão WiFi estabelecida
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());       // Exibe o endereço IP obtido
}

// Função de callback que é chamada quando uma mensagem MQTT é recebida
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);                 // Imprime o tópico da mensagem recebida
  Serial.print("] ");
  
  // Converter o payload (os dados da mensagem) em uma string
  String tempString;
  for (int i = 0; i < length; i++) {
    tempString += (char)payload[i];     // Concatena o payload em uma string
  }
  Serial.println(tempString);           // Exibe a string recebida (temperatura)

  // Converter a string recebida em um número decimal (float)
  receivedTemp = tempString.toFloat();  // Converte a string para float
  Serial.print("Received temperature: ");
  Serial.println(receivedTemp);         // Exibe a temperatura recebida

  // Verificar se a temperatura recebida é maior que 30°C
  if (int(receivedTemp) > 30) {
    digitalWrite(ledPin, HIGH);         // Acende o LED se a temperatura for maior que 30°C
    Serial.println("Temperature is above 30°C");
  } else {
    digitalWrite(ledPin, LOW);          // Apaga o LED se a temperatura for 30°C ou menor
    Serial.println("Temperature is below 30°C ");
  }
}  

// Função para reconectar ao servidor MQTT, caso a conexão seja perdida
void reconnect() {
  while (!client.connected()) {         // Fica em loop até que a ESP32 se conecte ao broker MQTT
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-LED-";    // Cria um ID único para o cliente
    clientId += String(random(0xffff), HEX); // Gera um valor aleatório para o ID

    // Tenta se conectar ao broker MQTT
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");     // Conexão com sucesso
      client.subscribe("/ThinkIOT/temp");  // Inscreve-se no tópico de temperatura
    } else {
      Serial.print("failed, rc=");     // Falha na conexão
      Serial.print(client.state());    // Exibe o estado do cliente
      Serial.println(" try again in 5 seconds");
      delay(5000);                     // Aguarda 5 segundos antes de tentar novamente
    }
  }
}

// Função setup que é chamada uma vez ao iniciar o programa
void setup() {
  pinMode(ledPin, OUTPUT);      // Configura o pino do LED como saída
  Serial.begin(115200);         // Inicia a comunicação serial para depuração
  setup_wifi();                 // Conecta ao WiFi
  client.setServer(mqtt_server, 1883);  // Configura o servidor MQTT e a porta (1883 é a padrão do MQTT)
  client.setCallback(callback);         // Define a função callback para receber mensagens MQTT
}

// Função loop que roda continuamente após o setup
void loop() {
  if (!client.connected()) {     // Verifica se o cliente MQTT está conectado
    reconnect();                 // Se não estiver, tenta reconectar
  }
  client.loop();                 // Mantém a comunicação MQTT ativa
}

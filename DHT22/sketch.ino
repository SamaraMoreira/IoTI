#include <WiFi.h>          
#include <PubSubClient.h>     
#include <DHTesp.h>           

//pino do sensor DHT e cria uma instância da classe DHTesp
const int DHT_PIN = 15; 
DHTesp dht;

// Configurações de rede WiFi
const char* ssid = "Wokwi-GUEST";     
const char* password = "";          

// Configuração do servidor MQTT (broker)
const char* mqtt_server = "test.mosquitto.org"; // Servidor MQTT público (Mosquitto)

WiFiClient espClient;         
PubSubClient client(espClient);


unsigned long lastMsg = 0;    // tempo última mensagem enviada
float temp = 0;               // temperatura
float hum = 0;                //umidade

// Função que configura a conexão WiFi
void setup_wifi() {
  delay(10);
  Serial.println(); 
  Serial.print("Connecting to ");
  Serial.println(ssid);  

  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password);  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");   
  }

  randomSeed(micros()); 

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  
}

// Função de callback que é chamada sempre que uma mensagem MQTT é recebida
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);  
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);  
  }
}

// Função para reconectar ao servidor MQTT caso a conexão seja perdida
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX); 

   
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");    
      client.publish("/ThinkIOT/Publish", "Welcome");   
      client.subscribe("/ThinkIOT/Subscribe");         
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());  
      Serial.println(" try again in 5 seconds");
      delay(5000);                   
    }
  }
}

// Função de setup que é chamada uma vez no início do programa
void setup() {
  Serial.begin(115200);   
  setup_wifi();         
  client.setServer(mqtt_server, 1883);  // Define o servidor MQTT e a porta (1883 é a porta padrão do MQTT)
  client.setCallback(callback);         // Define a função callback para tratar mensagens recebidas
  dht.setup(DHT_PIN, DHTesp::DHT22);    // Configura o sensor DHT22
}

// Função principal que roda em loop continuamente
void loop() {
  if (!client.connected()) {  
    reconnect();              
  }
  client.loop();            

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;       

    // Obtém os dados de temperatura e umidade do sensor DHT22
    TempAndHumidity data = dht.getTempAndHumidity();

    // Publica a temperatura no tópico MQTT "/ThinkIOT/temp"
    String temp = String(data.temperature, 2);  // Converte a temperatura para string com 2 casas decimais
    client.publish("/ThinkIOT/temp", temp.c_str());

    // Publica a umidade no tópico MQTT "/ThinkIOT/hum"
    String hum = String(data.humidity, 1); 
    client.publish("/ThinkIOT/hum", hum.c_str());

    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(hum);
  }
}

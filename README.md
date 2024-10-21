# Checkpoint 2

## Integrantes
- Samara de Oliveira Moreira - RM552302
- Vinicius Monteiro Manfrin - RM552293
- Angelica de Melo
- Giovanni Sguizzardi - RM551261
- Nicolas E. Inohue - RM98057
- Marcel Prado - RM99841

## Arquitetura 
![Arquitetura da solução](https://raw.githubusercontent.com/SamaraMoreira/IoTI/main/arquitetura.jfif)

### 1. **Dispositivos ESP32**:
  - **ESP32 com DHT22 (Publicador)**:
    - Este dispositivo está conectado ao sensor **DHT22** e faz leituras de temperatura e umidade.
    - Ele atua como **publicador** no sistema MQTT, enviando as leituras para um tópico específico no broker Mosquitto (por exemplo, `/ThinkIOT/temp`).
  - **ESP32 com LED (Inscrito)**:
    - O segundo **ESP32** está conectado a um LED na porta **GPIO 15**.
    - Ele é **inscrito (subscriber)** no tópico em que o primeiro ESP32 publica as leituras do DHT22.
    - Quando ele recebe uma mensagem com a temperatura do primeiro ESP32 através do tópico `/ThinkIOT/temp`, verifica se a temperatura é superior a **30°C**. Se for,ele aciona a porta GPIO 15 para **ligar o LED**.
### 2. **Protocolo MQTT e Mosquitto**:
  - O **broker Mosquitto** é responsável por receber as mensagens do ESP32 com o sensor DHT22 e distribuir essas informações para os inscritos, incluindo o segundo ESP32 com o LED.
  - O primeiro ESP32 publica a temperatura (por exemplo, no tópico `/ThinkIOT/temp`), e o segundo ESP32, inscrito nesse tópico, recebe as informações de temperatura.
### 3. **Node-RED**:
  - O **Node-RED** está inscrito no mesmo tópico (`/ThinkIOT/temp`) que exibe a temperatura e no tópico (`/ThinkIOT/hum`) para umidade no dashboard.
  - **Apenas Visualização**: O **Node-RED** não interfere no controle do LED. Ele serve apenas para monitorar e exibir os dados do sensor DHT22.
### Ciclo do Sistema:
1. O **ESP32 com o DHT22** lê a temperatura e umidade e publica essas informações em um tópico MQTT. No tópico `/ThinkIOT/temp` a temperatura e no `/ThinkIOT/hum` a umidade).
2. O **Node-RED** está inscrito em ambos os tópicos e exibe as leituras no dashboard.
3. O **ESP32 com o LED** também está inscrito no tópico `/ThinkIOT/temp`. Quando recebe os dados de temperatura:
  - Se a temperatura for maior que **30°C**, ele aciona o LED na porta **GPIO 15** para **ligar**.
  - Se a temperatura for inferior a **30°C**, o LED permanece **desligado**.
### Resumo:
- O **ESP32 com DHT22** publica a temperatura no broker Mosquitto.
- O **ESP32 com o LED** se inscreve no tópico de temperatura e, ao receber uma temperatura acima de 30°C, **liga o LED**.
- O **Node-RED** serve apenas para exibir as informações do sensor em um dashboard visual, sem controle sobre o LED.

## Objetivo 
O objetivo da solução proposta é desenvolver um sistema de monitoramento e controle de temperatura e umidade utilizando dispositivos ESP32, o protocolo MQTT e a plataforma Node-RED. Essa solução permite:

1. **Monitoramento Remoto**: Coletar dados de temperatura e umidade de um sensor DHT22 conectado a um ESP32 e disponibilizar essas informações em um dashboard visual por meio do Node-RED.

2. **Ação Automática**: Acionar um LED conectado a outro ESP32 com base nas leituras de temperatura, permitindo uma resposta automática ao ambiente. Se a temperatura ultrapassar 30°C, o LED é ativado, sinalizando um alerta visual.

3. **Comunicação Eficiente**: Utilizar o protocolo MQTT e um broker Mosquitto para garantir uma comunicação leve e eficiente entre os dispositivos, permitindo que os dados do sensor sejam publicados e acessados em tempo real por diferentes inscritos.


## Vídeos

##### Vídeo principal - 7min
  
[Principal](https://youtu.be/SU-YVkxO2Yg)

##### Vídeo Extra
- Observação: Adicionando mais testes para validação no vídeo abaixo

[Testes](https://www.youtube.com/watch?v=hKFk4saf1qs)


## Configurações da servidor
  
Adicione um novo servidor no nó mqqt

 ![Novo servidor](https://github.com/SamaraMoreira/IoTI/blob/main/servidor/Add.PNG)

Essa é a visualização padrão

 ![Padrão](https://github.com/SamaraMoreira/IoTI/blob/main/servidor/localhost.PNG)

Altere a URL e porta
  
 ![URL ](https://github.com/SamaraMoreira/IoTI/blob/main/servidor/mosquito.PNG)

Insira o tópico, no exemplo a seguir seria o tópico da temperatura. Crie também um novo nó para o tópico da umidade

 ![Tópico ](https://github.com/SamaraMoreira/IoTI/blob/main/servidor/topicoInscri%C3%A7%C3%A3o.PNG)

 

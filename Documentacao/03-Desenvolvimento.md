# Materiais

Os materiais utilizados no projeto foram:

- ESP32
- Sensor de Umidade e temperatura DHT11
- Sensor de Umidade do solo capacitivo
- Sensor de Luminosidade (photoresistor)
- Mini Ventoinha
- Mini Bomba de água
- Mini tela OLED
- Fonte Ajustável Para Protoboard
- Resistores, Diodos, Leds

# Desenvolvimento

O desenvolvimento do projeto foi realizado de forma incremental, testando cada componente isoladamente antes de integrar ao sistema final. Primeiro simulamos no Wokwi para entender o funcionamento dos sensores, depois testamos cada sensor isoladamente no hardware real, em seguida testamos o servidor MQTT, depois os atuadores substituídos por LEDs, e por fim integramos tudo.

## Desenvolvimento do Aplicativo

### Interface

Foi desenvolvido um aplicativo mobile utilizando o MIT App Inventor. O aplicativo se conecta ao broker MQTT para publicar comandos (acionar atuadores) e subscrever nos tópicos dos sensores para exibir os dados em tempo real.

### Código

O App Inventor não utiliza código tradicional. A lógica é construída através de blocos visuais que são arrastados e conectados, similar a um quebra-cabeça. Isso facilita o desenvolvimento sem necessidade de conhecimento em programação.

## Desenvolvimento do Hardware

### Montagem

A montagem foi feita de forma gradual. Inicialmente, utilizamos o simulador Wokwi para entender o funcionamento dos sensores e do ESP32 sem necessidade do hardware físico. Depois, cada sensor foi montado e testado individualmente em pequenos projetos separados (DHT11, sensor de umidade do solo, photoresistor). Os atuadores foram primeiro substituídos por LEDs para testar a lógica de acionamento de forma segura. Só após validar cada componente isoladamente, realizamos a montagem final integrando todos os sensores e atuadores.

### Desenvolvimento do Código

O código do ESP32 foi desenvolvido em C++ utilizando PlatformIO. O desenvolvimento seguiu a mesma abordagem incremental: primeiro código para cada sensor isolado, depois integração com MQTT, depois lógica de acionamento dos atuadores, e por fim integração de tudo no código principal.

## Comunicação entre App e Hardware

A comunicação é feita via protocolo MQTT. O ESP32 publica os dados dos sensores em tópicos específicos (temperatura, umidade, luminosidade, umidade do solo). Um servidor em C++ se conecta ao broker MQTT, recebe esses dados e armazena em banco de dados SQLite. O servidor também pode enviar comandos para acionar os atuadores (ventoinha, bomba, luz) através de tópicos MQTT que o ESP32 escuta.

```
                                  ┌─────────────┐
                                  │     App     │
                                  └──────┬──────┘
                                         │ commands
                                         ▼
┌─────────────┐       MQTT        ┌─────────────┐
│   ESP32     │ ◄───────────────► │  Mosquitto  │
│  (sensors)  │   sensor/*        │   Broker    │
└─────────────┘                   └──────┬──────┘
                                         │
                                         ▼
                                  ┌─────────────┐      ┌─────────────┐
                                  │   Server    │ ───► │   SQLite    │
                                  └─────────────┘      │  garden.db  │
                                                       └─────────────┘
```

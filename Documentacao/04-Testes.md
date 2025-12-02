# Testes do Projeto

Os testes foram realizados de forma incremental, validando cada componente isoladamente antes da integração final.

## Testes Realizados

1. **Simulação no Wokwi** - Testamos a lógica básica dos sensores e ESP32 no simulador antes de usar o hardware real

2. **Testes dos Sensores** - Cada sensor foi testado individualmente:

   - DHT11: verificação das leituras de temperatura e umidade
   - Sensor de umidade do solo: calibração e leitura de valores
   - Photoresistor: detecção de luminosidade

3. **Teste do MQTT** - Validamos a conexão com o broker e o envio/recebimento de mensagens

4. **Teste dos Atuadores** - Substituímos os atuadores por LEDs para testar a lógica de acionamento de forma segura

5. **Teste do Servidor** - Verificamos o recebimento dos dados via MQTT e armazenamento no SQLite

6. **Integração Final** - Testamos o sistema completo com todos os componentes conectados

## Desafios

Um dos principais desafios foi a questão da alimentação elétrica. O ESP32 opera com 3.3V, mas os atuadores (ventoinha, bomba d'água) precisam de 5V para funcionar. Para resolver isso, utilizamos transistores BC337 como chaves eletrônicas, funcionando como uma espécie de relé. O ESP32 aciona o transistor com 3.3V, e o transistor permite a passagem de 5V da fonte de alimentação para os atuadores. Foi necessário também usar diodos de proteção e configurar corretamente o módulo de fonte ajustável para protoboard.

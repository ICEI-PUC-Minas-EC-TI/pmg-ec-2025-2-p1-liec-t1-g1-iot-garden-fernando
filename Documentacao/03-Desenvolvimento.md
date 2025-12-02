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

O desenvolvimento do projeto foi realizado de forma incremental, testando cada componente isoladamente antes de integrar ao sistema final.

## Etapa 1: Simulação no Wokwi

Inicialmente, utilizamos o simulador Wokwi para entender o funcionamento dos sensores e do ESP32. Isso permitiu testar a lógica do código sem a necessidade do hardware físico, acelerando o aprendizado sobre cada componente.

## Etapa 2: Testes Isolados dos Sensores

Após a simulação, partimos para testes com o hardware real. Cada sensor foi testado individualmente em pequenos projetos separados:

- Teste do DHT11 (temperatura e umidade)
- Teste do sensor de umidade do solo
- Teste do photoresistor (luminosidade)

## Etapa 3: Teste do Servidor MQTT

Com os sensores funcionando, desenvolvemos e testamos o servidor C++ que recebe os dados via MQTT. Validamos a conexão com o broker e o armazenamento no banco SQLite.

## Etapa 4: Teste dos Atuadores com LEDs

Antes de conectar os atuadores reais (ventoinha, bomba, luz), substituímos cada um por LEDs para testar a lógica de acionamento de forma segura.

## Etapa 5: Integração Final

Por fim, integramos todos os componentes: sensores, atuadores reais, comunicação MQTT e servidor. Cada parte já havia sido validada isoladamente, facilitando a identificação de problemas na integração.

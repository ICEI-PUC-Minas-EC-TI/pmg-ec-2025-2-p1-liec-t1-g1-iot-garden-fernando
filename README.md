# IOT ESP32 Garden

`Coração Eucarístico`

`Engenharia da Computação`

`Oitavo`

`Laboratório de Introdução de Engenharia da computação`

## Integrantes

- Fernando Vieira da Rocha Valentim

## Orientador

- Marta Dias Moreira Noronha

## Resumo

Aplicação para monitoramento de hortas/plantas com ESP32 com integração na nuvem usando protocolo MQTT.

O ESP32 coleta dados dos sensores (temperatura, umidade do ar, umidade do solo e luminosidade) e publica via MQTT. Um servidor em C++ recebe esses dados e armazena em banco SQLite. O sistema também aciona atuadores automaticamente (ventoinha, bomba d'água, luz) com base nas leituras dos sensores.

# Código (do arduino ou esp32)

<li><a href="Codigo/iot/src/main.cpp"> Código Fonte (.ino)</a></li>

# Aplicativo para Smartphone

<li><a href="App/README.md"> Aplicativo </a></li>

# Apresentação

<ol>
<li><a href="Apresentacao/README.md"> Vídeo do Funcionamento</a></li>
<li><a href="Apresentacao/README.md"> Fotos do Projeto</a></li>
</ol>

# Manual de Utilização

<li><a href="Manual/manual de utilização.md"> Manual de Utilização</a></li>

# Documentação

Componentes:

- ESP32
- Sensor de Umidade e temperatura DHT11
- Sensor de Umidade do solo capacitivo
- Sensor de Luminosidade (photoresistor)
- Mini Ventoinha
- Mini Bomba de água
- Mini tela OLED
- Fonte Ajustável Para Protoboard
- Transistores BC337
- Resistores, Diodos, Leds

<ol>
<li><a href="Documentacao/01-Introducão.md"> Introdução</a></li>
<li><a href="Documentacao/02-Metodologias Ágeis.md"> Metodologias Ágeis</a></li>
<li><a href="Documentacao/03-Desenvolvimento.md"> Desenvolvimento </a></li>
<li><a href="Documentacao/04-Testes.md"> Testes </a></li>
<li><a href="Documentacao/05-Conclusão.md"> Conclusão </a></li>
<li><a href="Documentacao/06-Referências.md"> Referências </a></li>
</ol>

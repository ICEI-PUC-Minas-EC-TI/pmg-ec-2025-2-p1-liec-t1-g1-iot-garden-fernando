# Manual de Utilização

## Aviso Importante

**Sistema Operacional:** Todo o desenvolvimento deste projeto foi + realizado no **Linux**. Não testamos nem oferecemos suporte para o + desenvolvimento em outros sistemas operacionais (Windows, macOS, + etc.). Caso você utilize outro SO, poderá encontrar diferenças ou + problemas não documentados na hora de buildar o projeto e fazer upload para o ESP32

Este projeto foi desenvolvido utilizando o **PlatformIO** integrado ao **Visual Studio Code (VS Code)**. O PlatformIO oferece diversas vantagens em relação à IDE tradicional do Arduino, incluindo gerenciamento automático de bibliotecas, suporte a múltiplas placas e um ambiente de desenvolvimento mais robusto.

**Não utilizamos a Arduino IDE** neste projeto. Portanto, para compilar e fazer upload do código para o ESP32, é necessário ter o VS Code com a extensão PlatformIO instalada e saber usar ambos

---

## Configuração do Ambiente de Desenvolvimento

### Pré-requisitos

- **Visual Studio Code** instalado
- **Extensão PlatformIO IDE** para VS Code

### Instalação do PlatformIO

1. Abra o Visual Studio Code
2. Vá em **Extensions** (Ctrl+Shift+X)
3. Pesquise por "PlatformIO IDE"
4. Clique em **Install**
5. Aguarde a instalação completa e reinicie o VS Code se necessário

---

## Executando o Código do ESP32 (Pasta IOT)

### Passo 1: Configurar Credenciais

**IMPORTANTE:** O arquivo `platformio.ini` fornecido no repositório contém credenciais placeholder. Antes de compilar o projeto, você **deve substituir** as credenciais pelas suas credenciais reais (WiFi, MQTT, etc.) e depois **fechar e abrir o projeto novamente**

1. Abra o arquivo `Codigo/iot/platformio.ini`
2. Localize as variáveis de configuração (SSID, senha WiFi, credenciais MQTT, etc.)
3. Substitua os valores placeholder pelas suas credenciais reais

### Passo 2: Abrir o Projeto pela Interface do PlatformIO

É recomendado abrir o projeto através da interface do PlatformIO para garantir que todas as configurações sejam carregadas corretamente:

1. Abra o Visual Studio Code
2. Clique no ícone do **PlatformIO** na barra lateral esquerda (ícone de formiga/alien)
3. Na tela inicial do PlatformIO, clique em **"Open Project"**
4. Navegue até a pasta `Codigo/iot` do projeto
5. Selecione a pasta e clique em **Abrir**

Para mais detalhes sobre como abrir projetos no PlatformIO, consulte:

- [Documentação Oficial do PlatformIO - IDE para VSCode](https://docs.platformio.org/en/latest/integration/ide/vscode.html)
- [Vídeo Tutorial - Getting Started with PlatformIO](https://www.youtube.com/watch?v=PYSy_PLjytQ)

### Passo 3: Instalar Bibliotecas (se necessário)

O PlatformIO normalmente baixa as bibliotecas automaticamente ao abrir o projeto. Porém, caso isso não aconteça ou ocorram erros de compilação relacionados a bibliotecas faltando, você precisará instalá-las manualmente.

**Bibliotecas utilizadas neste projeto:**

| Biblioteca | Descrição |
|------------|-----------|
| **PubSubClient** | Cliente MQTT para comunicação com o broker |
| **DHT sensor library for ESPx** | Leitura do sensor de temperatura e umidade DHT11 |
| **Adafruit SSD1306** | Controle da tela OLED |
| **Adafruit GFX Library** | Biblioteca gráfica para displays (dependência do SSD1306) |
| **Adafruit BusIO** | Comunicação I2C/SPI (dependência do SSD1306) |

**Como instalar bibliotecas manualmente:**

1. Clique no ícone do **PlatformIO** na barra lateral esquerda
2. Clique em **"Libraries"** no menu do PlatformIO
3. Pesquise pelo nome da biblioteca (ex: "PubSubClient")
4. Clique na biblioteca desejada e depois em **"Add to Project"**
5. Selecione o projeto `Codigo/iot` e confirme

Para mais detalhes sobre gerenciamento de bibliotecas no PlatformIO:
- [Documentação Oficial - Library Manager](https://docs.platformio.org/en/latest/librarymanager/index.html)

### Passo 4: Aguardar Inicialização do PlatformIO

- O PlatformIO irá detectar automaticamente o arquivo `platformio.ini`
- Aguarde o download automático das bibliotecas e dependências
- Este processo pode levar alguns minutos na primeira vez

### Passo 5: Conectar o ESP32

1. Conecte o ESP32 ao computador via cabo USB
2. Verifique se o driver do ESP32 está instalado corretamente
3. O PlatformIO deve detectar a porta COM automaticamente
4. (Talvez voce tenha que clicar no botão "reset" do ESP32 quando faz upload)

### Passo 6: Compilar o Projeto

1. Clique no ícone de **checkmark (✓)** na barra inferior do VS Code
   <adicionar imagem>

### Passo 7: Upload para o ESP32

1. Clique no ícone de **seta para direita (→)** na barra inferior do VS Code
   <adicionar imagem>

### Passo 8: Monitor Serial (Opcional)

Para visualizar as mensagens do ESP32:

1. Clique no ícone de **tomada/plug** na barra inferior do VS Code

### Passo 9: Funcionando

1. Se tudo deu certo o ESP32 vai tentar conectar no WIFI e depois no broker mqtt configurado. Caso sucesso, imediatamente vai começar a emitir os dados dos sensores

---

## Configuração do Servidor

_Seção a ser adicionada posteriormente._

---

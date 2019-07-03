# ESP32 NoC Platform


## O que é uma NoC?
Com a evolução das tecnologias, hoje já é possível construir um sistema completo em um único chip, os quais são denominados sistemas integrados ou SoCs.

Os barramentos que interligam os núcleos de um SoC possuem fortes limitações quanto ao desempenho, pois sua largura de banda é fixa. Como a tendência é de que mais núcleos sejam integrados, as pesquisas apontam para as redes-em-chip (NoCs).

**Uma NoC (Network On Chip) consiste de um conjunto de roteadores e canais interligados formando uma rede chaveada. Os núcleos são conectados aos terminais da rede e a comunicação ocorre pela troca de pacotes.** As NoCs oferecem uma série de vantagens em relação ao barramento: largura de banda escalável, paralelismo em comunicação, maior freqüência de operação, entre outras. Contudo, também apresenta desvantagens, como maior latência de comunicação e sobrecusto de silício.


## Separação do Projeto:
- **Emulador**
- **Aplicativo**
- **Firmware ESP32**


## Emulador

> Desenvolvido em C++, o emulador da NoC tem por objetivo ler um arquivo de mensagens e as executar na rede montada. Processadoes e roteadores são representados por threads e são criados com base no tamanho XY da rede. O emulador adiciona na rede mensagem por mensagem, sendo ela composta por **id**, **source_id**, **target_id** e **end**. Cada mensagem tem o seu source e o seu target. Conforme a topologia escolhida, a rede troca mensagens e gera um arquivo de estatística.

## Aplicativo

> Desenvolvido com o framework React-Native, o aplicativo da NoC tem por objetivo permitir a configuração e o envio de mensagens. O usuário seleciona o source, o target e escreve a mensagem. Ao clicar para enviar, o app envia a mensagem ao Firebase Database Realtime. Na parte de configuração, o usuário seleciona o tamanho XY da NoC e o tipo da topologia. Da mesma forma, essa configuração é adicionada no Firebase.

## Firmware ESP32

> Desenvolvido com o framework Arduino na IDE PLatformIO, o firmware tem por objetivo monitorar eventos do Firebase Database Realtime e, consequentemente, configurar a rede e executar o roteamento de mensagens. Para isso, o firmware controla via SPI o driver de matriz de LED MAX7219. A matriz mostra em tempo real o roteamento do pacote ao ligar os LEDs correspondentes ao id atual. Juntamente, o app controla o display OLED SSD1306 via I2C, mostrando informações da mensagem em trânsito.


# REQUIREMENTS:
Para a execução de cada módulo, favor ler o README.md interno.

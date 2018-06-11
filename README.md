# Socket-Aplication

Este programa implementa um **socket** em C++.

## Intro - Socket Overview

Sockets são utilizados para comunicação interprocessos, promovendo uma API entre a camada de Aplicação
da camada OSI e as inferiores.  
Sockets são representados por um *file descriptor* determinados na sua criação, diferentemente de *pipes*
podem realizar comunicação suportam comunicações entre processos não relacionados e,mais ainda, são
o principal meio de comunicação entre máquinas diferentes.
>Neste programa foi implementado um socket usando os protocolos TCP/IP, sendo o protocolo IP o IPv4.

## IPv4 Header

O header do protocolo IPv4 é dado da seguinte maneira:  
Version(4bits) | ... | Protocol(1Bytes) | ... | Source IP (4Bytes) | Destination IP(4Bytes) | ...
-------------- | --- | ---------------- | --- | ------------------ | ---------------------  | ---
4(IPv4) 0100   | ... | 6(TCP)           | ... | 192.168.0.1        | 168.172.0.3            | ...

## Port : Definições de portas

As são definidas por 2Bytes, portanto $2^{16}$ possibilidades de portas. Contudo existem portas reservadas
pela IANA e não é uma boa prática utilizá-las, são elas:  
>0 → 1023 : Reservadas.  
>1024 → 49151 : Semi-Reservadas.  
>49152 → 65535 : Aplicações em geral.

## Usage

A porta que o socket utilizará, bem como o endereço IP da rede local do Server, no caso do *client*,
devem ser fornecidos pelo usuário.
Como compilar e utilizar este software:

*Host 1*(**Server**):  

``` bash
$make
$make server
Enter port number: <port_number>
```

*Host 1*(**Client**):  

``` bash
$make
$make client
Enter port number: <port_number>
Enter IP addr: <ip_addr>
```

*Como descobrir seu IP?* - **Aqui estão algumas maneiras**:  

``` bash
$ifconfig [interface]
#no meu caso <wlp1s0>
Link encap:Ethernet  HWaddr 00:00:00:00:00:00  
inet addr:xxx.xxx.xxx.xxx  Bcast:255.255.255.255  Mask:255.255.0.0

$hostname -I
xxx.xxx.xxx.xxx

$netstat -tpe
tcp  0  0 xxx.xxx.xxx.xxx:41160  ec2-34-211-99-53.:https ESTABLISHED antonio  491685  8683/firefox
tcp  0  0 xxx.xxx.xxx.xxx:55442  151.101.204.134:https   ESTABLISHED antonio  491034  8683/firefox
```
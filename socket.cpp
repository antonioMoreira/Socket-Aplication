/**
 * @brief Programa cria um socket simples.
 * 
 * @file socket.cpp
 * @author Antonio Moreira
 * @date 2018-06-02
 */


/*
	IPv4: Endereçamento de 32bits (RFC 791 and RFC 1122)
		(!) Explicar todo o header do ipv4 (14 campos, sendo 13 obrigatórios)
			|Version| |...| |Protocol| |...| |SourceIP_Addr| |DestIP_Addr| |14th Field(opcional)|
			
			Neste programa:
				Version: 4
				Protocol: 6 (TCP)


	Sockets: Comunicação inter-processos.
		Promover uma interface (API) entre a camada Aplicação e as inferiores
		
		Internet (TCP/IP):

				Host 1									Host 2
			→ Aplicação								→ Aplicação								 	
				» Sockets  <-- Comunicação Lógica -->	» Sockets
			→ Transporte							→ Transporte
			→ Rede									→ Rede
			→ Física/Enlace							→ Física/Enlace
				|										|
				---------- Comunicação Real -------------

		→ Aplicação: HTTP, HTTPS, SSH, DNS, FTP, MQTT, SSL...
		→ Transporte: TCP, UDP
		→ Rede: IP, ICMP, ARP
		→ Física: Ethernet, PPP, ADSL
	
	→ Port: 16bits~2Bytes para representar portanto 0~65535
			- 0 	-> 1023	: Reservadas pela IANA
			- 1024	-> 49151: Semi-reservedos (IANA)
			- 49152 -> 65535: Clients Programs
	

	→ Sockets (OVERVIEW):
		Quando você cria um socket, voce deve especificar o estilo de comunicação (style of communication) que você
		deseja usar e o tipo de protocolo (protocol) que deseja implementar. 	
		
		→ Communication style: defines the user-level semantics of sending and receiving data of the socket
		  Questões relacionadas
		  		(1) Qual a unidade de dado de trasmissão?
				(2) Dados podem ser perdidos durante a trasmissão? ie, deve ter garantia de recebimento?
				(3) A comunicação é somente com um 'parceiro'?
		Namespace : for naming the socket. A socket name (“address”) is meaningful only 
					in the context of a particular namespace

					Protocol Family PF_ : sulfix of every namespace symbolic name
					Address  Family AF_ : sulfix of a corresponding symbolic name, designates the address format for that namespace 
		
		→ Protocols: Each protocol is valid for a particular namespace and communication style; a namespace is sometimes
					 called a protocol family because of this, which is why the namespace names start with ‘PF_’.


	→ Interface Naming
		» lo :	loopback interface
				comunicação client e server no mesmo host (usando TCP/IP)
		
		» virbr0 :	virtual bridge 0

		» eth0 : Ethernet 0
				



	---------------------------------------------------------------------------------------------------------

	» Configurações de rede	
		$ netstat
		$ ifconfig
		$ telnet (?)
		$ networkctl [OPTION]
		$ /etc/init.d/network-mannager [OPTION] ↑ através do networkctl
		
	» Scan de Rede
		$ wireshark
		$ nmap
		$ Sane : https://help.ubuntu.com/community/sane (é mais pra scan de hw)  

	» SSH (/etc/init.d/ssh)
		Clinet SSH → SSHD (deamon do SSH)
					 $ ps -axl ("?" : diz a existência de deamons)
		/etc/init.d/ssh status/stop/start/...
		cat /etc/init.d/sshd_congig

*/

#include "socket.h"


/**
 * @brief Função para informar corretamente o erro encontrado, 
 * 		  informando o valor da variável 'errno' e finalizando o processo.
 * 		  $ man errono	:	para mais informações.
 */
void getError(const char *msg){
	printf("\nERRO!\n%s\n[errno:\t%d]\n", msg, errno);
	exit(EXIT_FAILURE); // Finaliza o processo
}


/**
 * @brief : Função libera todos os endereços alocados por Socket
 * 
 * @param exists_clients : verifica se existem clients a serem liberados 
 */
void Socket::__free(bool exists_clients){
	if(exists_clients){
		for(int i=0; i<nclients;i++){
			if(add_clients[i] != NULL){
				free(add_clients[i]);
			}
		}
		free(len_clients);
		free(fd_clients);
		free(add_clients);
	}

	free(buffer);
	free(add_socket);
}


void getInformation(int fd){
	struct sockaddr_in aux;
	socklen_t len_aux = sizeof(struct sockaddr_in);

	if(getsockname(fd, (struct sockaddr *)&aux, &len_aux) == -1){
		getError("Error in getsockname()");
	}
	
	printf("Socket Name: %s:%d\n", inet_ntoa(aux.sin_addr), ntohs(aux.sin_port));
	printf("len : %d\n", len_aux);

}


int Socket::getFdSocket() {
	return fd_socket;
}


void Socket::closeSocket(){
	if(shutdown(fd_socket, SHUT_RDWR) == -1){
		__free();
		getError("Error in listen()");
	}
}


void readMsg(int fd, char *buffer){
	int n;
	
	if((n = read(fd, buffer, 255)) == -1)
		getError("Error in read()");
	
	printf("%d bytes read from buffer\n", n);
	printf("Msg: %s\n", buffer);
}



void writeMsg(int fd, char *buffer){
	int n;
	char msg[256];

	//getchar();
	scanf("%s", msg);
	memcpy(buffer, &msg, strlen(msg)); 
	//	acho que não é uma boa prática escrever direto no buffer, além disso
	//	não precisa ficar zerando toda hora o buffer, ACHO

	if((n = write(fd, buffer, sizeof(buffer))))
		getError("Error in write()");

	printf("%d bytes write on buffer\n", n);
}


/**
 * @brief :	Função, primeiramente, inicializa as devidas estruturas dos clients, 
 * 			específicada pela variável 'nclients', aguarda a conexão no socket
 * 			para todos os clients e, por fim, exibe a informação de cada endereço.
 * 
 */
void Socket::acceptClients(){
	// non bloking
	add_clients = (struct sockaddr_in **)malloc(sizeof(struct sockaddr_in *)*nclients);
	len_clients = (socklen_t *)malloc(sizeof(socklen_t)*nclients);
	fd_clients = (int *)malloc(sizeof(int)*nclients);

	for(int i=0; i<nclients; i++){
		add_clients[i] = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));
		len_clients[i]=sizeof(struct sockaddr_in);

		/**
		 * @brief	accept(fd, sockaddr *, socklen_t *)
		 * 			Função para aceitar a conexão em um socket.
		 * 			Neste caso, aceita a conexão do primeiro client.
		 * 
		 * @param int fd :
		 * 
		 * @param sockaddr * :
		 * 
		 * @param socklen_t * :
		 * 			
		 */
		if((fd_clients[i] = accept(fd_socket, (struct sockaddr *)add_clients[i], &(len_clients[i]))) == -1){
			closeSocket();
			getError("Error in accept()");
		}

		printf("Fd client[%d]: %d\n",i, fd_clients[i]);
		getInformation(fd_clients[i]);
	}
}


Socket::Socket(int port, int nclients){
	add_socket = (struct sockaddr_in *)calloc(1, sizeof(sockaddr_in));

	/**
	 * @brief :	socket(domain, type, protocol)
	 * 		Um socket recentemente criado pela função socket() não tem endereço (address).
	 * 		Neste programa é implementado um tcp_socket com protocolo IPv4.
	 * 	
	 * 
	 * 		tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
	 * 		udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	 * 	
	 * @param : domain: Especifica o dominio de comunicaçao.
	 * 		'Address(protocol) family':
	 * 		AF_UNIX		Local Communication
	 * 		AF_LOCAL	Local Communication
	 * 		AF_INET		IPv4
	 * 		AF_INET6	IPv6
	 * 
	 * 		Em 	<socket.h> : #define AF_INET PF_INET
	 * 						 #define PF_INET 2
	 * 
	 * 
	 * @param : type: Especifica a semantica de comunicação.
	 * 					SOCK_STREM para abrir um socket TCP
	 * 					SOCK_DGRAM para abrir um socket UDP
	 * 					
	 * 
	 * @param : protocol: é protocolo IP, para envio e recebimento de pacotes.
	 * 		Possiveis valores são IPPROTO_IP e IPPROTO_TCP, no caso de (TCP Socket) <in.h>
	 * 							  IPPROTO_IP e IPPROTO_UDP, no caso de (UDP Socket)  <in.h>
	 * 		(!) Note que em <in.h>  #define IPPROTO_IP 0
	 * 								#define IPPROTO_TCP 6
	 * 								#define IPPROTO_UDP 17
	 * 
	 * @return if( == -1) getError()
	 * 		São feitas verificações nas invocações de funções de <socket.h>
	 * 		para ter certeza que as funções estão retornando valores esperados.
	 * 		Este if(), com a mesma finalidade, é realizado em diversas funções 
	 * 		a seguir, portanto a explicação dele será omitida posteriormente. 
	 * 		
	 * 		Em caso de sucesso socket() retorna o fd, caso contrário retorna -1.
	 * 		Neste último caso, getError() é invocada para obter mais informações do erro,
	 * 		além disso, todos os ponteiros são devidamente liberados.
	 */
	if((fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){ 
		__free(false);
		getError("Error in socket()\n");
	}	

	printf("fd server: %d\n", fd_socket);

	this->port = port;

	/**
	 * @brief Configuração do socket
	 *  	Config1:	Quando um processo deseja receber novos pacotes ou conexões,
	 * 					ele deve ligar um socket a um endereço de interface local.
	 * 					Esta ligação pode ser ÚNICA se, na configuração, específicar um endereço,
	 * 					como por exemplo, 192.168.2.1. Neste caso, haverá apenas um par (endereço, porta).
	 * 					Uma alternativa é, chamar a função bind() como o endereço setado com INADDR_ANY, 
	 * 					neste caso o socket será ligado a todas as interfaces locais. Esta é a configuração
	 * 					feita neste programa.
	 * 
	 * 		Config2:	Atribui à estrutura a familia do protocolo correspondente, neste caso IPv4.
	 * 
	 * 		Config3:	Para especificar a porta de comunicação do socket
	 * 					htons() é usada para converter para NBO			
	 */
	add_socket->sin_addr.s_addr = INADDR_ANY; // Config1
	add_socket->sin_family = AF_INET;		  // Config2
	add_socket->sin_port = htons(port);	   	  // Config3

	/**
	 * @brief : bind(fd, sockaddr *, sockaddr_len)
	 * 	Dá ao socket um endereço (address). Se outros processos desejarem se comunicar com
	 * 	ele, deverão conhecer o endereço:
	 * 		Socket <--> local interface address.
	 * 
	 * 	(!) Pare recuperar o nome (address) : getsockname();
	 * 
	 * @param int fd : file descriptor recebido quando o socket foi criado pela função socket().
	 * 
	 * @param sockaddr : Cada sockaddr tem sua struct dependendo do formato utilizdo,
	 * 					 por isso o cast para o sockaddr genérico.
	 * 					 Neste caso foi utilizado o IPv4. Ver a definição a struct sockaddr em questão!
	 * 
	 * @param unsigned int sockaddr_len : Tamanho da struct usada pelo formato em questão
	 */
	if(bind(fd_socket, (struct sockaddr *) add_socket, (unsigned int)sizeof(struct sockaddr_in)) == -1){
		__free(false);
		getError("Error in bind()\n");
	}

	this->nclients = nclients;

	/**
	 * @brief : listen(fd, n)
	 * 
	 * @param int fd : file descriptor do soccket gerado
	 * 
	 * @param int n : numero de conexoes que o sistema pode segurar por vez.
	 * 				  Se n=2, por exemplo, o socket pode segurar duas conexões
	 * 				  ao mesmo tempo.
	 */
	if(listen(fd_socket, nclients) == -1){
		__free(false);
		getError("Error in listen()");
	}

	buffer = (char *)calloc(256,sizeof(char));
	bzero(buffer, 256);

	printf("Socket criado...\n");
}
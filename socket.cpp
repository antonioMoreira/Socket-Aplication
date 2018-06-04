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
		$ /etc/init.d/network-mannager [OPTION]
		$ networkctl [OPTION]
		
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h> // acho que nao precisa deste include
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 65535 // passar porta por argv ?
#define SOCKET_PATH '/home/antonio/Documents/Redes (Kalinka)/Trabalho' // pode usar pwd() na fç?
#define NUM_CLIENTS 2

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

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
 * @brief	Função recebe um vetor de endereços para
 * 			dar free()
 * 
 * @param vec_addr : vetor de endereços
 * @param size_vec : tamanho do vetor
 */
void __free(void **vec_addr, int size_vec){
	for(int i=0; i<size_vec; i++)
		free((void*)vec_addr[i]);	
}

void getInformation(int fd){
	struct sockaddr_in aux;
	socklen_t len_aux = sizeof(struct sockaddr_in);

	getsockname(fd, (struct sockaddr *)&aux, &len_aux);
	
	printf("Socket Name: %s:%d\n", inet_ntoa(aux.sin_addr), ntohs(aux.sin_port));
	printf("len : %d\n", len_aux);
}

/**
 * @brief
 * 		Argument counter (argc) & Argument Vector (argv)
 * 		
 * @param argc :	Armazena o numero de argumentos passados.
 * 					1 (default) : nome do binário.
 * 
 * @param argv :	Armazena os valores dos argumentos passados em um vetor < const char * >.
 * 					(!) cons char * : é read-only variable.
 * 
 * @return int 
 */
int main(int argc, char const *argv[]){
	/**
	 * @brief Struct que descreve um socket IPv4 em <in.h> (16 Bytes)
	 * 
	 *  'Herda' a struct sockaddr que descreve um socket address generico com (16 Bytes) <bits/socket.h> <sys/socket.h>
	 * 		herda __SOCKADDR_COMMON de <sockaddr.h> (ou ele pega )
	 * 		unsigned short int sockaddr.sa_family;	« Especifica a familia do endereço (unsigned short int)
	 * 		char sockaddr.sa_data[14];		« 14 Bytes of protocol address 
	 * 
	 * 	Em sockaddr_in:
	 * 			» in_port_t sin_port; (unisgned short int em <_stdint.h>)
	 * 				função hlens(PORT_NUMBER) usada para converter no devido formato			
	 * 	
	 * 			» struct in_addr sin_addr; struct definida em <in.h>
	 * 				Internet Address:
	 * 				typedef uint32_t in_addr_t; (unsigned int) <_stdint.h>
	 * 				struct in_addr{ in_addr_t s_addr; };
	 * 			
	 * 			» sockaddr_in.zeros; Bom costume inicializar com zeros, por isso o calloc()
	 * 			
	 * 			(!) Endereço e porta são armzenados em Network Byte Order (NBO)
	 */
	struct sockaddr_in *sockaddr_server = (struct sockaddr_in *)calloc(1, sizeof(sockaddr_in)); 
	struct sockaddr_in *addr_client1, *addr_client2;

	//nao precisa ser alocado acho
	socklen_t *len_server = (socklen_t *)calloc(1, sizeof(socklen_t)); 
	socklen_t *len_client1, *len_client2;
	
	int fd_server, fd_client1, fd_client2; // Files descriptors

	void **vec_addr; //montar o vetor e passar pra função

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
	if((fd_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1){ 
		free(len_server);
		free(sockaddr_server);
		getError("Error in socket()\n");
	}

	// Tem gente que faz a verificação se fd_server > 0 não sei se precisa
	printf("fd_svr: %d\n", fd_server);

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
	sockaddr_server->sin_addr.s_addr = INADDR_ANY; // Config1
	sockaddr_server->sin_family = AF_INET;		   // Config2
	sockaddr_server->sin_port = htons(PORT);	   // Config3

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
	if(bind(fd_server, (struct sockaddr *) sockaddr_server, (unsigned int)sizeof(struct sockaddr_in)) == -1){
		free(len_server);
		free(sockaddr_server);
		getError("Error in bind()\n");
	}

	/**
	 * @brief : listen(fd, n)
	 * 
	 * @param int fd : file descriptor do soccket gerado
	 * 
	 * @param int n : numero de conexoes que o sistema pode segurar por vez.
	 * 				  Neste programa, o socket pode gerenciar 2 conexões.
	 */
	if(listen(fd_server, 2) == -1){
		free(len_server);
		free(sockaddr_server);
		getError("Error in listen()");
	}


	/**
	 * @brief Inicializa as devidas estruturas do client1
	 */
	addr_client1 = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));
	len_client1 = (socklen_t *)malloc(sizeof(socklen_t));
	
	/**
	 * @brief :	accept(fd, sockeddr *, socklen_t *)
	 * 			Função para aceitar a conexão em um socket.
	 * 			Neste caso, aceita a conexão do primeiro client.			
	 * 
	 * @param int fd : 
	 * 
	 * @param sockaddr :
	 * 
	 * @param socklen_t * : 
	 * 
	 */
	if((fd_client1 = accept(fd_server, (struct sockaddr*) addr_client1, len_client1)) == 0){
		free(len_server);
		free(sockaddr_server);
		free(addr_client1);
		free(len_client1);
		getError("Error in accept()");
	}
	
	/**
	 * @brief  Inicializa as devidas estruturas do client2
	 * 
	 */
	addr_client2 = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));
	len_client2 = (socklen_t *)malloc(sizeof(socklen_t));
	
	/**
	 * @brief	Aceita a conexão do client 2
	 */
	if((fd_client1 = accept(fd_server, (struct sockaddr*) addr_client2, len_client2)) == 0){
		free(len_server);
		free(sockaddr_server);
		free(addr_client1);
		free(len_client1);
		getError("Error in accept()");
	}

	////----------- Jeito mais bonitinho -----------
	//struct sockaddr_in *clients[NUM_CLIENTS];
	//socklen_t *len_clients[NUM_CLIENTS];
	//
	//for(int i=0; i<NUM_CLIENTS; i++){
	//	clients[i] = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));
	//	len_clients[i] = (socklen_t *)malloc(sizeof(socklen_t));
	//
	//	// captar o erro disso ↓
	//	accept(fd_server, (struct sockaddr *)clients[i], len_clients[i]);
	//}
	////-------------------------------------------

	// socket pronto
	// read()
	// write()
	// USAR recvmmsg() E sendvmmsg()!



	if(shutdown(fd_server, SHUT_RDWR) == -1){
		//Dar free nos clients tbm
		free(len_server);
		free(sockaddr_server);
		getError("Error in listen()");
	}

	free(len_server);
	free(sockaddr_server);
	exit(EXIT_SUCCESS);
}
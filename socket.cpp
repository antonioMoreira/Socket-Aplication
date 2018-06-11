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


int *Socket::getFdClients(){
	return fd_clients;
}


/**
 * @brief	Função que fecha o socket e sua respectiva porta e
 * 			libera todas as estruturas relacionadas
 * 
 */
void Socket::closeSocket(){
	/**
	 * @brief Fecha a comunicação do socket em questão
	 * 
	 * @param int fd : file descriptor do socket
	 * 
	 * @param int how : modo como deve ser finalizado.
	 * 					Neste caso SHUT_RDWR deve é impedida a trasmissão e
	 * 					recebimento de mensagens. 
	 */
	if(shutdown(fd_socket, SHUT_RDWR) == -1){
		__free();
		getError("Error in listen()");
	}
}


/**
 * @brief	Função le uma mensagem do buffer especificado por um
 * 			file descriptor
 * 
 * @param fd : file descriptor
 * @param buffer : endereço do buffer
 */
void readMsg(int fd, char *buffer){
	int n;

	//bzero(buffer, 1);
	*buffer = '\0';
	//printf("Recebendo msg de [%d]\n", fd);

	/**
	 * @brief : recv(fd, buffer *, buffe_len, flag)
	 * 			System call usada para receber uma mensagem de um socket.
	 * 
	 * @param int fd : file descriptor
	 * 
	 * @param char *buffer : endereço do buffer
	 * 
	 * @param int size_buffer : tamanho do buffer a ser lido
	 * 
	 * @param int flag : flag que identifica o tipo de recebimento
	 * 
	 */
	if((n = recv(fd, buffer, 1, MSG_WAITALL)) == -1)
		getError("Error in read()");

	//printf("%d bytes read from buffer\n", n);
	printf("Msg: %c\n", *buffer);
	//bzero(buffer, 1);
	*buffer = '\0';
}


/**
 * @brief 	Função escreve uma mensagem no buffer especificado por um
 * 			file descriptor.
 * 
 * @param fd
 * @param buffer : endereço do buffer 
 */
void writeMsg(int fd, char *buffer){
	int n;

	//printf("Digite a mensagem em [%d]: \n", fd);
	//bzero(buffer, 1);
	*buffer = '\0';
	//scanf("%s", buffer); // se voltar a ser escanf mudar pra strlen
	scanf("%c", buffer);

	/**
	 * @brief	send(fd, buffer *, size_buffer, flag)
	 * 			System call usda para trasmitir uma mensagem para um socket.
	 * 
	 * @param int fd : file descriptor do socket que vai receber a mensagem
	 * 
	 * @param buffer * : endereço do buffer
	 * 
	 * @param int len_buffer : tamanho da mensagem a ser escrita (max msg : sizeof(buffer))
	 * 
	 * @param int flag : flag que identifica ao tipo de trasmissão
	 * 
	 */
	if((n = send(fd, buffer, sizeof(char), MSG_WAITALL)) == -1)
		getError("Error in send()");

	//printf("%d bytes write on buffer\n", n);
}


sockaddr_in *Socket::getAddrSocket(){
	return add_socket;
}


/**
 * @brief :	Função, primeiramente, inicializa as devidas estruturas dos clients, 
 * 			específicada pela variável 'nclients', aguarda a conexão no socket
 * 			para todos os clients e, por fim, exibe a informação de cada endereço.
 * 
 */
void Socket::acceptClients(int nclients){
	add_clients = (struct sockaddr_in **)malloc(sizeof(struct sockaddr_in *)*nclients);
	len_clients = (socklen_t *)malloc(sizeof(socklen_t)*nclients);
	fd_clients = (int *)malloc(sizeof(int)*nclients);
	
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
		getError("Error in listen()\n");
	}

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


Socket::Socket(const char* port, in_addr_t addr, bool doBind){
	add_socket = (struct sockaddr_in *)calloc(1, sizeof(sockaddr_in));
	//bzero(buffer, 1);
	buffer = '\0';

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

	this->port = atoi(port);

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
	add_socket->sin_addr.s_addr = addr; 		// Config1
	add_socket->sin_family = AF_INET;			// Config2
	add_socket->sin_port = htons(this->port);  	// Config3

	if(doBind == true){
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
		printf("Bind realisado...\n");
	}

	printf("Socket criado...\n");
}
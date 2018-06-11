#ifndef _SKT_H__
#define _SKT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h> // acho que nao precisa deste include
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

//Biblios que fogem um pouco do negocio ↓ (que tem tudo na socket.h)
#include <strings.h>
#include <netdb.h> //tentar tirar e colocar gethostbyaddr
#include <unistd.h> // Tirar essa biblio e substitui por alguma do socket.h se der
// essa biblio é pra usar a write e a read, mas a socket tem fç pra isso, então tem que mudar.

#define EXIT_SUCESS 0
#define EXIT_FAILURE 1


// explicar todas as variáveis
class Socket{
    private:
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
	     * 			(!) Endereço e porta são armzenados em Network Byte Order (NBO) (Big Endian)
	     */
        struct sockaddr_in *add_socket;
        struct sockaddr_in **add_clients;
        
        socklen_t len_socket;
        socklen_t *len_clients;

        int fd_socket;
        int *fd_clients;
        int nclients, port;
    
	public:
		char buffer[4];
        
		Socket(const char *, in_addr_t addr = INADDR_ANY, bool doBind = false);

        
		int getFdSocket();
        int *getFdClients();
        void acceptClients(int);
        void closeSocket();
        void __free(bool exists_clients = true);
		sockaddr_in *getAddrSocket();
};


void writeMsg(int, char *);
void readMsg(int, char *);
void getInformation(int);

void getError();

#endif

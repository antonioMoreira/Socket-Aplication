/**
 * @brief Teste basico de socket
 * 
 * @file basic_socket_1.cpp
 * @author Antonio Moreira
 * @date 2018-06-02
 */

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 55555


int main(int argc, char const *argv[]){
	struct sockaddr *addr_default = (struct sockaddr *)calloc(1, sizeof(struct sockaddr));
	struct sockaddr_in *addr_ipv4 = (struct sockaddr_in *)calloc(1, sizeof(sockaddr_in));
	int id;


	//addr_default->sa_family = AF_INET;
	//printf("\naddr_default >\tsize struct: %d\n\tdata vec size: %d\n", 
	//			(int)sizeof(struct sockaddr), (int)sizeof(addr_default->sa_data));
	//for(int i=0; i<sizeof(addr_default->sa_data); i++) printf("\tdata[%d]: %c\n", i, addr_default->sa_data[i]);
	//printf("\tsa_family: %u\n", addr_default->sa_family);
	
	addr_ipv4->sin_family = AF_INET;
	addr_ipv4->sin_port = htons(PORT);
	//addr_ipv4->sin_port = (unsigned short int)PORT; 
	printf("addr_ipv4 >\tsize struct: %d\n", (int)sizeof(struct sockaddr_in));
	printf("\tsin_family: %d\n\tsin_port %d\n", addr_ipv4->sin_family, addr_ipv4->sin_port);



	free(addr_default);
	free(addr_ipv4);

	return 0;
}


//		/* Structure describing an Internet socket address.  */
//		struct sockaddr_in
//		  {
//		    __SOCKADDR_COMMON (sin_);
//		    in_port_t sin_port;			/* Port number.  */
//		    struct in_addr sin_addr;		/* Internet address.  */
//		
//		    /* Pad to size of `struct sockaddr'.  */
//		    unsigned char sin_zero[sizeof (struct sockaddr) -
//					   __SOCKADDR_COMMON_SIZE -
//					   sizeof (in_port_t) -
//					   sizeof (struct in_addr)];
//		  };



//		/* Internet address.  */
//		typedef uint32_t in_addr_t;
//		struct in_addr{ in_addr_t s_addr; };
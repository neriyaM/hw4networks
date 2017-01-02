#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>


int main()
{
	
	// open sockets with servers
    int sockfd1 , sockfd2 , sockfd3;
    struct sockaddr_in serv1_addr, serv2_addr, serv3_addr;
    struct hostent *server1 , *server2 ,*server3;
	struct in_addr addr1 , addr2 , addr3;

    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
	sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
	sockfd3 = socket(AF_INET, SOCK_STREAM, 0);
	
	serv1_addr.sin_family = AF_INET;
	serv2_addr.sin_family = AF_INET;
	serv3_addr.sin_family = AF_INET;
	
	serv1_addr.sin_port = htons(80);
	serv2_addr.sin_port = htons(80);
	serv3_addr.sin_port = htons(80);
	
	serv1_addr.sin_addr.s_addr = inet_addr("192.168.0.101");
	serv2_addr.sin_addr.s_addr = inet_addr("192.168.0.102");
	serv3_addr.sin_addr.s_addr = inet_addr("192.168.0.103");
	
	memset(serv1_addr.sin_zero, '\0', sizeof(serv1_addr.sin_zero) ); 
	memset(serv2_addr.sin_zero, '\0', sizeof(serv2_addr.sin_zero) ); 
	memset(serv3_addr.sin_zero, '\0', sizeof(serv3_addr.sin_zero) ); 	 
	
    connect(sockfd1,(struct sockaddr *) &serv1_addr, sizeof(serv1_addr) );
	connect(sockfd2,(struct sockaddr *) &serv2_addr, sizeof(serv2_addr) );
	connect(sockfd3,(struct sockaddr *) &serv3_addr, sizeof(serv3_addr) );
	
	// establish LB server to communicate hosts
	
	int sockfd, newsockfd, clilen, pid;
    struct sockaddr_in lb_addr, cli_addr;
	char buffer[256];
	int counter =0;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char *) &lb_addr, sizeof(lb_addr));

	lb_addr.sin_family = AF_INET;

	lb_addr.sin_addr.s_addr = INADDR_ANY;

	lb_addr.sin_port = htons(80);

	bind(sockfd, (struct sockaddr *) &lb_addr, sizeof(lb_addr)) ;

	listen(sockfd,5);

	clilen = sizeof(cli_addr);
	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		pid = fork();
		if (pid == 0)  {
			close(sockfd);
			bzero(buffer,256);
			read(newsockfd,buffer,255);
			if(counter == 0){
				write(sockfd1,buffer,strlen(buffer));
			} else if(counter == 1){
				write(sockfd2,buffer,strlen(buffer));
			} else {
				write(sockfd3,buffer,strlen(buffer));
			}
			//dostuff(newsockfd);
			exit(0);
		} else {
			counter++;
			counter%=3;	
			close(newsockfd);
		}
	 } /* end of while */
	 return 0; /* we never get here */
}
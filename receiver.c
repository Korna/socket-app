#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>

#include <arpa/inet.h>
#include "functions.h"
#define BUFFER_SIZE 1024


int main(){
	int sock;
	struct sockaddr_in addr;//server address
	char buf[BUFFER_SIZE];//received message
		

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("Socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	printf("Receiving messages from:%s\n", inet_ntoa(addr.sin_addr));
	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		perror("Bind");
		exit(2);
	}

	struct sockaddr_in temp_client;
	
	char* message;
	
	while(1){
		memset(buf,'\0', BUFFER_SIZE);
		int temp_len = sizeof(temp_client);
		int bytes_read = recvfrom(sock, buf, BUFFER_SIZE, 0,
		(struct sockaddr *) &temp_client, &temp_len);
		buf[bytes_read] = '\0';
		
		
		
		if(command_con(buf)){//check for first connect
			add_to_list(temp_client);
			
			if(is_first(temp_client)){
				sendto(sock, "0", strlen("0"), 0,  
				(struct sockaddr *) &temp_client, temp_len);
			}else{
				char* response = "Server is busy";
				sendto(sock, response, strlen(response), 0,  
				(struct sockaddr *) &temp_client, temp_len);
			}
		}else{
			printf("Received:%s", buf);
		}


		
		if(command_exit(buf) && is_first(temp_client)){
			printf("Client is exitting...\n");
			queue_user_out();
			send_queue(sock);
		}
			
	
		if(!command_con(buf))
			if(sendto(sock, buf, strlen(buf), 0,  
			(struct sockaddr *) &temp_client, temp_len) < 0){
		        perror("Error at sending");
		       // close(sock);
		        //exit(1);
		    }
	
	}
	return 0;
}

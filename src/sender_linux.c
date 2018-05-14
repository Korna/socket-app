#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>


#include <string.h>
#include <stdio.h>


#define BUFFER_SIZE 1024


char message_exit[] = "exit";
char message_con[] = "con";


int command_exit(char* msg){
	if(msg[0] == 'e' &&
	msg[1] == 'x' &&
	msg[2] == 'i' &&
	msg[3] == 't')
		return 1;
	else 
		return 0;
}

void handleError(char* msg){
	perror(msg);
	getchar();
	exit(1);
}


int main(){
	char recvline[BUFFER_SIZE];

	int sock;
	struct sockaddr_in addr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(sock < 0){
		handleError("Socket error");
	}else{
		printf("Created socket\n");
	}
	
	//u_long addr_long = INADDR_LOOPBACK;
	char* addr_char = "192.168.7.255";
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = inet_addr(addr_char);	//htonl(addr_long);

	
	printf("Adress is:%s\n", inet_ntoa(addr.sin_addr));

	sendto(sock, message_con, sizeof(message_con), 0,
	(struct sockaddr *)&addr, sizeof(addr));
	
	int bytes_read;
	printf("Waiting for connection...\n");
	while(1){
		bytes_read = recvfrom(sock, recvline, BUFFER_SIZE, 0, 
		(struct sockaddr *) 0, 0);
		
		if(bytes_read < 0){
		   handleError("Receive at first connect");
		}else{
			recvline[bytes_read] = '\0';
			printf("Server:%s\n", recvline);
		}
	
		if(recvline[0] == '0')
			break;
	}

	char message[BUFFER_SIZE];
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) >= 0)
		while(1){
			printf(">>");
			fgets(message, BUFFER_SIZE, stdin);
			
			sendto(sock, message, sizeof(message), 0,
			(struct sockaddr *)&addr, sizeof(addr));
		
			if(command_exit(message)){
				printf("Exitting...\n");
				break;
			}else{
				bytes_read = recvfrom(sock, recvline, BUFFER_SIZE,
				 0, (struct sockaddr *) 0, 0);
				if(bytes_read < 0){
			        handleError("Receive");
			    }else{
			    	recvline[bytes_read] = '\0';
			    	printf("Answer from server:%s", recvline);
				}
			}
		}
	else{
		handleError("Connect error");
	}
	
	close(sock);
	getchar();
	return 0;
}

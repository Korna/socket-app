#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>
#include <stdio.h>
//#include <conio.h>

char message_exit[] = "exit\0";

char msg1[] = "Hello there!\n";
char msg2[] = "Bye bye!\n";

int command_exit(char* msg){
	if(msg[0] == 'e' &&
	msg[1] == 'x' &&
	msg[2] == 'i' &&
	msg[3] == 't')
		return 1;
	else 
		return 0;
}


int main(){
	char recvline[1000];
	int sock;
	struct sockaddr_in addr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(sock < 0){
		perror("socket");
		exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	sendto(sock, "Con", sizeof("Con"), 0,
	(struct sockaddr *)&addr, sizeof(addr));
	
	while(1){
			if((recvfrom(sock, recvline, 1000, 0, 
		    (struct sockaddr *) 0, 0)) < 0){
		        perror("Receive error:");
		        close(sock);
		        exit(1);
		    }else
		    	printf("Server:%s\n", recvline);
		    	
		    if(recvline[0] == '0')
		    	break;
		    	
	}


	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) >= 0)
		while(1){
			printf("Input your string:");
			fgets(msg1, 1000, stdin);
		
			sendto(sock, msg1, sizeof(msg1), 0,
			(struct sockaddr *)&addr, sizeof(addr));
		
			
		
		//	send(sock, msg2, sizeof(msg2), 0);
			if(command_exit(msg1)){
				printf("Exitting...\n");
				break;
			}else{
				if((recvfrom(sock, recvline, 1000, 0, 
			    (struct sockaddr *) 0, 0)) < 0){
			        perror("Receive error:");
			        close(sock);
			        exit(1);
			    }else
			    	printf("Answer from server:%s\n", recvline);
			}
		}
	else{
		perror("Connect error:");
		close(sock);
		exit(1);
	}
	
	close(sock);
	return 0;
}

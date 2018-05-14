#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>

#include <arpa/inet.h>

#define BUFFER_SIZE 1024

#define max_client 1000

int current = 0;
int total = 0;

struct sockaddr_in cliaddr[max_client];//client address
int clilen[max_client];//length of client

void send_queue(int sock){//check for client length error
	int pos = 0;
	for(int i = current; i < total; ++i){
		char str[2];
   		str[0] = pos + '0';
   		str[1] = '\0';
		
		if(sendto(sock, str, strlen(str), 0,  
			(struct sockaddr *) &cliaddr[i], sizeof(cliaddr[i])) < 0){
		    perror("Error at sending:");
		}
		
		pos++;
	}

}

int checkIfExists(char *dot_ip, unsigned int port){// 0 if already exists else 1
	for(int i = current; i < total; ++i){
		if(port == cliaddr[i].sin_port && 
		strcmp(dot_ip, inet_ntoa(cliaddr[i].sin_addr)) == 0){
			return 0;
		}
	}
	return 1;
}



int add_to_list(struct sockaddr_in newcomer){
	
	char *dot_ip = inet_ntoa(newcomer.sin_addr);
	unsigned int port = newcomer.sin_port;
	
	int check = checkIfExists(dot_ip, port);
	if(check){
		cliaddr[total] = newcomer;
		total++;
		printf("add. current:%d total:%d\n", current, total);
	}
	
	return check;
}


int is_first(struct sockaddr_in newcomer){
	char *dot_ip = inet_ntoa(newcomer.sin_addr);
	unsigned int port = newcomer.sin_port;
	printf("%s:%u - ", dot_ip, port);
	if(port == cliaddr[current].sin_port && 
	strcmp(dot_ip, inet_ntoa(cliaddr[current].sin_addr)) == 0)
		return 1;
	else
		return 0;
}

int command_exit(char* msg){
	if(msg[0] == 'e' &&
	msg[1] == 'x' &&
	msg[2] == 'i' &&
	msg[3] == 't'
	)
		return 1;
	else 
		return 0;
}

int command_con(char* msg){
	if(msg[0] == 'c' &&
	msg[1] == 'o' &&
	msg[2] == 'n'
	)
		return 1;
	else 
		return 0;
}



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
			current++;
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

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>

#include <arpa/inet.h>

#define max_client 1000

int current = 0;
int total = 0;

struct sockaddr_in cliaddr[max_client];//client address
int clilen[max_client];//length of client

void send_queue(int sock){//check for client length error
	int pos = 0;
	int i;
	for(i = current; i < total; ++i){
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
	int i;
	for(i = current; i < total; ++i){
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

int queue_user_out(){
	current++;
	return current;
}

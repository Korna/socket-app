#ifndef funcs
#define funcs
void send_queue(int sock);
int checkIfExists(char *dot_ip, unsigned int port);
int add_to_list(struct sockaddr_in newcomer);
int is_first(struct sockaddr_in newcomer);
int command_exit(char* msg);
int command_con(char* msg);
int queue_user_out();
#endif

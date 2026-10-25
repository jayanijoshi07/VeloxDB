#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>

int main(){
    int client_fd = socket(AF_INET,SOCK_STREAM,0);
    if(client_fd==-1){
        std::cerr << "socket creation failed." << std::endl;
        return 1;
    }
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    int ip_result = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (ip_result != 1) {
        std::cerr << "IP conversion failed." << std::endl;
        close(client_fd);
        return 1;
    }
    
    return 0;
}
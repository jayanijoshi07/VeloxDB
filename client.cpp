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
    int connect_result = connect(client_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(connect_result==-1){
        std::cerr << "connection failed." << std::endl;
        close(client_fd);
        return 1;
    }
    int bytes_sent = write(client_fd,"hello",5);
    if(bytes_sent==-1){
        std::cerr << "write() failed ." << std::endl;
        close(client_fd);
        return 1;
    }
    char buffer[1024];
    

    int bytes_received = read(client_fd, buffer, sizeof(buffer));

    if (bytes_received == -1) {
        std::cerr << "read() failed." << std::endl;
        close(client_fd);
        return 1;
    }
    
    return 0;
}
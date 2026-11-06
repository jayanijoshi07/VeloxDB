#include <iostream>
#include <sys/socket.h> //for socket API
#include <netinet/in.h> //IPv4 address structure and related definitions
#include <arpa/inet.h> // for address conversion utilities (like htons())
#include <unistd.h> //POSIX functions such as read(),write(), close() etc
#include <cstddef> // for size_t
#include <sys/types.h> // for ssize_t 
#include <cstdint> // for uint32_t
#include <cerrno> // for errno and EINTR.
#include <string>
#include <limits>
#include "protocol.h"



int main(){
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd==-1){
        std::cerr << "socket creation failed." << std::endl;
        return 1;
    } 
    int opt = 1;
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))==-1){
        std::cerr<<"setsockopt failed." << std::endl;
        close(server_fd);
        return 1;
    }
    sockaddr_in addr{};
    addr.sin_family=AF_INET;
    addr.sin_port=htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY ;
    int bind_result = bind(server_fd,(struct sockaddr *)&addr,sizeof(addr));
    if (bind_result == -1) {
        std::cerr << "bind failed." << std::endl;
        close(server_fd);
        return 1;
    }
    int listen_result = listen(server_fd,5);
    if (listen_result == -1) {
        std::cerr << "listen failed." << std::endl;
        close(server_fd);
        return 1;
    }
    while (true) {
    int client_fd = accept(server_fd, nullptr, nullptr);

    if (client_fd == -1) {
        std::cerr << "accept() failed" << std::endl;
        continue;
    }

    while (true) {
        std::string message;

        if (!receive_message(client_fd, message)) {
            break;
        }

        std::cout << "Received: " << message << std::endl;

        if (!send_message(client_fd, "world")) {
            break;
        }
    }

    close(client_fd);
}
    close(server_fd);
    return 0;
}
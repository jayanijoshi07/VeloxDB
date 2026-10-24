#include <iostream>
#include <sys/socket.h> //for socket API
#include <netinet/in.h> //IPv4 address structure and related definitions
#include <arpa/inet.h> // for address conversion utilities (like htons())
#include <unistd.h> //POSIX functions such as read(),write(), close() etc

int main(){
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd==-1){
        std::cerr << "socket creation failed." << std::endl;
        return 1;
    } 
    int opt = 1;
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))==-1){
        std::cerr<<"setsockopt failed." << std::endl;
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
    return 0;
}
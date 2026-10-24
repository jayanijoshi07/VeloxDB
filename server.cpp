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
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)==-1)){
        std::cerr<<"setsockopt failed." << std::endl;
    }
    return 0;
}
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
        int client_fd = accept(server_fd,nullptr,nullptr);
        if(client_fd==-1){
            std::cerr<<"accept() failed" << std::endl;
            continue;
        }
        char buffer[1024];
        int bytes_received = read(client_fd,buffer,sizeof(buffer));
        if(bytes_received==0){
            std::cout<<"no data"<<std::endl;
            close(client_fd);
            continue;
        }
        else if(bytes_received==-1){
            std::cout<<"read() failed."<<std::endl;
            close(client_fd);
            continue;
        }
        else{
            std::cout.write(buffer, bytes_received);
            std::cout << std::endl;
            int bytes_sent = write(client_fd, "world", 5);
            if(bytes_sent==-1){
                std::cerr << "write() failed ." << std::endl;
                
            }
            close(client_fd);
        }
    }
    close(server_fd);
    return 0;
}
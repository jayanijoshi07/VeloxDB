#include <iostream>
#include <sys/socket.h> //for socket API
#include <netinet/in.h> //IPv4 address structure and related definitions
#include <arpa/inet.h> // for address conversion utilities (like htons())
#include <unistd.h> //POSIX functions such as read(),write(), close() etc
#include <cstddef> // for size_t
#include <sys/types.h> // for ssize_t 
#include <cstdint> // for uint32_t
#include <cerrno> // for errno and EINTR


ssize_t read_full(int fd, void* buffer, size_t count) {
    size_t total = 0;
    size_t remaining = count;

    char* ptr = static_cast<char*>(buffer);

    while (remaining > 0) {
        ssize_t bytes_received = read(fd, ptr + total, remaining);

        if (bytes_received == 0) {
            return total == 0 ? 0 : static_cast<ssize_t>(total);
        }

        if (bytes_received == -1) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        total += static_cast<size_t>(bytes_received);
        remaining -= static_cast<size_t>(bytes_received);
    }

    return static_cast<ssize_t>(total);
}

uint32_t decode_length(const unsigned char header[4]){
    uint32_t length = 0;

    length |= static_cast<uint32_t>(header[0]);
    length |= static_cast<uint32_t>(header[1]) << 8;
    length |= static_cast<uint32_t>(header[2]) << 16;
    length |= static_cast<uint32_t>(header[3]) << 24;

    return length;
}

void encode_length(uint32_t length, unsigned char header[4]) {
    header[0] = length & 0xFF;
    header[1] = (length >> 8) & 0xFF;
    header[2] = (length >> 16) & 0xFF;
    header[3] = (length >> 24) & 0xFF;
}

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
        ssize_t bytes_received = read(client_fd,buffer,sizeof(buffer));
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
            ssize_t bytes_sent = write(client_fd, "world", 5);
            if(bytes_sent==-1){
                std::cerr << "write() failed ." << std::endl;
                
            }
            close(client_fd);
        }
    }
    close(server_fd);
    return 0;
}
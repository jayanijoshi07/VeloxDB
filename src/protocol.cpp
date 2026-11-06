
#include "protocol.h"

#include <unistd.h>
#include <cerrno>
#include <limits>

ssize_t read_full(int fd, void* buffer, size_t count) {
    size_t total = 0;
    char* ptr = static_cast<char*>(buffer);

    while (total < count) {
        ssize_t bytes_received = read(fd, ptr + total, count - total);

        if (bytes_received == 0) {
            return static_cast<ssize_t>(total);
        }

        if (bytes_received == -1) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        total += static_cast<size_t>(bytes_received);
    }

    return static_cast<ssize_t>(total);
}

ssize_t write_all(int fd, const void* buffer, size_t count) {
    size_t total = 0;
    const char* ptr = static_cast<const char*>(buffer);

    while (total < count) {
        ssize_t bytes_sent = write(fd, ptr + total, count - total);

        if (bytes_sent == -1) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }

        if (bytes_sent == 0) {
            return -1;
        }

        total += static_cast<size_t>(bytes_sent);
    }

    return static_cast<ssize_t>(total);
}

void encode_length(uint32_t length, unsigned char header[4]) {
    header[0] = length & 0xFF;
    header[1] = (length >> 8) & 0xFF;
    header[2] = (length >> 16) & 0xFF;
    header[3] = (length >> 24) & 0xFF;
}

uint32_t decode_length(const unsigned char header[4]) {
    uint32_t length = 0;

    length |= static_cast<uint32_t>(header[0]);
    length |= static_cast<uint32_t>(header[1]) << 8;
    length |= static_cast<uint32_t>(header[2]) << 16;
    length |= static_cast<uint32_t>(header[3]) << 24;

    return length;
}

bool send_message(int fd, const std::string& message) {
    if (message.size() > std::numeric_limits<uint32_t>::max()) {
        return false;
    }

    uint32_t length = static_cast<uint32_t>(message.size());
    unsigned char header[4];

    encode_length(length, header);

    if (write_all(fd, header, sizeof(header)) != sizeof(header)) {
        return false;
    }

    if (write_all(fd, message.data(), message.size()) !=
        static_cast<ssize_t>(message.size())) {
        return false;
    }

    return true;
}

bool receive_message(int fd, std::string& message) {
    unsigned char header[4];

    ssize_t bytes_received = read_full(fd, header, sizeof(header));

    if (bytes_received != sizeof(header)) {
        return false;
    }

    uint32_t length = decode_length(header);

    if (length > MAX_MESSAGE_SIZE) {
        return false;
    }

    message.resize(length);

    ssize_t payload_received = read_full(fd, message.data(), length);

    return payload_received == length;
}

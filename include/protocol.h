#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <sys/types.h>

inline constexpr uint32_t MAX_MESSAGE_SIZE = 1024 * 1024;

ssize_t read_full(int fd, void* buffer, size_t count);
ssize_t write_all(int fd, const void* buffer, size_t count);

void encode_length(uint32_t length, unsigned char header[4]);
uint32_t decode_length(const unsigned char header[4]);

bool send_message(int fd, const std::string& message);
bool receive_message(int fd, std::string& message);
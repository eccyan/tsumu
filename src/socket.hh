#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cerrno>
#include <tuple>
#include <optional>
#include <memory>
#include <iostream>
#include <fmt/format.h>

namespace tsumu {

inline void throw_system_error_on(bool condition, const char* what_arg = "");

class socket {
  int _fd;
public:
  socket() = delete;
  socket(socket const &) = delete;
  socket(socket&& x) : _fd(x._fd) { x._fd = -1; }
  ~socket() { if (_fd != -1) { ::close(_fd); } }

  int fd() const { return _fd; }

  static socket make(int socket_family, int socket_type, int protocol) {
    int fd = ::socket(socket_family, socket_type, protocol);
    throw_system_error_on(fd == -1, "socket");
    return socket(fd);
  }

  template <class X>
  socket setsockopt(int level, int option_name, X&& option_value) {
    auto ret = ::setsockopt(_fd, level, option_name, &option_value, sizeof(option_value));
    throw_system_error_on(ret == -1, "setsockopt");
    return socket(std::move(*this));
  }

  socket bind(const struct sockaddr * const addr, socklen_t addrlen) {
    auto ret = ::bind(_fd, addr, addrlen);
    throw_system_error_on(ret == -1, "bind");
    return socket(std::move(*this));
  }

  socket listen(int backlog) {
    auto ret = ::listen(_fd, backlog);
    throw_system_error_on(ret == -1, "listen");
    return socket(std::move(*this));
  }

  socket accept(struct sockaddr * const addr, socklen_t& addrlen) {
    auto fd = ::accept(_fd, addr, &addrlen);
    throw_system_error_on(fd == -1, "accept");
    return socket(fd);
  }

  std::optional<size_t> read(void* buffer, size_t len) {
    auto ret = ::read(_fd, buffer, len);
    if (ret == -1 && errno == EAGAIN) {
      return {};
    }
    throw_system_error_on(ret == -1, "read");
    return { size_t(ret) };
  }

  std::optional<size_t> send(const void* const buffer, size_t len, int flags) {
    auto ret = ::send(_fd, buffer, len, flags);
    if (ret == -1 && errno == EAGAIN) {
      return {};
    }
    throw_system_error_on(ret == -1, "send");
    return { size_t(ret) };
  }

private:
  socket(int fd) :_fd(fd) { }
};

inline
void throw_system_error_on(bool condition, const char* what_arg) {
  if (condition) {
    throw std::system_error(errno, std::system_category(), what_arg);
  }
}


}

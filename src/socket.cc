#include "socket.hh"

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <fmt/format.h>

namespace tsumu {

socket::~socket() {
  if (close(_fd) == -1) {
    std::cerr << fmt::format("Error closing socket: {0}", strerror(errno)) << std::endl;
  }
}

int socket::configure(int level, int option_name, void *option_value,
   socklen_t option_len) noexcept {
  if (setsockopt(_fd, level, option_name,
        &option_value, sizeof(option_value)) == -1) {
    return errno;
  }

  return 0;
}

int socket::bind(const struct sockaddr *addr, socklen_t addrlen) noexcept {
  if (::bind(_fd, addr, addrlen) == -1) {
    return errno;
  }

  return 0;
}

int socket::configure_and_bind_tcp() noexcept {
  int opt = 1;
  if (this->configure(SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    return errno;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(_port);
  if (this->bind((struct sockaddr*)&addr, sizeof(addr)) == -1) {
    return errno;
  }

  _addr = std::shared_ptr<struct sockaddr>((struct sockaddr *)&addr);
  return 0;
}

int socket::listen(int backlog) noexcept {
  if (::listen(_fd, backlog) == -1) {
    return errno;
  }

  return 0;
}

std::tuple<std::unique_ptr<socket>, int>
socket_factory::make_socket(in_port_t port) {
  const int fd = ::socket(_socket_family, _socket_type, _protocol);
  if (fd == -1) {
    return { nullptr, errno };
  }
  int opt;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
       &opt, sizeof(opt)) == -1) {
  }
  return { std::unique_ptr<socket>(new socket(port, fd)), 0 };
}

}

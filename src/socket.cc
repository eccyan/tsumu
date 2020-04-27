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
    return -1;
  }

  return 0;
}

int socket::bind(const struct sockaddr *addr, socklen_t addrlen) noexcept {
  if (::bind(_fd, addr, addrlen) == -1) {
    return -1;
  }

  return 0;
}

int socket::configure_and_bind_tcp() noexcept {
  int opt = 1;
  if (this->configure(SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    return errno;
  }

  try {
    auto addr = std::make_unique<struct sockaddr_in>();
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(_port);
    if (this->bind((struct sockaddr*)addr.get(),
         sizeof(addr.get())) == -1) {
      return errno;
    }

    _addr.reset(
        reinterpret_cast<struct sockaddr *>(std::move(addr).get()));
  } catch(std::bad_alloc e) {
    return -1;
  }

  return 0;
}

int socket::listen(int backlog) noexcept {
  if (::listen(_fd, backlog) == -1) {
    return errno;
  }

  return 0;
}

int socket::accept() noexcept {
  socklen_t addrlen = sizeof(*_addr.get());
  int server_socket_fd = ::accept(_fd, (struct sockaddr *)_addr.get(), &addrlen);
  if (server_socket_fd == -1) {
    return -1;
  }

  return server_socket_fd;
}

std::tuple<std::unique_ptr<socket>, int>
socket_factory::make_socket(in_port_t port) noexcept {
  const int fd = ::socket(_socket_family, _socket_type, _protocol);
  if (fd == -1) {
    return { nullptr, errno };
  }

  int opt;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
       &opt, sizeof(opt)) == -1) {
  }

  try {
    return { std::unique_ptr<socket>(new socket(port, fd)), 0 };
  } catch(std::bad_alloc e) {
    return { nullptr, -1 };
  }

  return { nullptr, -1 };
}

}

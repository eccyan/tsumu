#pragma once

#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <tuple>
#include <memory>
#include <iostream>
#include <fmt/format.h>

namespace tsumu {

// TODO: Separate socket by meaning
class listing_socket;
class sever_socket;

class socket {
public:
  socket(in_port_t port, int fd) noexcept
    :_port(port), _fd(fd), _addr(nullptr)
  {
  }

  ~socket();

  int fd() noexcept {
    return _fd;
  }

  std::shared_ptr<struct sockaddr> address() noexcept {
    return _addr;
  }

  int configure(int level, int option_name,
      void *option_value, socklen_t option_len) noexcept;

  int bind(const struct sockaddr *addr, socklen_t addrlen) noexcept;

  int configure_and_bind_tcp() noexcept;

  int listen(int backlog) noexcept;

  int accept() noexcept;

private:
  int _fd;
  in_port_t _port;
  std::shared_ptr<struct sockaddr> _addr;
};

class socket_factory {
public:
  socket_factory(int socket_family, int socket_type,
      int protocol) noexcept
    :_socket_family(socket_family), _socket_type(socket_type),
    _protocol(protocol)
  {
  }

  std::tuple<std::unique_ptr<socket>, int>
  make_socket(in_port_t port) noexcept;

private:
  int _socket_family;
  int _socket_type;
  int _protocol;
};


}

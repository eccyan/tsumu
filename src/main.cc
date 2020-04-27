#include "socket.hh"

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <fmt/format.h>

int main(int argc, char** argv) {
  int port = 8080;
  if (argc > 1) {
    try {
      port = std::stoi(argv[1]);
    }
    catch (const std::exception& e) {
      std::cerr << fmt::format("Error port: {0}, {1}", argv[1], e.what()) << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  std::cout << "Welcome to tsumu !" << std::endl;

  auto socket_factory = std::unique_ptr<tsumu::socket_factory>(new tsumu::socket_factory(AF_INET, SOCK_STREAM, 0));
  auto [ sock, err ] = socket_factory->make_socket(port);

  sock->configure_and_bind_tcp();
  sock->listen(3);

  const int socket = sock->accept();
  if (socket == -1)
  {
    std::cerr << fmt::format("Error accept socket: {0}", strerror(errno)) << std::endl;
    exit(EXIT_FAILURE);
  }

  char readbuf[1024] = {0};
  const int read_size = read(socket, readbuf, 1024);
  if (read_size == -1)
  {
    std::cerr << fmt::format("Error read socket: {0}", strerror(errno)) << std::endl;
    exit(EXIT_FAILURE);
  }
  const std::string recv_message = readbuf;
  std::cerr << fmt::format("Received: {0}", recv_message) << std::endl;

  const std::string send_message = fmt::format("You say '{0}'", recv_message);
  send(socket, send_message.c_str(), send_message.size(), 0);
  std::cerr << fmt::format("Sent message: {0}", send_message) << std::endl;

  return 0;
}

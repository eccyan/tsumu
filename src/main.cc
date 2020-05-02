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

  int opt = 1;
  struct ::sockaddr_in in = {};
  in.sin_family = AF_INET;
  in.sin_addr.s_addr = INADDR_ANY;
  in.sin_port = htons(port);
  tsumu::socket_address addr = tsumu::socket_address(in);
  auto server_sock = tsumu::socket::make(AF_INET, SOCK_STREAM, 0)
    .setsockopt(SOL_SOCKET, SO_REUSEADDR, opt)
    .bind(addr, addr.length)
    .listen(3)
    .accept(addr, addr.length);

  char readbuf[1024] = {0};
  server_sock.read(readbuf, 1024);
  std::cerr << fmt::format("Received: {0}", readbuf) << std::endl;

  const std::string send_message = fmt::format("You say '{0}'", readbuf);
  server_sock.send(send_message.c_str(), send_message.size(), 0);
  std::cerr << fmt::format("Sent message: {0}", send_message) << std::endl;

  return 0;
}

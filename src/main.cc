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
    port = std::stoi(argv[0]);
  }
  std::cout << "Welcome to tsumu !" << std::endl;

  const int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    std::cerr << fmt::format("Error creating new socket: {0}", strerror(errno)) << std::endl;
    exit(EXIT_FAILURE);
  }

  int opt = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
  {
    std::cerr << fmt::format("Error setting socket: {0}", strerror(errno)) << std::endl;
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
  if (bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1)
  {
    std::cerr << fmt::format("Error bind socket: {0}", strerror(errno)) << std::endl;
    exit(EXIT_FAILURE);
  }
  if (listen(fd, 3) == -1)
  {
    std::cerr << fmt::format("Error listen socket: {0}", strerror(errno)) << std::endl;
    exit(EXIT_FAILURE);
  }


  int addrlen = sizeof(address);
  const int socket = accept(fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
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
  std::cerr << format("Received: {0}", recv_message) << std::endl;

  const std::string send_message = fmt::format("You say '{0}'", recv_message);
  send(socket, send_message.c_str(), send_message.size(), 0);
  std::cerr << fmt::format("Sent message: {0}", send_message) << std::endl;

  return 0;
}

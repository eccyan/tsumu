#include <iostream>
#include <string>
#include <fmt/format.h>

int main(int argc, char** argv) {
  std::string who = "Unknown";
  if (argc > 1) {
    who = argv[1];
  }
  std::cout << fmt::format("Welcome to tsumu, {0} !", who) << std::endl;
  return 0;
}

#include <iostream>
#include <string>

int main(int argc, char** argv) {
  std::string who = "Unknown";
  if (argc > 1) {
    who = argv[1];
  }
  std::cout << "Welcome to tsumu, " + who + " !" << std::endl;
  return 0;
}

#include "fwd_chaining.h"

int main(int argc, char* argv[]) {

  std::ifstream input;
  FwdChaining machine(input, argv[1]);

  return 0;
}
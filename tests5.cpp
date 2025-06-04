#include "numero.h"
#include <iomanip>

int main() {
  //Store double to memory and get its byte representation
  double x = 43232.1813;
  DoubleStore obj1;
  obj1.value = x;
  unsigned char byte_rep[sizeof(double)];
  memcpy(byte_rep, obj1.byte_rep, sizeof(double));
  std::deque<bool> bit_rep = ByteToBinaryDouble(byte_rep);
  std::deque<bool> int_dq = DoubleToIntBinary(bit_rep);
  int i = 0;
  while (i < int_dq.size()) {
    std::cout << int_dq[i];
    i += 1;
  };
  std::cout << "\n";
  return 0;
}



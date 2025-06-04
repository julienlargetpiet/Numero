#include "numero.h"
#include <iomanip>

int main() {
  //Store double to memory and get its byte representation
  double x = 43232.181342;
  DoubleStore obj1;
  obj1.value = x;
  unsigned char byte_rep[sizeof(double)];
  memcpy(byte_rep, obj1.byte_rep, sizeof(double));
  std::deque<bool> bit_rep = ByteToBinaryDouble(byte_rep);
  double x2 = 23232.181342;
  DoubleStore obj2;
  obj2.value = x2;
  unsigned char byte_rep2[sizeof(double)];
  memcpy(byte_rep2, obj2.byte_rep, sizeof(double));
  std::deque<bool> bit_rep2 = ByteToBinaryDouble(byte_rep2);
  int i = 0;
  while (i < bit_rep.size()) {
    std::cout << bit_rep[i];
    i += 1;
  };
  std::cout << "\n";
  i = 0;
  while (i < bit_rep2.size()) {
    std::cout << bit_rep2[i];
    i += 1;
  };
  std::cout << "\n";
  ////
  bool test = AreEqualDouble(bit_rep, bit_rep2);
  std::cout << test << "\n";
  test = IsSuperiorDouble(bit_rep, bit_rep2);
  std::cout << test << "\n";
  return 0;
}



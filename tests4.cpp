#include "numero.h"
#include <iomanip>

int main() {
  //Store double to memory and get its byte representation
  float x = 43232.1813;
  FloatStore obj1;
  obj1.value = x;
  unsigned char byte_rep[sizeof(float)];
  memcpy(byte_rep, obj1.byte_rep, sizeof(float));
  std::deque<bool> bit_rep = ByteToBinaryFloat(byte_rep);
  float x2 = -123232.112;
  FloatStore obj2;
  obj2.value = x2;
  unsigned char byte_rep2[sizeof(float)];
  memcpy(byte_rep2, obj2.byte_rep, sizeof(float));
  std::deque<bool> bit_rep2 = ByteToBinaryFloat(byte_rep2);
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
  bool test = AreEqualFloat(bit_rep, bit_rep2);
  std::cout << test << "\n";
  test = IsSuperiorFloat(bit_rep, bit_rep2);
  std::cout << test << "\n";
  return 0;
}



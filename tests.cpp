#include "numero.h"
#include <iomanip>

int main() {
  int i;
  //Store float to memory and get its byte representation
  float x = 43.13;
  FloatStore obj1;
  obj1.value = x;
  unsigned char byte_rep[sizeof(float)];
  memcpy(byte_rep, obj1.byte_rep, sizeof(float));
  ////
  //Convert to binary format, default to IEEE754
  std::deque<bool> binary_rep = ByteToBinaryFloat(byte_rep);
  i = 0;
  while (i < binary_rep.size()) {
    std::cout << binary_rep[i];
    i += 1;
  };
  std::cout << "\n";
  ////
  //Convert binary representation (IEEE754) to double
  float rslt = IEEE754ToFloat(binary_rep);
  std::cout << std::setprecision(9);
  std::cout << rslt << "\n";
  ////
  //Convert double to IEE754 binary representation
  rslt = 833.726;
  binary_rep = FloatToIEEE754(rslt);
  i = 0;
  while (i < binary_rep.size()) {
    std::cout << binary_rep[i];
    i += 1;
  };
  std::cout << "\n";
  ////
  //Convert to float
  rslt = IEEE754ToFloat(binary_rep);
  std::cout << rslt << "\n";
  ////
  return 0;
}



#include "numero.h"

int main() {
  unsigned char rslt_arr[sizeof(double)];
  DoubleStore obj1;
  obj1.value = 127;
  std::cout << "value 1: " << obj1.value << "\n";
  memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
  std::deque<bool> dq1 = ByteToBinaryDouble(rslt_arr);
  int i = 0;
  while (i < 64) {
    std::cout << dq1[i];
    i += 1;
  };
  std::cout << "\n";
  DoubleStore obj2;
  obj2.value = -141;
  std::cout << "value 2: " << obj2.value << "\n";
  memcpy(rslt_arr, obj2.byte_rep, sizeof(double));
  std::deque<bool> dq2 = ByteToBinaryDouble(rslt_arr);
  i = 0;
  while (i < 64) {
    std::cout << dq2[i];
    i += 1;
  };
  std::cout << "\n";
  obj2.value = obj1.value + obj2.value;
  std::cout << "intended_result: " << obj2.value << "\n";
  memcpy(rslt_arr, obj2.byte_rep, sizeof(double));
  std::deque<bool> dq3 = ByteToBinaryDouble(rslt_arr);
  i = 0;
  while (i < 64) {
    std::cout << dq3[i];
    i += 1;
  };
  std::cout << "\n";
  dq3 = IEEE754DoubleToDoubleAddition(dq1, dq2);
  std::cout << "dq3 size: " << dq3.size() << "\n";
  i = 0;
  while (i < dq3.size()) {
    std::cout << dq3[i];
    i += 1;
  };
  std::cout << "\n";
  BinaryToByteDouble(dq3, rslt_arr);
  memcpy(obj2.byte_rep, rslt_arr, sizeof(double));
  std::cout << "actual result: " << obj2.value << "\n";
  return 0;
}




#include "numero.h"

int main() {
  int i;
  unsigned char rslt_arr[sizeof(int)];
  IntStore obj1;
  obj1.x = 2;
  memcpy(rslt_arr, obj1.x_array, sizeof(int));
  std::deque<bool> dq1 = ByteToBinaryInt(rslt_arr);
  IntStore obj2;
  obj2.x = -1232;
  memcpy(rslt_arr, obj2.x_array, sizeof(int));
  std::deque<bool> dq2 = ByteToBinaryInt(rslt_arr);
  i = 0;
  while (i < 32) {
    std::cout << dq1[i];
    i += 1;
  };
  std::cout << "\n";
  i = 0;
  while (i < 32) {
    std::cout << dq2[i];
    i += 1;
  };
  std::cout << "\n";
  std::deque<bool> dq3;
  int rslt = obj1.x + obj2.x;
  std::cout << "intended result: " << rslt << "\n";
  dq3 = AbstractionIntSameTypeAddition(dq1, dq2);
  //dq3[22] = 0;
  i = 0;
  while (i < 32) {
    std::cout << dq3[i];
    i += 1;
  };
  std::cout << "\n";
  i = 0;
  IntStore obj3;
  BinaryToByteInt(dq3, rslt_arr);
  memcpy(obj3.x_array, rslt_arr, sizeof(int));
  std::cout << obj3.x << "\n";
  obj3.x = rslt;
  memcpy(rslt_arr, obj3.x_array, sizeof(int));
  dq3 = ByteToBinaryInt(rslt_arr);
  i = 0;
  while (i < 32) {
    std::cout << dq3[i];
    i += 1;
  };
  std::cout << "\n";

  //dq3[6] = 0;
  //i = 0;
  //while (i < 32) {
  //  std::cout << dq3[i];
  //  i += 1;
  //};
  //std::cout << "\n";

  //BinaryToByteInt(dq3, rslt_arr);
  //memcpy(obj3.x_array, rslt_arr, sizeof(int));
  //std::cout << obj3.x << "\n";

  return 0;
}




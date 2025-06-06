#include "numero.h"

int main() {
  IntStore obj1;
  obj1.x = -289;
  unsigned char byte_rep[sizeof(int)];
  memcpy(byte_rep, obj1.x_array, sizeof(int));
  std::deque<bool> bit_rep = ByteToBinaryInt(byte_rep);
  int i = 0;
  while (i < bit_rep.size()) {
    std::cout << bit_rep[i];
    i += 1;
  };
  std::cout << "\n";
  std::deque<bool> flipped_rep = FlippBinaryNegativeInt(bit_rep);
  i = 0;
  while (i < flipped_rep.size()) {
    std::cout << flipped_rep[i];
    i += 1;
  };
  std::cout << "\n";
  //std::deque<bool>::iterator it = bit_rep.begin() + 1;
  //*it = 1;
  //i = 0;
  //while (i < bit_rep.size()) {
  //  std::cout << bit_rep[i];
  //  i += 1;
  //};
  //std::cout << "\n";
  std::deque<bool> dq1 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1}; //161
  std::deque<bool> dq2 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1}; //87
  std::deque<bool> dq3 = {1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0};
  std::deque<bool> dq4 = ReverseFlippBinaryNegativeInt(dq3);
  i = 0;
  std::cout << "dq3: ";
  while (i < dq3.size()) {
    std::cout << dq3[i];
    i += 1;
  };
  std::cout << "\n";
  i = 0;
  std::cout << "dq4: ";
  while (i < dq4.size()) {
    std::cout << dq4[i];
    i += 1;
  };
  std::cout << "\n";
  std::deque<bool> dq5 = FlippBinaryNegativeInt(dq4);
  i = 0;
  std::cout << "dq5: ";
  while (i < dq5.size()) {
    std::cout << dq5[i];
    i += 1;
  };
  std::cout << "\n";
  unsigned char rslt_arr[sizeof(int)];
  BinaryToByteInt(dq4, rslt_arr);
  IntStore obj3;
  memcpy(obj3.x_array, rslt_arr, sizeof(int));
  std::cout << "val: " << obj3.x << "\n";
  memcpy(rslt_arr, obj3.x_array, sizeof(int));
  i = 0;
  while (i < sizeof(int)) {
    std::cout << rslt_arr[i];
    i += 1;
  };
  std::cout << "\n";
  dq5 = ByteToBinaryInt(rslt_arr);
  i = 0;
  while (i < 32) {
    std::cout << dq5[i];
    i += 1;
  };
  std::cout << "\n";
  dq4 = FlippBinaryNegativeInt(dq5);
  i = 0;
  while (i < 32) {
    std::cout << dq4[i];
    i += 1;
  };
  std::cout << "\n";

  //IntSameTypeAddition(bit_rep, flipped_rep);
  //i = 0;
  //while (i < bit_rep.size()) {
  //  std::cout << bit_rep[i];
  //  i += 1;
  //};
  //std::cout << "\n";
  return 0;
}




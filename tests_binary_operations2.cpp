#include "numero.h"

void IntCPUSameTypeAddition(std::deque<bool> &x, std::deque<bool> &x2, int i_stop) {
  int i = 31;
  int i_bf;
  std::cout << "i_stop: " << i_stop << "\n";
  x[i_stop] = 0;

  int i2 = 0;
  while (i2 < 32) {
    std::cout << x[i2];
    i2 += 1;
  };
  std::cout << "\n";

  while (i > i_stop) {
    if (x[i] == 0 && x2[i] == 1) {
      x[i] = 1;
    } else if (x[i] == 1 && x2[i] == 1) {
      x[i] = 0;
      i_bf = i - 1;
      if (i_bf == 0) {
        return;
      };
      while (x[i_bf] != 0) {
        x[i_bf] = 0;
        i_bf -= 1;
        if (i_bf == 0) {
          return;
        };
      };
      x[i_bf] = 1;
    };
    i -= 1;
  };
  return;
};

std::deque<bool> AbstractionIntSameTypeAddition(std::deque<bool> x, std::deque<bool> x2) {
  int i = 1;
  if (x[0] == 1 && x2[0] == 0) {
    while (i < 32) {
      if (x[i] == 0 && x2[i] == 0) {
        std::cout << "here1A\n";
        IntCPUSameTypeAddition(x, x2, i);
        return x;
      } else if (x[i] == 1 && x2[i] == 1) {
        std::cout << "here1B\n";
        IntCPUSameTypeAddition(x2, x, i);
        return x2;
      } else if (x[i] == 0 && x2[i] == 1) {
        //std::cout << "here1C\n";
        x[i] = 1;
        x2[i] = 0;
      };
      i += 1;
    };
  } else if (x[0] == 0 && x2[0] == 1) {
    while (i < 32) {
      if (x[i] == 0 && x2[i] == 0) {
        std::cout << i << " here2A\n";
        IntCPUSameTypeAddition(x2, x, i);
        return x2;
      } else if (x[i] == 1 && x2[i] == 1) {
        std::cout << i << " here2B\n";
        IntCPUSameTypeAddition(x, x2, i);
        return x;
      } else if (x[i] == 1 && x2[i] == 0) {
        //std::cout << "laaa here2C\n";
        x[i] = 0;
        x2[i] = 1;
      };
      i += 1;
    };
  };
  if (x[0] == 0) {
    IntSameTypeAddition(x, x2);
  } else {
    std::cout << "ok\n";
    while (i < 32) {
      if (x[i] == 0 && x2[i] == 1) {
        std::cout << "here4A\n";
        IntCPUSameTypeAddition(x, x2, i - 1);
        return x;
      } else if (x[i] == 1 && x2[i] == 0) {
        std::cout << "here4B\n";
        IntCPUSameTypeAddition(x2, x, i - 1);
        return x2;
      } else if (x[i] == 0 && x2[i] == 0) {
        //std::cout << "here4C\n";
        //i -= 1;
        //IntCPUSameTypeAddition(x2, x, i);
        //return x2;
        x[i] = 1;
        x2[i] = 1;
      };
      i += 1;
    };
  };
  return x;
};

int main() {
  int i;
  unsigned char rslt_arr[sizeof(int)];
  IntStore obj1;
  obj1.x = -1025;
  memcpy(rslt_arr, obj1.x_array, sizeof(int));
  std::deque<bool> dq1 = ByteToBinaryInt(rslt_arr);
  IntStore obj2;
  obj2.x = -1024;
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
  return 0;
}




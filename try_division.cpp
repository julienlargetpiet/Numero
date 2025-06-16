#include "numero.h"
#include <iomanip>

bool IsSuperiorBaseOrEq(std::deque<bool> &x, std::deque<bool> &x2) {
  if (x.size() > x2.size()) {
    return 1;
  } else if (x.size() < x2.size()) {
    return 0;
  };
  for (int i = 0; i < 32; i++) {
    if (x[i] == 1 && x2[i] == 0) {
      return 1;
    } else if (x[i] == 0 && x2[i] == 1) {
      return 0;
    };
  };
  return 1;
};

std::deque<bool> FloatDivide(std::deque<bool> x, std::deque<bool> x2) {

  int ref_cnt;

  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> divider_mantissa_dq = {};
  std::deque<bool> divided_mantissa_dq = {};
  std::deque<bool> mantissa_dq = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<bool> cur_val_dq = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  std::deque<bool> cur_mantissa_dq = {};
  std::deque<bool> cur_divided_mantissa_dq = {};
  std::deque<bool> exponent_dq = {};
  std::deque<bool> divider_mantissa_dq2 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  bool is_greater = IsSuperiorFloat(x, x2);
  int n;
  int n2;
  int i;
  for (i = 9; i < 32; i++) {
    divided_mantissa_dq.push_back(x[i]);
    divider_mantissa_dq.push_back(x2[i]);
  };

  std::cout << "divided_mantissa_dq:\n";
  for (int i2 = 0; i2 < 23; i2++) {
    std::cout << divided_mantissa_dq[i2];
  };
  std::cout << "\n";
  std::cout << divided_mantissa_dq.size() << " divided_mantissa_dq size:\n";
  std::cout << "divider_mantissa_dq:\n";
  for (int i2 = 0; i2 < divider_mantissa_dq.size(); i2++) {
    std::cout << divider_mantissa_dq[i2];
  };
  std::cout << "\n";
  std::cout << divider_mantissa_dq.size() << " divider_mantissa_dq size:\n";
  std::cout << divider_mantissa_dq2.size() << " divider_mantissa_dq2 size:\n";

  if (is_greater) {
    is_greater = 0;

    for (int i2 = 0; i2 < mantissa_dq.size(); i2++) {
      std::cout << mantissa_dq[i2];
    };
    std::cout << "\n";
    std::cout << mantissa_dq.size() << "\n";


    while (!is_greater) {
      IntSameTypeAddition2(divider_mantissa_dq2, divider_mantissa_dq);
      IntSameTypeAddition2(mantissa_dq, cur_val_dq);
      is_greater = IsSuperiorBaseOrEq(divider_mantissa_dq2, divided_mantissa_dq);
    };
    if (divider_mantissa_dq2 != divided_mantissa_dq) {
      IntSameTypeSubstraction2(mantissa_dq, cur_val_dq);
      while (divider_mantissa_dq.size() < divider_mantissa_dq2.size()) {
        divider_mantissa_dq.push_front(0);
      };
      IntSameTypeSubstraction2(divider_mantissa_dq2, divider_mantissa_dq);
      while (divider_mantissa_dq.size() > 23) {
        divider_mantissa_dq.pop_front();
      };

      for (int i2 = 0; i2 < 23; i2++) {
        std::cout << divider_mantissa_dq2[i2];
      };
      std::cout << " ICI\n";

      IntSameTypeSubstraction2(divided_mantissa_dq, divider_mantissa_dq2);
      while (!divided_mantissa_dq[0]) {
        divided_mantissa_dq.pop_front();
      };
    };

    for (n = 0; n < 23; n++) {
      if (mantissa_dq[n]) {
        break;
      };
    };
    n2 = 22 - n + 127;
    exponent_dq = int_to_binarydq(n2);
    if (exponent_dq.size() > 8) {
      return mantissa_dq;
    };
    while (exponent_dq.size() < 8) {
      exponent_dq.push_front(0);
    };
    for (int i2 = 0; i2 < 23; i2++) {
      std::cout << divider_mantissa_dq[i2];
    };
    std::cout << " divider_mantissa_dq\n";
    while (1) {
      std::cout << "###########\n";
      for (int i2 = 0; i2 < divided_mantissa_dq.size(); i2++) {
        std::cout << divided_mantissa_dq[i2];
      };
      std::cout << " dividedA\n";
      std::cout << divided_mantissa_dq.size() << "\n";

      if (mantissa_dq.size() > 23) {
        break;
      };
      is_greater = IsSuperiorBaseOrEq(divided_mantissa_dq, divider_mantissa_dq);
      while (!is_greater) {
        std::cout << 0 << " ref_cnt\n";
        cur_divided_mantissa_dq = divided_mantissa_dq;
        for (i = 0; i < 9; i++) {
          IntSameTypeAddition2(divided_mantissa_dq, cur_divided_mantissa_dq);
          //IntSameTypeSubstraction2(mantissa_dq, cur_val_dq);
          if (divided_mantissa_dq.size() > cur_divided_mantissa_dq.size()) {
            cur_divided_mantissa_dq.push_front(0);
          };
        };
        is_greater = IsSuperiorBaseOrEq(divided_mantissa_dq, divider_mantissa_dq);
      };
      cur_mantissa_dq = mantissa_dq;
      for (i = 0; i < 9; i++) {
        IntSameTypeAddition2(mantissa_dq, cur_mantissa_dq);
        if (mantissa_dq.size() > 23) {
          break;
        };
      };

      if (mantissa_dq.size() > 23) {
        break;
      };
      divider_mantissa_dq2 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      while (divider_mantissa_dq2.size() < divided_mantissa_dq.size()) {
        divider_mantissa_dq2.push_back(0);
        divider_mantissa_dq.push_front(0);
      };
      is_greater = 0;

      ref_cnt = 0;

      for (int i2 = 0; i2 < divided_mantissa_dq.size(); i2++) {
        std::cout << divided_mantissa_dq[i2];
      };
      std::cout << " dividedB\n";
      std::cout << divided_mantissa_dq.size() << "\n";

      while (!is_greater) {
        IntSameTypeAddition2(divider_mantissa_dq2, divider_mantissa_dq);
        IntSameTypeAddition2(mantissa_dq, cur_val_dq);
        is_greater = IsSuperiorBaseOrEq(divider_mantissa_dq2, divided_mantissa_dq);
        ref_cnt += 1;
      };
 
      if (divider_mantissa_dq2 != divided_mantissa_dq) {
        IntSameTypeSubstraction2(mantissa_dq, cur_val_dq);
        while (divider_mantissa_dq.size() < divider_mantissa_dq2.size()) {
          divider_mantissa_dq.push_front(0);
        };
        IntSameTypeSubstraction2(divider_mantissa_dq2, divider_mantissa_dq);
        while (divider_mantissa_dq.size() > 23) {
          divider_mantissa_dq.pop_front();
        };

        for (int i2 = 0; i2 < divider_mantissa_dq2.size(); i2++) {
          std::cout << divider_mantissa_dq2[i2];
        };
        std::cout << "\n";
        std::cout << divider_mantissa_dq2.size() << " divider_mantissa_dq2\n";

        IntSameTypeSubstraction2(divided_mantissa_dq, divider_mantissa_dq2);
        while (!divided_mantissa_dq[0]) {
          divided_mantissa_dq.pop_front();
        };
      } else {
        while (divider_mantissa_dq.size() > 23) {
          divider_mantissa_dq.pop_front();
        };
      };

      std::cout << ref_cnt - 1 << " ref_cnt B\n";

    };
    mantissa_dq.pop_front();
  } else {
    
  };
  rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
  rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
  std::cout << "END\n";
  return rtn_dq;
};

int main() {
  int i;
  unsigned char rslt_arr[sizeof(float)];
  FloatStore obj1;
  obj1.value = 48.5;
  memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
  std::deque<bool> dq = ByteToBinaryFloat(rslt_arr);
  for (i = 0; i < sizeof(float) * 8; i++) {
    std::cout << dq[i];
  };
  std::cout << "\n";
  FloatStore obj2;
  obj2.value = 48.985;
  memcpy(rslt_arr, obj2.byte_rep, sizeof(float));
  std::deque<bool> dq2 = ByteToBinaryFloat(rslt_arr);
  for (i = 0; i < sizeof(float) * 8; i++) {
    std::cout << dq2[i];
  };
  std::cout << "\n";
  std::deque<bool> dq3 = FloatDivide(dq2, dq);
  float intended_rslt = obj2.value / obj1.value;
  std::cout << "intended_result: " << intended_rslt << "\n intended_dq:\n";
  obj1.value = intended_rslt;
  memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
  dq = ByteToBinaryFloat(rslt_arr);
  for (i = 0; i < dq.size(); i++) {
    std::cout << dq[i];
  };
  std::cout << "\n";
  BinaryToByteFloat(dq3, rslt_arr);
  memcpy(obj1.byte_rep, rslt_arr, sizeof(float));
  std::cout << "dq3 size: " << dq3.size() << "\n";
  for (i = 0; i < dq3.size(); i++) {
    std::cout << dq3[i];
  };
  std::cout << "\n";
  std::cout << obj1.value << "\n";
  return 0;
};




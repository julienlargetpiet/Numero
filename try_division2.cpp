#include "numero.h"
#include <iomanip>

std::deque<bool> IEEE754FloatToFloatSubstraction2(std::deque<bool> x, std::deque<bool> x2) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> mantissa_dq = {1};
  std::deque<bool> mantissa_dq2 = {1};
  std::deque<bool> exponent_dq = {};
  std::deque<bool> exponent_dq2 = {};
  std::deque<bool> just_one = {0, 0, 0, 0, 0, 0, 0, 1};
  std::deque<bool> ref_zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i = 1;
  int delta;
  int pre_delta;
  while (i < 9) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  delta = binarydq2_to_int(exponent_dq);
  i = 1;
  while (i < 9) {
    exponent_dq2.push_back(x2[i]);
    i += 1;
  };
  pre_delta = binarydq2_to_int(exponent_dq2);
  delta -= pre_delta;
  pre_delta = delta;
  std::cout << i << " Ici\n";
  if (ref_zero == x) {
    return x2;
  } else if (ref_zero == x2) {
    return x;
  } else if (x[0] != x2[0]) {
    std::cout << delta << " A\n";
    rtn_dq.push_back(x[0]);
    if (delta < 0) {
      while (delta < 0) {
        mantissa_dq.push_front(0);
        mantissa_dq2.push_back(x2[i]);
        delta += 1;
        i += 1;
      };
      while (i < 32) {
        mantissa_dq.push_back(x[i + pre_delta]);
        mantissa_dq2.push_back(x2[i]);
        i += 1; 
      };
      IntSameTypeAddition2(mantissa_dq, mantissa_dq2);
      mantissa_dq.pop_front();
      while (mantissa_dq.size() > 23) {
        IntSameTypeAddition2(exponent_dq2, just_one);
        mantissa_dq.pop_back();
      };
      rtn_dq.insert(rtn_dq.end(), exponent_dq2.begin(), exponent_dq2.end());
    } else {
      std::cout << delta << " A2\n";
      while (delta > 0) {
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_front(0);
        delta -= 1;
        i += 1;
      };
      std::cout << "A2\n";
      while (i < 32) {
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_back(x2[i - pre_delta]);
        i += 1; 
      };
      IntSameTypeAddition2(mantissa_dq, mantissa_dq2);
      mantissa_dq.pop_front();
      while (mantissa_dq.size() > 23) {
        IntSameTypeAddition2(exponent_dq, just_one);
        mantissa_dq.pop_back();
      };
      rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
    };
    rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
  } else {
    std::cout << "ici\n";
    if (delta < 0) {
      rtn_dq.push_back(!x[0]);
      while (delta < 0) {
        mantissa_dq.push_front(0);
        mantissa_dq2.push_back(x2[i]);
        delta += 1;
        i += 1;
      };
      while (i < 32) {
        mantissa_dq.push_back(x[i + pre_delta]);
        mantissa_dq2.push_back(x2[i]);
        i += 1; 
      };
      IntSameTypeSubstraction2(mantissa_dq2, mantissa_dq);
      while (mantissa_dq2[0] == 0) {
        mantissa_dq2.pop_front();
      };
      mantissa_dq2.pop_front();
      while (mantissa_dq2.size() < 23) {
        IntSameTypeSubstraction2(exponent_dq2, just_one);
        while (exponent_dq2.size() < 8) {
          exponent_dq2.push_front(0);
        };
        mantissa_dq2.push_back(0);
      };
      rtn_dq.insert(rtn_dq.end(), exponent_dq2.begin(), exponent_dq2.end());
      rtn_dq.insert(rtn_dq.end(), mantissa_dq2.begin(), mantissa_dq2.end());
    } else if (delta > 0) {
      rtn_dq.push_back(x[0]);
      while (delta > 0) {
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_front(0);
        delta -= 1;
        i += 1;
      };
      while (i < 32) {
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_back(x2[i - pre_delta]);
        i += 1; 
      };
      IntSameTypeSubstraction2(mantissa_dq, mantissa_dq2);
      while (mantissa_dq[0] == 0) {
        mantissa_dq.pop_front();
      };
      mantissa_dq.pop_front();
      while (mantissa_dq.size() < 23) {
        IntSameTypeSubstraction2(exponent_dq, just_one);
        while (exponent_dq.size() < 8) {
          exponent_dq.push_front(0);
        };
        mantissa_dq.push_back(0);
      };
      rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
      rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
    } else {
      while (i < 32) {
        if (x[i] == 1 && x2[i] == 0) {
          rtn_dq.push_back(x[0]);
          while (i < 32) {
            mantissa_dq.push_back(x[i]);
            mantissa_dq2.push_back(x2[i]);
            i += 1; 
          };
          IntSameTypeSubstraction2(mantissa_dq, mantissa_dq2);
          while (mantissa_dq[0] == 0) {
            mantissa_dq.pop_front();
          };
          mantissa_dq.pop_front();
          while (mantissa_dq.size() < 23) {
            IntSameTypeSubstraction2(exponent_dq, just_one);
            while (exponent_dq.size() < 8) {
              exponent_dq.push_front(0);
            };
            mantissa_dq.push_back(0);
          };
          rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
          rtn_dq.insert(rtn_dq.end(), 
                        mantissa_dq.begin(), 
                        mantissa_dq.end());
          return rtn_dq;
        } else if (x[i] == 0 && x2[i] == 1) {
          rtn_dq.push_back(!x2[0]);
          while (i < 32) {
            mantissa_dq.push_back(x[i]);
            mantissa_dq2.push_back(x2[i]);
            i += 1; 
          };
          IntSameTypeSubstraction2(mantissa_dq2, mantissa_dq);
          while (mantissa_dq2[0] == 0) {
            mantissa_dq2.pop_front();
          };
          mantissa_dq2.pop_front();
          while (mantissa_dq2.size() < 23) {
            IntSameTypeSubstraction2(exponent_dq2, just_one);
            while (exponent_dq2.size() < 8) {
              exponent_dq2.push_front(0);
            };
            mantissa_dq2.push_back(0);
          };
          rtn_dq.insert(rtn_dq.end(), exponent_dq2.begin(), exponent_dq2.end());
          rtn_dq.insert(rtn_dq.end(), 
                        mantissa_dq2.begin(), 
                        mantissa_dq2.end());
          return rtn_dq;
        };
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_back(x2[i]);
        i += 1;
      };
      return ref_zero;
    };
  };
  std::cout << "END\n";
  return rtn_dq;
};

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
  std::deque<bool> rtn_dq = {};
  if (x[0] == x2[0]) {
    rtn_dq.push_back(0);
  } else {
    rtn_dq.push_back(1);
  };
  int i = 0;
  int i2;
  int exponent;
  int rest_iterations = 0;
  std::deque<bool> ref_zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<bool> exponent_dq = {};
  std::deque<bool> cur_float = x2;
  std::deque<bool> mantissa_dq = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<bool> just_one = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  std::deque<bool> just_one2 = {0, 0, 0, 0, 0, 0, 0, 1};
  bool is_greater = IsSuperiorFloat(cur_float, x);
  double decimal_counter = 0;
  while (!is_greater) {
    cur_float = IEEE754FloatToFloatAddition(cur_float, x2);
    IntSameTypeAddition2(mantissa_dq, just_one);
    is_greater = IsSuperiorFloat(cur_float, x);
  };
  if (cur_float != x) {
    cur_float = IEEE754FloatToFloatSubstraction(cur_float, x2);
    x = IEEE754FloatToFloatSubstraction(x, cur_float);
  };
  i = 0;

  for (i2 = 0; i2 < 23; i2++) {
    std::cout << mantissa_dq[i2];
  };
  std::cout << "\n";

  if (mantissa_dq != ref_zero) {
    std::cout << "A\n";
    while (!mantissa_dq[0]) {
      i += 1;
      if (i == 23) {
        break;
      };
      mantissa_dq.pop_front();
    };
    std::cout <<"i: " << i << "\n";
    mantissa_dq.pop_front();
    exponent = 22 - i + 127;
    exponent_dq = int_to_binarydq(exponent);
  } else {
    std::cout << "B\n";
    is_greater = 0;
    cur_float = x;
    rest_iterations = 0;
    while (!is_greater) {
      std::cout << "loop\n";
      for (i2 = 0; i2 < cur_float.size(); i2++) {
        std::cout << cur_float[i2];
      };
      std::cout << "\n " << cur_float.size() << "\n";

      cur_float = IEEE754FloatToFloatAddition(cur_float, cur_float);
      is_greater = IsSuperiorFloatOrEq(cur_float, x2); 
      rest_iterations += 1;
    };
    exponent = 127 - rest_iterations;
    exponent_dq = int_to_binarydq(exponent);
    i = 22;
  };
  while (exponent_dq.size() < 8) {
    exponent_dq.push_front(0);
  };
  while (mantissa_dq.size() < 23) {
    mantissa_dq.push_back(0);
  };
  rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());

  std::cout << "mantissa_dq:\n";
  for (i2 = 0; i2 < 23; i2++) {
    std::cout << mantissa_dq[i2];
  };
  std::cout << "\n";

  std::cout << "x:\n";
  for (i2 = 0; i2 < 32; i2++) {
    std::cout << x[i2];
  };
  std::cout << "\n";

  i = 22 - i;

  int hmn = 0;

  std::cout << "strt iterations: " << i << "\n";
  while (i < 23) {
    cur_float = x;
    for (i2 = 0; i2 < 9; i2++) {
      x = IEEE754FloatToFloatAddition(x, cur_float);
    };
    cur_float = x2;
    is_greater = IsSuperiorFloatOrEq(cur_float, x);
    if (!is_greater) {
    while (!is_greater) {
      hmn += 1;
      cur_float = IEEE754FloatToFloatAddition(cur_float, x2);
      IntSameTypeAddition2(mantissa_dq, just_one);
      is_greater = IsSuperiorFloatOrEq(cur_float, x);
    };
    
    if (cur_float != x) {
      cur_float = IEEE754FloatToFloatSubstraction(cur_float, x2);
      x = IEEE754FloatToFloatSubstraction2(x, cur_float);
    } else {
      break;
    };
    };
    i += 1;
    std::cout << "hmn: " << hmn << " i: " << i << "\n";
    hmn = 0;
  };
  rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
  std::cout << "END\n";
  return rtn_dq;
};

int main() {
  int i;
  unsigned char rslt_arr[sizeof(float)];
  FloatStore obj1;
  obj1.value = 20.17;
  memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
  std::deque<bool> dq = ByteToBinaryFloat(rslt_arr);
  for (i = 0; i < sizeof(float) * 8; i++) {
    std::cout << dq[i];
  };
  std::cout << "\n";
  FloatStore obj2;
  obj2.value = 9000;
  std::cout << std::setprecision(9) << "\n";
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




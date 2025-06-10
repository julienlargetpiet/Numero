#include "numero.h"

std::deque<bool> IEEE754FloatToFloatSubstraction(std::deque<bool> x, std::deque<bool> x2) {
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
  if (ref_zero == x) {
    return x2;
  } else if (ref_zero == x2) {
    return x;
  } else if (x[0] != x2[0]) {
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
  return rtn_dq;
};

int main() {
  unsigned char rslt_arr[sizeof(float)];
  FloatStore obj1;
  obj1.value = -214.27;
  std::cout << "value 1: " << obj1.value << "\n";
  memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
  std::deque<bool> dq1 = ByteToBinaryFloat(rslt_arr);
  int i = 0;
  while (i < 32) {
    std::cout << dq1[i];
    i += 1;
  };
  std::cout << "\n";
  FloatStore obj2;
  obj2.value = -214.67;
  std::cout << "value 2: " << obj2.value << "\n";
  memcpy(rslt_arr, obj2.byte_rep, sizeof(float));
  std::deque<bool> dq2 = ByteToBinaryFloat(rslt_arr);
  i = 0;
  while (i < 32) {
    std::cout << dq2[i];
    i += 1;
  };
  std::cout << "\n";
  obj2.value = obj1.value - obj2.value;
  std::cout << "intended_result: " << obj2.value << "\n";
  memcpy(rslt_arr, obj2.byte_rep, sizeof(float));
  std::deque<bool> dq3 = ByteToBinaryFloat(rslt_arr);
  i = 0;
  while (i < 32) {
    std::cout << dq3[i];
    i += 1;
  };
  std::cout << "\n";
  dq3 = IEEE754FloatToFloatSubstraction(dq1, dq2);
  std::cout << "dq3 size: " << dq3.size() << "\n";
  i = 0;
  while (i < dq3.size()) {
    std::cout << dq3[i];
    i += 1;
  };
  std::cout << "\n";
  BinaryToByteFloat(dq3, rslt_arr);
  memcpy(obj2.byte_rep, rslt_arr, sizeof(float));
  std::cout << "actual result: " << obj2.value << "\n";
  return 0;
}




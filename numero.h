#include <iostream>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdint>

//@T int_to_binarydq
//@U std::deque&lt;bool&gt; int_to_binarydq(unsigned int x)
//@X
//@D Converts an unsigned int to a binary format as a boolean deque
//@A x : is the input unsigned int
//@X
//@E std::deque&lt;bool&gt; rtn_dq  = int_to_binarydq(1286);
//@E 1 0 1 0 0 0 0 0 1 1 0
//@X

std::deque<bool> int_to_binarydq(unsigned int x) {
  std::deque<bool> rtn_dq = {};
  while (x > 0) {
    if (x % 2 == 0) {
      rtn_dq.push_front(0);
    } else {
      rtn_dq.push_front(1);
    };
    x /= 2;
  };
  return rtn_dq;
};

std::deque<bool> int_to_binarydq2(uint64_t x) {
  std::deque<bool> rtn_dq = {};
  while (x > 0) {
    if (x % 2 == 0) {
      rtn_dq.push_front(0);
    } else {
      rtn_dq.push_front(1);
    };
    x /= 2;
  };
  return rtn_dq;
};

//@T binarydq_to_int
//@U unsigned int binarydq_to_int(std::deque&lt;bool&gt; &x)
//@X
//@D Converts a binary format as a boolean deque to an unsigned int
//@A x : is the input boolean std deque
//@X
//@E std::deque&lt;bool&gt; dq_input = {1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0};
//@E unsigned int out = binarydq_to_int(rtn_dq);
//@E 1286
//@X

unsigned int binarydq_to_int(std::deque<bool> &x) {
  unsigned int rtn_int = 0;
  std::deque<bool> ref_dq = {};
  const unsigned int ref_n = x.size() - 1;
  ref_dq.resize(ref_n + 1, 0);
  int i;
  while (ref_dq != x) {
    i = ref_n;
    while (ref_dq[i] == 1) {
      ref_dq[i] = 0;
      i -= 1;
    };
    ref_dq[i] = 1;
    rtn_int += 1;
  };
  return rtn_int;
};

//@T FloatStore
//@U union FloatStore {
//@U   float value;
//@U   unsigned char byte_rep[sizeof(float)];
//@U };
//@X
//@D A simple union for a float representation and its byte representation according to the IEEE754 standard
//@A no args : no def
//@X
//@E float x = 43.13;
//@E FloatStore obj1;
//@E obj1.value = x;
//@X

union FloatStore {
  float value;
  unsigned char byte_rep[sizeof(float)];
};

//@T IEEE754ToFloat
//@U float IEEE754ToFloat(std::deque&lt;bool&gt; &binary_rep)
//@X
//@D Converts a IEE754 standardized binary representation of a single value precision decimal to a float.
//@A x : is the input binary representation
//@X
//@E int i;
//@E //Store float to memory and get its byte representation
//@E float x = 43.13;
//@E FloatStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(float)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(float));
//@E ////
//@E //Convert to binary format, default to IEEE754
//@E std::deque&lt;bool&gt; binary_rep = ByteToBinaryFloat(byte_rep);
//@E i = 0;
//@E while (i &lt; binary_rep.size()) {
//@E   std::cout &lt;&lt; binary_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E ////
//@E //Convert binary representation (IEEE754) to double
//@E float rslt = IEEE754ToFloat(binary_rep);
//@E std::cout &lt;&lt; std::setprecision(9);
//@E std::cout &lt;&lt; rslt &lt;&lt; "\n";
//@X

float IEEE754ToFloat(std::deque<bool> &binary_rep) {
  float x = 0;
  int i = 1;
  std::deque<bool> exponent_dq = {};
  double divider = 2;
  double multiplicator = 1;
  if (binary_rep[0] == 1) {
    multiplicator = -1;
  };
  while (i < 9) {
    exponent_dq.push_back(binary_rep[i]);
    i += 1;
  };
  int exponent_vl = binarydq_to_int(exponent_dq);
  exponent_vl -= 127;
  float mantissa_vl = 0;
  while (i < 32) {
    mantissa_vl += (binary_rep[i] / divider);
    divider *= 2;
    i += 1;
  };
  mantissa_vl += 1;
  i = 0;
  while (i < exponent_vl) {
    mantissa_vl *= 2;
    i += 1;
  };
  x = multiplicator * mantissa_vl;
  return x;
}

//@T FloatToIEEE754
//@U std::deque&lt;bool&gt; FloatToIEEE754(float &x)
//@X
//@D Converts a float to a boolean <code>std::deque</code> representing its binary representation according to the IEEE754 standard for single value precison.
//@A x : is the input float
//@X
//@E float rslt = 833.726;
//@E std::deque&lt;bool&gt; binary_rep = FloatToIEEE754(rslt);
//@E int i = 0;
//@E while (i &lt; binary_rep.size()) {
//@E   std::cout &lt;&lt; binary_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 01000100010100000110111001110110
//@X

std::deque<bool> FloatToIEEE754(float &x) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> cur_dq = {};
  int int_part = int(x);
  double dec_part;
  rtn_dq = int_to_binarydq(int_part);
  int i = 0;
  dec_part = x - int_part;
  int n = rtn_dq.size();
  rtn_dq.pop_front();
  i = 0;
  while (i < 24 - n) {
    dec_part *= 2;
    if (dec_part > 1) {
      rtn_dq.push_back(1);
      dec_part -= 1;
    } else {
      rtn_dq.push_back(0);
    };
    i += 1;
  };
  n += 126;
  cur_dq = int_to_binarydq(n);
  while (cur_dq.size() < 8) {
    cur_dq.push_front(0);
  };
  for (i = cur_dq.size() - 1; i >= 0; i--) {
    rtn_dq.push_front(cur_dq[i]);
  };
  if (x >= 0) {
    rtn_dq.push_front(0);
  } else {
    rtn_dq.push_front(1);
  };
  return rtn_dq;
}

//@T ByteToBinaryFloat
//@U std:deque&lt;bool&gt; ByteToBinaryFloat(unsigned char (&byte_rep)[sizeof(float)])
//@X
//@D Converts a byte representation of a float to its binary representation
//@A x : is the input byte array
//@X
//@E float x = 43.13;
//@E FloatStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(float)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; binary_rep = ByteToBinaryFloat(byte_rep);
//@E i = 0;
//@E while (i &lt; binary_rep.size()) {
//@E   std::cout &lt;&lt; binary_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 01000010001011001000010100011111
//@X

std::deque<bool> ByteToBinaryFloat(unsigned char (&byte_rep)[sizeof(float)]) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> cur_dq = {};
  int cur_int;
  int i;
  for (i = sizeof(float) - 1; i >= 0; i--) {
    cur_int = int(byte_rep[i]);
    cur_dq = int_to_binarydq(cur_int);
    while (cur_dq.size() < 8) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
  };
  return rtn_dq;
};

//@T DoubleStore
//@U union DoubleStore {
//@U   double value;
//@U   unsigned char byte_rep[sizeof(double)];
//@U };
//@X
//@D A simple union for a double representation and its byte representation according to the IEEE754 standard
//@A no args : no def
//@X
//@E double x = 43.13;
//@E DoubleStore obj1;
//@E obj1.value = x;
//@X

union DoubleStore {
  double value;
  unsigned char byte_rep[sizeof(double)];
};

//@T IEEE754ToDouble
//@U float IEEE754ToDouble(std::deque&lt;bool&gt; &binary_rep)
//@X
//@D Converts a IEE754 standardized binary representation of a double value precision decimal to a double.
//@A x : is the input binary representation
//@X
//@E int i;
//@E //Store float to memory and get its byte representation
//@E double x = 43.13;
//@E DoubleStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(double)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(double));
//@E ////
//@E //Convert to binary format, default to IEEE754
//@E std::deque&lt;bool&gt; binary_rep = ByteToBinaryDouble(byte_rep);
//@E i = 0;
//@E while (i &lt; binary_rep.size()) {
//@E   std::cout &lt;&lt; binary_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E ////
//@E //Convert binary representation (IEEE754) to double
//@E float rslt = IEEE754ToDouble(binary_rep);
//@E std::cout &lt;&lt; std::setprecision(9);
//@E std::cout &lt;&lt; rslt &lt;&lt; "\n";
//@X

double IEEE754ToDouble(std::deque<bool> &binary_rep) {
  double x = 0;
  int i = 1;
  std::deque<bool> exponent_dq = {};
  double divider = 2;
  double multiplicator = 1;
  if (binary_rep[0] == 1) {
    multiplicator = -1;
  };
  while (i < 12) {
    exponent_dq.push_back(binary_rep[i]);
    i += 1;
  };
  int exponent_vl = binarydq_to_int(exponent_dq);
  exponent_vl -= 1023;
  double mantissa_vl = 0;
  while (i < 64) {
    mantissa_vl += (binary_rep[i] / divider);
    divider *= 2;
    i += 1;
  };
  mantissa_vl += 1;
  i = 0;
  while (i < exponent_vl) {
    mantissa_vl *= 2;
    i += 1;
  };
  x = multiplicator * mantissa_vl;
  return x;
}

//@T DoubleToIEEE754
//@U std::deque&lt;bool&gt; DoubleToIEEE754(float &x)
//@X
//@D Converts a double to a boolean <code>std::deque</code> representing its binary representation according to the IEEE754 standard for double value precison.
//@A x : is the input double
//@X
//@E double rslt = 833.726;
//@E std::deque&lt;bool&gt; binary_rep = DoubleToIEEE754(rslt);
//@E int i = 0;
//@E while (i &lt; binary_rep.size()) {
//@E   std::cout &lt;&lt; binary_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000010001010000011011100111011011001000101101000011100101010
//@X

std::deque<bool> DoubleToIEEE754(double &x) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> cur_dq = {};
  int int_part = int(x);
  double dec_part;
  rtn_dq = int_to_binarydq(int_part);
  int i = 0;
  dec_part = x - int_part;
  int n = rtn_dq.size();
  rtn_dq.pop_front();
  i = 0;
  while (i < 53 - n) {
    dec_part *= 2;
    if (dec_part > 1) {
      rtn_dq.push_back(1);
      dec_part -= 1;
    } else {
      rtn_dq.push_back(0);
    };
    i += 1;
  };
  n += 1022;
  cur_dq = int_to_binarydq(n);
  while (cur_dq.size() < 11) {
    cur_dq.push_front(0);
  };
  for (i = cur_dq.size() - 1; i >= 0; i--) {
    rtn_dq.push_front(cur_dq[i]);
  };
  if (x >= 0) {
    rtn_dq.push_front(0);
  } else {
    rtn_dq.push_front(1);
  };
  return rtn_dq;
}

//@T ByteToBinaryDouble
//@U std:deque&lt;bool&gt; ByteToBinaryDouble(unsigned char (&byte_rep)[sizeof(double)])
//@X
//@D Converts a byte representation of a double to its binary representation
//@A x : is the input byte array
//@X
//@E double x = 43.13;
//@E DoubleStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(double)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; binary_rep = ByteToBinaryDouble(byte_rep);
//@E i = 0;
//@E while (i &lt; binary_rep.size()) {
//@E   std::cout &lt;&lt; binary_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000001111011000000101110010101100000010000011000100100110111
//@X

std::deque<bool> ByteToBinaryDouble(unsigned char (&byte_rep)[sizeof(double)]) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> cur_dq = {};
  int cur_int;
  int i;
  for (i = sizeof(double) - 1; i >= 0; i--) {
    cur_int = int(byte_rep[i]);
    cur_dq = int_to_binarydq(cur_int);
    while (cur_dq.size() < 8) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
  };
  return rtn_dq;
};



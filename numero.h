#include <iostream>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdint>

//@I C++ library emulating addition and substraction of signed int, float and double at binary level

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

//@T binarydq2_to_int
//@U unsigned int binarydq2_to_int(std::deque&lt;bool&gt; &x)
//@X
//@D Converts a binary format as a boolean deque to an unsigned int, contrary to <code>binarydq_to_int()</code> it does not try to emulate an int (from binary) which increase a lot its speed.
//@A x : is the input boolean std deque
//@X
//@E std::deque&lt;bool&gt; dq_input = {1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0};
//@E unsigned int out = binarydq2_to_int(rtn_dq);
//@E 1286
//@X

int binarydq2_to_int(std::deque<bool> &x) {
  int rtn_int = 0;
  int power_vl = 1;
  for (int i = x.size() - 1; i >= 0; i--) {
    rtn_int += (x[i] * power_vl);
    power_vl *= 2;
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
  int exponent_vl = binarydq2_to_int(exponent_dq);
  double exponent_vl2 = 1;
  exponent_vl -= 127;
  float mantissa_vl = 0;
  while (i < 32) {
    mantissa_vl += (binary_rep[i] / divider);
    divider *= 2;
    i += 1;
  };
  mantissa_vl += 1;
  i = 0;
  if (exponent_vl > 0) {
    while (i < exponent_vl) {
      mantissa_vl *= 2;
      i += 1;
    };
  } else {
    while (i > exponent_vl) {
      exponent_vl2 *= 2;
      i -= 1;
    };
    mantissa_vl *= (1 / exponent_vl2);
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
  int i = 0;
  int n = 0;
  int nf;
  if (int_part < 0) {
    int_part *= -1;
    rtn_dq = int_to_binarydq(int_part);
    dec_part = x + int_part;
    dec_part *= -1;
  } else {
    rtn_dq = int_to_binarydq(int_part);
    dec_part = x - int_part;
  };
  if (int_part != 0) {
    n = rtn_dq.size();
    nf = n;
    rtn_dq.pop_front();
  } else {
    while (n > -23) {
      dec_part *= 2;
      if (dec_part > 1) {
        dec_part -= 1;
        break;
      };
      n -= 1;
    };
    nf = 1;
  };
  i = 0;
  while (i < 24 - nf) {
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
//@A x : is the input byte array (little endian encoding assumed)
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

//@T AreEqualFloat
//@U bool AreEqualFloat(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Returns a boolean, if the binary (IEEE754) representation of 2 differents float are equal it will return 1, 0 either.
//@A x : is the first IEEE754 binary representation of a float, the first double
//@A x : is the second IEEE754 binary representation of a float, the second double
//@X
//@E //Store double to memory and get its byte representation
//@E float x = 43232.1813;
//@E FloatStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(float)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryFloat(byte_rep);
//@E float x2 = 123232.112;
//@E FloatStore obj2;
//@E obj2.value = x2;
//@E unsigned char byte_rep2[sizeof(float)];
//@E memcpy(byte_rep2, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; bit_rep2 = ByteToBinaryFloat(byte_rep2);
//@E int i = 0;
//@E while (i &lt; bit_rep.size()) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E i = 0;
//@E while (i &lt; bit_rep2.size()) {
//@E   std::cout &lt;&lt; bit_rep2[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E ////
//@E bool test = AreEqualFloat(bit_rep, bit_rep2);
//@E std::cout &lt;&lt; test &lt;&lt; "\n";
//@X

bool AreEqualFloat(std::deque<bool> &x, std::deque<bool> &x2) {
  unsigned int n = x.size();
  if (n != x2.size()) {
    std::cout << "Error: not comparing same types\n";
    return 0;
  };
  if (n != 32) {
    std::cout << "Error: one or more is not a float\n";
  };
  for (int i = 0; i < 64; i++) {
    if (x[i] != x2[i]) {
      return 0;
    };
  };
  return 1;
};

//@T IsSuperiorFloat
//@U bool IsSuperiorFloat(std::deque&lt;bool&gt; &x1, std::deque&lt;bool&gt; &x2)
//@X
//D Returns a boolean, if the first float is superior (its IEEE754 binary representation) it will return a 1, 0 either.
//@A x : is the first IEEE754 binary representation of a float, the first float
//@A x : is the second IEEE754 binary representation of a float, the second float
//@X
//@E //Store double to memory and get its byte representation
//@E float x = 43232.1813;
//@E FloatStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(float)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);
//@E float x2 = 23232.18;
//@E FoatStore obj2;
//@E obj2.value = x2;
//@E unsigned char byte_rep2[sizeof(float)];
//@E memcpy(byte_rep2, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; bit_rep2 = ByteToBinaryDouble(byte_rep2);
//@E int i = 0;
//@E while (i &lt; bit_rep.size()) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E i = 0;
//@E while (i &lt; bit_rep2.size()) {
//@E   std::cout &lt;&lt; bit_rep2[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E ////
//@E bool test = IsSuperiorFloat(bit_rep, bit_rep2);
//@E std::cout &lt;&lt; test &lt;&lt; "\n";
//@E 1
//@X

bool IsSuperiorFloat(std::deque<bool> &x1, std::deque<bool> &x2) {
  unsigned int n = x1.size();
  std::deque<bool> exponent1_dq = {};
  std::deque<bool> exponent2_dq = {};
  std::deque<bool> mantissa1_dq = {};
  std::deque<bool> mantissa2_dq = {};
  bool is_negative1 = 0;
  bool is_negative2 = 0;
  int val1;
  int val2;
  int i = 1;
  if (n != 32) {
    std::cout << "Error: first arg is not a float binary representation";
    return 0;
  };
  if (x2.size() != 32) {
    std::cout << "Error: second arg is not a float binary representation";
    return 0;
  };
  if (x1[0] == 1) {
    is_negative1 = 1;
    if (x2[0] == 0) {
      return 0;
    };
  };
  if (x2[0] == 1) {
    is_negative2 = 1;
    if (!is_negative1) {
      return 1;
    };
  };  
  while (i < 32) {
    if (x1[i] && !x2[i]) {
      if (is_negative1) {
        return 0;
      } else {
        return 1;
      };
    } else if (!x1[i] && x2[i]) {
      if (is_negative1) {
        return 1;
      } else {
        return 0;
      };
    };
    i += 1;
  };
  return 0; 
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
  int exponent_vl = binarydq2_to_int(exponent_dq);
  double exponent_vl2 = 1;
  exponent_vl -= 1023;
  double mantissa_vl = 0;
  while (i < 64) {
    mantissa_vl += (binary_rep[i] / divider);
    divider *= 2;
    i += 1;
  };
  mantissa_vl += 1;
  i = 0;
  if (exponent_vl > 0) {
    while (i < exponent_vl) {
      mantissa_vl *= 2;
      i += 1;
    };
  } else {
    while (i > exponent_vl) {
      exponent_vl2 *= 2;
      i -= 1;
    };
    mantissa_vl *= (1 / exponent_vl2);
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
  int i = 0;
  int n = 0;
  int nf;
  if (x < 0) {
    int_part *= -1;
    rtn_dq = int_to_binarydq(int_part);
    dec_part = x + int_part;
    dec_part *= -1;
  } else {
    rtn_dq = int_to_binarydq(int_part);
    dec_part = x - int_part;
  };
  if (int_part != 0) {
    n = rtn_dq.size();
    rtn_dq.pop_front();
    nf = n;
  } else {
    while (n > -52) {
      dec_part *= 2;
      if (dec_part > 1) {
        dec_part -= 1;
        break;
      };
      n -= 1;
    };
    nf = 1;
  };
  while (i < 53 - nf) {
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
//@D Converts a byte representation of a double to its binary representation (little endian encoding assumed)
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

//@T AreEqualDouble
//@U bool AreEqualDouble(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Returns a boolean, if the binary (IEEE754) representation of 2 differents double are equal it will return 1, 0 either.
//@A x : is the first IEEE754 binary representation of a double, the first double
//@A x : is the second IEEE754 binary representation of a double, the second double
//@X
//@E //Store double to memory and get its byte representation
//@E double x = 43232.181342;
//@E DoubleStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(double)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);
//@E double x2 = 23232.181342;
//@E DoubleStore obj2;
//@E obj2.value = x2;
//@E unsigned char byte_rep2[sizeof(double)];
//@E memcpy(byte_rep2, obj2.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; bit_rep2 = ByteToBinaryDouble(byte_rep2);
//@E int i = 0;
//@E while (i &lt; bit_rep.size()) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E i = 0;
//@E while (i &lt; bit_rep2.size()) {
//@E   std::cout &lt;&lt; bit_rep2[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E ////
//@E bool test = AreEqualDouble(bit_rep, bit_rep2);
//@E std::cout &lt;&lt; test &lt;&lt; "\n";
//@E 0
//@X

bool AreEqualDouble(std::deque<bool> &x, std::deque<bool> &x2) {
  unsigned int n = x.size();
  if (n != x2.size()) {
    std::cout << "Error: not comparing same types\n";
    return 0;
  };
  if (n != 64) {
    std::cout << "Error: one or more is not a double\n";
  };
  for (int i = 0; i < 64; i++) {
    if (x[i] != x2[i]) {
      return 0;
    };
  };
  return 1;
};

//@T IsSuperiorDouble
//@U bool IsSuperiorDouble(std::deque&lt;bool&gt; &x1, std::deque&lt;bool&gt; &x2)
//@X
//D Returns a boolean, if the first double is superior (its IEEE754 binary representation) it will return a 1, 0 either.
//@A x : is the first IEEE754 binary representation of a double, the first double
//@A x : is the second IEEE754 binary representation of a double, the second double
//@X
//@E //Store double to memory and get its byte representation
//@E double x = 43232.181342;
//@E DoubleStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(double)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);
//@E double x2 = 23232.181342;
//@E DoubleStore obj2;
//@E obj2.value = x2;
//@E unsigned char byte_rep2[sizeof(double)];
//@E memcpy(byte_rep2, obj2.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; bit_rep2 = ByteToBinaryDouble(byte_rep2);
//@E int i = 0;
//@E while (i &lt; bit_rep.size()) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E i = 0;
//@E while (i &lt; bit_rep2.size()) {
//@E   std::cout &lt;&lt; bit_rep2[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@E ////
//@E bool test = IsSuperiorDouble(bit_rep, bit_rep2);
//@E std::cout &lt;&lt; test &lt;&lt; "\n";
//@E 1
//@X

bool IsSuperiorDouble(std::deque<bool> &x1, std::deque<bool> &x2) {
  unsigned int n = x1.size();
  std::deque<bool> exponent1_dq = {};
  std::deque<bool> exponent2_dq = {};
  std::deque<bool> mantissa1_dq = {};
  std::deque<bool> mantissa2_dq = {};
  bool is_negative1 = 0;
  bool is_negative2 = 0;
  int val1;
  int val2;
  int i = 1;
  if (n != 64) {
    std::cout << "Error: first arg is not a double binary representation";
    return 0;
  };
  if (x2.size() != 64) {
    std::cout << "Error: second arg is not a double binary representation";
    return 0;
  };
  if (x1[0] == 1) {
    is_negative1 = 1;
    if (x2[0] == 0) {
      return 0;
    };
  };
  if (x2[0] == 1) {
    is_negative2 = 1;
    if (!is_negative1) {
      return 1;
    };
  };  
  while (i < 64) {
    if (x1[i] && !x2[i]) {
      if (is_negative1) {
        return 0;
      } else {
        return 1;
      };
    } else if (!x1[i] && x2[i]) {
      if (is_negative1) {
        return 1;
      } else {
        return 0;
      };
    };
    i += 1;
  };
  return 0; 
};

bool IsSuperiorDoubleOrEq(std::deque<bool> &x1, std::deque<bool> &x2) {
  unsigned int n = x1.size();
  std::deque<bool> exponent1_dq = {};
  std::deque<bool> exponent2_dq = {};
  std::deque<bool> mantissa1_dq = {};
  std::deque<bool> mantissa2_dq = {};
  bool is_negative1 = 0;
  bool is_negative2 = 0;
  int val1;
  int val2;
  int i = 1;
  if (n != 64) {
    std::cout << "Error: first arg is not a double binary representation";
    return 0;
  };
  if (x2.size() != 64) {
    std::cout << "Error: second arg is not a double binary representation";
    return 0;
  };
  if (x1[0] == 1) {
    is_negative1 = 1;
    if (x2[0] == 0) {
      return 0;
    };
  };
  if (x2[0] == 1) {
    is_negative2 = 1;
    if (!is_negative1) {
      return 1;
    };
  };  
  while (i < 64)  {
    if (x1[i] && !x2[i]) {
      if (is_negative1) {
        return 0;
      } else {
        return 1;
      };
    } else if (!x1[i] && x2[i]) {
      if (is_negative1) {
        return 1;
      } else {
        return 0;
      };
    };
    i += 1;
  };
  return 1; 
};

//@T FloatToIntBinary
//@U std::deque&lt;bool&gt; FloatToIntBinary(std::deque&lt;bool&gt; &x)
//@X
//@D Converts the IEEE754 standard binary representation of a float to an int (32 bits)
//@A x : is the input binary representation as a boolean deque
//@X 
//@E FloatStore obj2;
//@E obj2.value = -145.67;
//@E unsigned char byte_rep[sizeof(float)];
//@E memcpy(byte_rep, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryFloat(byte_rep);
//@E int i;
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11000011000100011010101110000101
//@E std::deque&lt;bool&gt; xfloatdq = FloatToIntBinary(bit_rep);
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; xfloatdq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 10000000000000000000000010010001
//@X

std::deque<bool> FloatToIntBinary(std::deque<bool> &x) {
  int i = 1;
  const int n = sizeof(int) * 8;
  std::deque<bool> exponent_dq = {};
  std::deque<bool> rtn_dq = {};
  int shift_over;
  while (i < 9) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  shift_over = binarydq2_to_int(exponent_dq);
  shift_over -= 127;
  if (shift_over < 0) {
    i = 0;
    while (i < 32) {
      rtn_dq.push_back(0);
      i+= 1;
    };
    return rtn_dq;
  } else if (shift_over > 31) {
    i = 0;
    while (i < 32) {
      rtn_dq.push_back(0);
      i+= 1;
    };
    return rtn_dq;
  };
  if (!x[0]) {
    rtn_dq.push_back(1);
    i = 0;
    while (i < shift_over) {
      rtn_dq.push_back(x[i + 9]);
      i += 1;
    };
    while (rtn_dq.size() != 32) {
      rtn_dq.push_front(0);
    };
  } else {
    rtn_dq.push_back(0);
    i = 0;
    while (i < shift_over) {
      rtn_dq.push_back(!x[i + 9]);
      i += 1;
    };
    while (rtn_dq.size() != 32) {
      rtn_dq.push_front(1);
    };
    if (rtn_dq[n - 1] == 0) {
      rtn_dq[n - 1] = 1;
    } else {
      i = n - 1;
      while (rtn_dq[i] == 1) {
        rtn_dq[i] = 0;
        i -= 1;
      };
      rtn_dq[i] = 1;
    };
  };
  return rtn_dq;
};

//@T DoubleToIntBinary
//@U std::deque&lt;bool&gt; DoubleToIntBinary(std::deque&lt;bool&gt; &x)
//@X
//@D Converts the IEEE754 standard binary representation of a double to an int (32 bits)
//@A x : is the input binary representation as a boolean deque
//@X
//@E //Store double to memory and get its byte representation
//@E double x = 43232.1813;
//@E DoubleStore obj1;
//@E obj1.value = x;
//@E unsigned char byte_rep[sizeof(double)];
//@E memcpy(byte_rep, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);
//@E std::deque&lt;bool&gt; int_dq = DoubleToIntBinary(bit_rep);
//@E int i = 0;
//@E while (i &lt; int_dq.size()) {
//@E   std::cout &lt;&lt; int_dq[i];
//@E   i += 1;
//@E };
//@E std::cout &lt;&lt; "\n";
//@X

std::deque<bool> DoubleToIntBinary(std::deque<bool> &x) {
  int i = 1;
  const int n = sizeof(int) * 8;
  std::deque<bool> exponent_dq = {};
  std::deque<bool> rtn_dq = {};
  int shift_over;
  while (i < 12) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  shift_over = binarydq2_to_int(exponent_dq);
  shift_over -= 1023;
  if (shift_over < 0) {
    i = 0;
    while (i < 64) {
      rtn_dq.push_back(0);
      i+= 1;
    };
    return rtn_dq;
  } else if (shift_over > 63) {
    i = 0;
    while (i < 64) {
      rtn_dq.push_back(0);
      i+= 1;
    };
    return rtn_dq;
  };
  if (!x[0]) {
    rtn_dq.push_back(1);
    i = 0;
    while (i < shift_over) {
      rtn_dq.push_back(x[i + 12]);
      i += 1;
    };
    while (rtn_dq.size() != 32) {
      rtn_dq.push_front(0);
    };
  } else {
    rtn_dq.push_back(0);
    i = 0;
    while (i < shift_over) {
      rtn_dq.push_back(!x[i + 12]);
      i += 1;
    };
    while (rtn_dq.size() != 32) {
      rtn_dq.push_front(1);
    };
    if (rtn_dq[n - 1] == 0) {
      rtn_dq[n - 1] = 1;
    } else {
      i = n - 1;
      while (rtn_dq[i] == 1) {
        rtn_dq[i] = 0;
        i -= 1;
      };
      rtn_dq[i] = 1;
    };
  };
  return rtn_dq;
};

//@T IntStore
//@U union IntStore {
//@U    int x;
//@U    unsigned char x_array[sizeof(int)];
//@U  };
//@X
//@D A union to store a int as an int type and as a <code>unsigned char</code> of length <code>sizeof(int)</code> array.
//@A no args : no def
//@X
//@E IntStore obj1;
//@E obj1.x = 56;
//@E int i;
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; obj1.x_array[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@X

union IntStore {
  int x;
  unsigned char x_array[sizeof(int)];
};

//@T IntToFloatBinary
//@U std::deque&lt;bool&gt; IntToFloatBinary(std::deque&lt;bool&gt; &x)
//@X
//@D Converts an int32 binary representation to a IEEE754 float binary representation.
//@A x : is the boolean deque representing the float 
//@X
//@E int i;
//@E FloatStore obj2;
//@E obj2.value = 45;
//@E unsigned char byte_rep[sizeof(float)];
//@E memcpy(byte_rep, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryFloat(byte_rep);
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 01000010001101000000000000000000
//@E int xint = 45;
//@E std::deque&lt;bool&gt; xintdq = int_to_binarydq(xint);
//@E while (xintdq.size() &lt; 32) {
//@E   xintdq.push_front(0);
//@E };
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; xintdq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000000101101
//@E std::deque&lt;bool&gt; xfloatdq = IntToFloatBinary(xintdq);
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; xfloatdq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 01000010001101000000000000000000
//@X

std::deque<bool> IntToFloatBinary(std::deque<bool> &x) {
  int i = 0;
  int ref_i;
  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> cur_dq = {};
  int exponent;
  if (!x[0]) {
    while (i < 32) {
      if (x[i] == 1) {
        break;
      };
      i += 1;
    };
    exponent = 31 - i + 127;
    cur_dq = int_to_binarydq(exponent);
    while (cur_dq.size() != 8) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
    i += 1;
    while (i < 32) {
      rtn_dq.push_back(x[i]);
      i += 1;
    };
    while (rtn_dq.size() != 32) {
      rtn_dq.push_back(0);
    };
  } else {
    while (i < 32) {
      if (x[i] == 0) {
        break;
      };
      i += 1;
    };
    exponent = 31 - i + 127;
    cur_dq = int_to_binarydq(exponent);
    while (cur_dq.size() != 8) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
    i += 1;
    ref_i = i;
    while (i < 32) {
      x[i] = !x[i];
      i += 1;
    };
    if (x[31] == 0) {
      x[31] = 1;
    } else {
      i = 31;
      while (x[i] == 1) {
        x[i] = 0;
        i -= 1;
      };
      x[i] = 1;
    };
    while (ref_i < 32) {
      rtn_dq.push_back(x[ref_i]);
      ref_i += 1;
    };
    while (rtn_dq.size() < 32) {
      rtn_dq.push_back(0);
    };
  };
  return rtn_dq;
};

//@T IntToDoubleBinary
//@U std::deque&lt;bool&gt; IntToDoubleBinary(std::deque&lt;bool&gt; &x)
//@X
//@D Converts an int32 binary representation to a IEEE754 double binary representation.
//@A x : is the boolean deque representing the float 
//@X
//@E int i;
//@E FloatStore obj2;
//@E obj2.value = 4567;
//@E unsigned char byte_rep[sizeof(double)];
//@E memcpy(byte_rep, obj2.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryFloat(byte_rep);
//@E for (i = 0; i &lt; 64; i++) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000010110001110101110000000000000000000000000000000000000000
//@E int xint = 4567;
//@E std::deque&lt;bool&gt; xintdq = int_to_binarydq(xint);
//@E while (xintdq.size() &lt; 32) {
//@E   xintdq.push_front(0);
//@E };
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; xintdq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000001000111010111
//@E std::deque&lt;bool&gt; xfloatdq = IntToDoubleBinary(xintdq);
//@E for (i = 0; i &lt; 64; i++) {
//@E   std::cout &lt;&lt; xfloatdq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000010110001110101110000000000000000000000000000000000000000
//@X

std::deque<bool> IntToDoubleBinary(std::deque<bool> &x) {
  int i = 0;
  int ref_i;
  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> cur_dq = {};
  int exponent;
  if (!x[0]) {
    while (i < 32) {
      if (x[i] == 1) {
        break;
      };
      i += 1;
    };
    exponent = 31 - i + 1023;
    cur_dq = int_to_binarydq(exponent);
    std::cout << cur_dq.size() << "\n";
    while (cur_dq.size() != 11) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
    i += 1;
    while (i < 32) {
      rtn_dq.push_back(x[i]);
      i += 1;
    };
    while (rtn_dq.size() != 64) {
      rtn_dq.push_back(0);
    };
  } else {
    while (i < 32) {
      if (x[i] == 0) {
        break;
      };
      i += 1;
    };
    exponent = 31 - i + 1023;
    cur_dq = int_to_binarydq(exponent);
    while (cur_dq.size() != 11) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
    i += 1;
    ref_i = i;
    while (i < 32) {
      x[i] = !x[i];
      i += 1;
    };
    if (x[31] == 0) {
      x[31] = 1;
    } else {
      i = 31;
      while (x[i] == 1) {
        x[i] = 0;
        i -= 1;
      };
      x[i] = 1;
    };
    while (ref_i < 32) {
      rtn_dq.push_back(x[ref_i]);
      ref_i += 1;
    };
    while (rtn_dq.size() < 64) {
      rtn_dq.push_back(0);
    };
  };
  return rtn_dq;
};

//@T FloatToDoubleBinary
//@U std::deque&lt;bool&gt; FloatToDoubleBinary(std::deque&lt;bool&gt; &x)
//@X
//@D Converts a IEEE754 binary representation of a float to an IEEE754 binary representation of a double.
//@A x : is the boolean deque representing the input float
//@X
//@E DoubleStore obj1;
//@E obj1.value = 0.067;
//@E unsigned char byte_rep1[sizeof(double)];
//@E memcpy(byte_rep1, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; bit_rep1 = ByteToBinaryDouble(byte_rep1);
//@E int i;
//@E for (i = 0; i &lt; 64; i++) {
//@E   std::cout &lt;&lt; bit_rep1[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0011111110110001001001101110100101111000110101001111110111110100
//@E double rslt = IEEE754ToDouble(bit_rep1);
//@E std::cout &lt;&lt; "rslt: " &lt;&lt; rslt &lt;&lt; "\n";
//@E 0.067
//@E FloatStore obj2;
//@E obj2.value = 0.067;
//@E unsigned char byte_rep[sizeof(float)];
//@E memcpy(byte_rep, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryFloat(byte_rep);
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00111101100010010011011101001100
//@E std::deque&lt;bool&gt; xdoubledq = FloatToDoubleBinary(bit_rep);
//@E for (i = 0; i &lt; xdoubledq.size(); i++) {
//@E   std::cout &lt;&lt; xdoubledq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E std::cout &lt;&lt; "size: " &lt;&lt; xdoubledq.size() << "\n";
//@E 0011111110110001001001101110100110000000000000000000000000000000
//@E rslt = IEEE754ToDouble(xdoubledq);
//@E std::cout &lt;&lt; "rslt: " &lt;&lt; rslt &lt;&lt; "\n";
//@E 0.067
//@X

std::deque<bool> FloatToDoubleBinary(std::deque<bool> &x) {
  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> exponent_dq = {};
  int exponent_vl;
  int i = 1;
  while (i < 9) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  exponent_vl = binarydq2_to_int(exponent_dq);
  exponent_vl -= 127;
  exponent_vl += 1023;
  exponent_dq = int_to_binarydq(exponent_vl);
  while (exponent_dq.size() < 11) {
    exponent_dq.push_front(0);
  };
  rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
  while (i < 32) {
    rtn_dq.push_back(x[i]);
    i += 1;
  };
  while (rtn_dq.size() != 64) {
    rtn_dq.push_back(0);
  };
  return rtn_dq;
};

//@T DoubleToFloatBinary
//@U std::deque&lt;bool&gt; DoubleToFloatBinary(std::deque&lt;bool&gt; &x)
//@X
//@D Converts a IEEE754 binary representation of a double to an IEEE754 binary representation of a float.
//@A x : is the boolean deque representing the input double
//@X
//@E FloatStore obj1;
//@E obj1.value = 0.067;
//@E unsigned char byte_rep1[sizeof(float)];
//@E memcpy(byte_rep1, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; bit_rep1 = ByteToBinaryFloat(byte_rep1);
//@E int i;
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; bit_rep1[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00111101100010010011011101001100
//@E float rslt = IEEE754ToFloat(bit_rep1);
//@E std::cout &lt;&lt; "rslt: " &lt;&lt; rslt &lt;&lt; "\n";
//@E 0.067
//@E DoubleStore obj2;
//@E obj2.value = 0.067;
//@E unsigned char byte_rep[sizeof(double)];
//@E memcpy(byte_rep, obj2.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);
//@E for (i = 0; i &lt; 64; i++) {
//@E   std::cout &lt;&lt; bit_rep[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0011111110110001001001101110100101111000110101001111110111110100
//@E std::deque&lt;bool&gt; xfloatdq = DoubleToFloatBinary(bit_rep);
//@E for (i = 0; i &lt; xfloatdq.size(); i++) {
//@E   std::cout &lt;&lt; xfloatdq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00111101100010010011011101001011
//@E std::cout &lt;&lt; "size: " &lt;&lt; xfloatdq.size() << "\n";
//@E 32
//@E rslt = IEEE754ToFloat(xfloatdq);
//@E std::cout &lt;&lt; "rslt: " &lt;&lt; rslt &lt;&lt; "\n";
//@E 0.067
//@X

std::deque<bool> DoubleToFloatBinary(std::deque<bool> &x) {
  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> exponent_dq = {};
  int exponent_vl;
  int i = 1;
  while (i < 12) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  exponent_vl = binarydq2_to_int(exponent_dq);
  exponent_vl -= 1023;
  exponent_vl += 127;
  exponent_dq = int_to_binarydq(exponent_vl);
  while (exponent_dq.size() < 8) {
    exponent_dq.push_front(0);
  };
  rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
  while (rtn_dq.size() < 32) {
    rtn_dq.push_back(x[i]);
    i += 1;
  };
  return rtn_dq;
};

//@T ShortIntStore
//@U union ShortIntStore {
//@U  short int x;
//@U  unsigned char x_array[sizeof(short int)];
//@U };
//@X
//@D Simple union storing a short int and its byte representation.
//@A no args : no definition
//@X
//@E ShortIntStore obj1;
//@E obj1.x = 23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(short int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryShortInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(short int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0000000000010111
//@X

union ShortIntStore {
  short int x;
  unsigned char x_array[sizeof(short int)];
};

//@T LongIntStore
//@U union LongIntStore {
//@U  long int x;
//@U  unsigned char x_array[sizeof(long int)];
//@U };
//@X
//@D Simple union storing a long int and its byte representation.
//@A no args : no definition
//@X
//@E LongIntStore obj1;
//@E obj1.x = 23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(long int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryLongInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(short int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000000010111
//@X

union LongIntStore {
  long int x;
  unsigned char x_array[sizeof(long int)];
};

//@T IntToShortIntBinary
//@U std::deque&lt;bool&gt; IntToShortIntBinary(std::deque&lt;bool&gt; x)
//@X
//@D Converts the binary representation of a signed int to the binary representation of an short signed int.
//@A x : is the boolean deque representing the int
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E unsigned char rslt_arr2[sizeof(short int)];
//@E IntStore obj1;
//@E obj1.x = -23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11111111111111111111111111101001
//@E dq = IntToShortIntBinary(dq);
//@E for (i = 0; i &lt; sizeof(short int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1111111111101001
//@E BinaryToByteShortInt(dq, rslt_arr2);
//@E ShortIntStore obj2;
//@E memcpy(obj2.x_array, rslt_arr2, sizeof(short int));
//@E std::cout &lt;&lt; obj2.x &lt;&lt; "\n";
//@E -23
//@X

std::deque<bool> IntToShortIntBinary(std::deque<bool> x) {
  unsigned int n = sizeof(short int) * 8;
  while (x.size() > n) {
    x.pop_front();
  };
  return x;
};

//@T IntToLongIntBinary
//@U std::deque&lt;bool&gt; IntToLongIntBinary(std::deque&lt;bool&gt; x)
//@X
//@D Converts the binary representation of a signed int to the binary representation of an long signed int.
//@A x : is the boolean deque representing the int
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E unsigned char rslt_arr2[sizeof(long int)];
//@E IntStore obj1;
//@E obj1.x = -23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11111111111111111111111111101001
//@E dq = IntToLongIntBinary(dq);
//@E for (i = 0; i &lt; sizeof(long int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0000000000000000000000000000000011111111111111111111111111101001
//@E BinaryToByteLongInt(dq, rslt_arr2);
//@E LongIntStore obj2;
//@E memcpy(obj2.x_array, rslt_arr2, sizeof(long int));
//@E std::cout &lt;&lt; obj2.x &lt;&lt; "\n";
//@E -23
//@X

std::deque<bool> IntToLongIntBinary(std::deque<bool> x) {
  unsigned int n = sizeof(long int) * 8;
  if (x[0] == 0) {
    while (x.size() < n) {
      x.push_front(0);
    };
  } else {
    while (x.size() < n) {
      x.push_front(1);
    };
  };
  return x;
};

//@T LongIntToIntBinary
//@U std::deque&lt;bool&gt; LongIntToIntBinary(std::deque&lt;bool&gt; x)
//@X
//@D Converts the binary representation of a signed long int to the binary representation of a signed int.
//@A x : is the boolean deque representing the long int
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(long int)];
//@E unsigned char rslt_arr2[sizeof(int)];
//@E LongIntStore obj1;
//@E obj1.x = -23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(long int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryLongInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(long int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1111111111111111111111111111111111111111111011010101100110001101
//@E dq = LongIntToIntBinary(dq);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11111111111011010101100110001101
//@E BinaryToByteInt(dq, rslt_arr2);
//@E IntStore obj2;
//@E memcpy(obj2.x_array, rslt_arr2, sizeof(int));
//@E std::cout &lt;&lt; obj2.x &lt;&lt; "\n";
//@E -23
//@X

std::deque<bool> LongIntToIntBinary(std::deque<bool> x) {
  unsigned int n = sizeof(int) * 8;
  x[n] = x[0];
  while (x.size() > n) {
    x.pop_front();
  };
  return x;
};

//@T LongIntToShortIntBinary
//@U std::deque&lt;bool&gt; LongIntToShortIntBinary(std::deque&lt;bool&gt; x)
//@X
//@D Converts the binary representation of a signed long int to the binary representation of a signed short int.
//@A x : is the boolean deque representing the long int
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(long int)];
//@E unsigned char rslt_arr2[sizeof(short int)];
//@E LongIntStore obj1;
//@E obj1.x = 122;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(long int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(long int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0000000000000000000000000000000000000000000000000000000001111010
//@E dq = LongIntToShortIntBinary(dq);
//@E for (i = 0; i &lt; sizeof(short int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0000000001111010
//@E BinaryToByteShortInt(dq, rslt_arr2);
//@E ShortIntStore obj2;
//@E memcpy(obj2.x_array, rslt_arr2, sizeof(short int));
//@E std::cout &lt;&lt; obj2.x &lt;&lt; "\n";
//@E 122
//@X

std::deque<bool> LongIntToShortIntBinary(std::deque<bool> x) {
  unsigned int n = sizeof(short int) * 8;
  x[n] = x[0];
  while (x.size() > n) {
    x.pop_front();
  };
  return x;
};

//@T ShortIntToIntBinary
//@U std::deque&lt;bool&gt; ShortIntToIntBinary(std::deque&lt;bool&gt; x)
//@X
//@D Converts the binary representation of a signed short int to the binary representation of a signed int.
//@A x : is the boolean deque representing the short int
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(short int)];
//@E unsigned char rslt_arr2[sizeof(int)];
//@E ShortIntStore obj1;
//@E obj1.x = 122;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(short int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryShortInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(short int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0000000000010111
//@E dq = ShortIntToIntBinary(dq);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000000010111
//@E BinaryToByteInt(dq, rslt_arr2);
//@E IntStore obj2;
//@E memcpy(obj2.x_array, rslt_arr2, sizeof(int));
//@E std::cout &lt;&lt; obj2.x &lt;&lt; "\n";
//@E 122
//@X

std::deque<bool> ShortIntToIntBinary(std::deque<bool> x) {
  unsigned int n = sizeof(int) * 8;
  if (x[0] == 0) {
    while (x.size() < n) {
      x.push_front(0);
    };
  } else {
    while (x.size() < n) {
      x.push_front(1);
    };
  };
  return x;
};

//@T ShortIntToLongIntBinary
//@U std::deque&lt;bool&gt; ShortIntToLongIntBinary(std::deque&lt;bool&gt; x)
//@X
//@D Converts the binary representation of a signed short int to the binary representation of a signed long int.
//@A x : is the boolean deque representing the short int
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(short int)];
//@E unsigned char rslt_arr2[sizeof(long int)];
//@E ShortIntStore obj1;
//@E obj1.x = -122;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(short int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryShortInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(short int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1111111110000110
//@E dq = ShortIntToLongIntBinary(dq);
//@E for (i = 0; i &lt; sizeof(long int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1111111111111111111111111111111111111111111111111111111110000110
//@E BinaryToByteLongInt(dq, rslt_arr2);
//@E LongIntStore obj2;
//@E memcpy(obj2.x_array, rslt_arr2, sizeof(long int));
//@E std::cout &lt;&lt; obj2.x &lt;&lt; "\n";
//@E -122
//@X

std::deque<bool> ShortIntToLongIntBinary(std::deque<bool> x) {
  unsigned int n = sizeof(long int) * 8;
  if (x[0] == 0) {
    while (x.size() < n) {
      x.push_front(0);
    };
  } else {
    while (x.size() < n) {
      x.push_front(1);
    };
  };
  return x;
};

//@T ByteToBinaryInt
//@U std::deque&lt;bool&gt; ByteToBinaryInt(unsigned char(&x)[sizeof(int)])
//@X
//@D Converts a byte representation of an Int to its binary representation.
//@A x : is the int byte representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000000010111
//@X

std::deque<bool> ByteToBinaryInt(unsigned char(&x)[sizeof(int)]) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> cur_dq = {};
  int cur_int;
  int i;
  for (i = sizeof(int) - 1; i >= 0; i--) {
    cur_int = int(x[i]);
    cur_dq = int_to_binarydq(cur_int);
    while (cur_dq.size() < 8) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
  };
  return rtn_dq;
};

//@T ByteToBinaryShortInt
//@U std::deque&lt;bool&gt; ByteToBinaryShortInt(unsigned char(&x)[sizeof(short int)])
//@X
//@D Converts a byte representation of an Short Int to its binary representation.
//@A x : is the short int byte representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(short int)];
//@E ShortIntStore obj1;
//@E obj1.x = 23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(short int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryShortInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(short int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0000000000010111
//@X

std::deque<bool> ByteToBinaryShortInt(unsigned char(&x)[sizeof(short int)]) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> cur_dq = {};
  int cur_int;
  int i;
  for (i = sizeof(short int) - 1; i >= 0; i--) {
    cur_int = int(x[i]);
    cur_dq = int_to_binarydq(cur_int);
    while (cur_dq.size() < 8) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
  };
  return rtn_dq;
};

//@T ByteToBinaryLongInt
//@U std::deque&lt;bool&gt; ByteToBinaryLongInt(unsigned char(&x)[sizeof(long int)])
//@X
//@D Converts a byte representation of an Long Int to its binary representation.
//@A x : is the long int byte representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(long int)];
//@E LongIntStore obj1;
//@E obj1.x = 23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(long int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryLongInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(long int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0000000000000000000000000000000000000000000000000000000000010111
//@X

std::deque<bool> ByteToBinaryLongInt(unsigned char(&x)[sizeof(long int)]) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> cur_dq = {};
  int cur_int;
  int i;
  for (i = sizeof(long int) - 1; i >= 0; i--) {
    cur_int = int(x[i]);
    cur_dq = int_to_binarydq(cur_int);
    while (cur_dq.size() < 8) {
      cur_dq.push_front(0);
    };
    rtn_dq.insert(rtn_dq.end(), cur_dq.begin(), cur_dq.end());
  };
  return rtn_dq;
};

//@T FlippBinaryNegativeInt
//@U void FlippBinaryNegativeInt(std::deque<bool> &x)
//@X
//@D Converts the binary representation of an int to its opposite binary representation as the 2's complement binary representation.
//@A x : is the input integer binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000000010111
//@E FlippBinaryNegativeInt(dq);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11111111111111111111111111101001
//@E BinaryToByteInt(dq, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E -23
//@X

void FlippBinaryNegativeInt(std::deque<bool> &x) {
  int i;
  for (i = 1; i < sizeof(int) * 8; i++) {
    x[i] = !x[i];
  };
  i = sizeof(int) * 8 - 1;
  while (x[i] != 0) {
    x[i] = 0;
    i -= 1;
  };
  x[i] = 1;
  x[0] = 1;
  return;
};

//@T ReverseFlippBinaryNegativeInt
//@U void ReverseFlippBinaryNegativeInt(std::deque&lt;bool&gt; &x)
//@X
//@D Converts the binary representation of a negative integer to its opposite binary representation.
//@A x : is the negative integer binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 23;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000000010111
//@E FlippBinaryNegativeInt(dq);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11111111111111111111111111101001
//@E BinaryToByteInt(dq, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E -23
//@E ReverseFlippBinaryNegativeInt(dq);
//@E BinaryToByteInt(dq, rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000000010111 
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E 23
//@X

void ReverseFlippBinaryNegativeInt(std::deque<bool> &x) {
  int n = x.size();
  int i = n - 1;
  while (x[i] != 1 && i > 0) {
    x[i] = 1;
    i -= 1;
  };
  x[i] = 0;
  i = 0;
  while  (i < n) {
    x[i] = !x[i];
    i += 1;
  };
  x[0] = 1;
  return;
};

//@T IntSameTypeAddition
//@U void IntSameTypeAddition(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Performs the binary addition algorithm, works for positive integers only.
//@A x : is the first int binary representation
//@A x2 : is the second int binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 223;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000011011111
//@E IntSameTypeAddition(dq, dq);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000110111110
//@E BinaryToByteInt(dq, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E 446
//@X

void IntSameTypeAddition(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool>::iterator it;
  std::deque<bool>::iterator it2 = x2.begin() + 1;
  std::deque<bool>::iterator it_bf;
  for (it = x.begin() + 1; it != x.end(); it++) {
    if (*it == 0 && *it2 == 1) {
      *it = 1;
    } else if (*it == 1 && *it2 == 1) {
      *it = 0;
      it_bf = it - 1;
      if (it_bf == x.begin()) {
        return;
      };
      while (*it_bf != 0) {
        *it_bf = 0;
        it_bf--;
        if (it_bf == x.begin()) {
          return;
        };
      };
      *it_bf = 1;
    };
    it2++;
  };
  return;
};

//@T IntSameTypeSubstraction
//@U void IntSameTypeSubstraction(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Performs the binary substraction algorithm. Works for positive integers only, and if the substracted is higher or equal to the substractor.
//@A x : is the first integer binary representation
//@A x2 : is the second integer binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 223;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000011011111
//@E IntStore obj2;
//@E obj2.x = 89;
//@E memcpy(rslt_arr, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000011011111
//@E IntSameTypeSubstraction(dq, dq2);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000010000110
//@E BinaryToByteInt(dq, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E 134
//@X

void IntSameTypeSubstraction(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool>::iterator it;
  std::deque<bool>::iterator it2 = x2.begin() + 1;
  std::deque<bool>::iterator it_bf;
  for (it = x.begin() + 1; it != x.end(); it++) {
    if (*it == 1 && *it2 == 1) {
      *it = 0;
    } else if (*it == 0 && *it2 == 1) {
      *it = 1;
      it_bf = it - 1;
      if (it_bf == x.begin()) {
        return;
      };
      while (*it_bf != 1) {
        *it_bf = 1;
        it_bf--;
        if (it_bf == x.begin()) {
          return;
        };
      };
      *it_bf = 0;
    };
    it2++;
  };
  return;
};

//@T BinaryToByteInt
//@U void BinaryToByteInt(std::deque&lt;bool&gt; &x, unsigned char (&rtn_arr)[sizeof(int)])
//@X
//@D Converts an integer binary representation to its byte representation.
//@A x : is the binary representation of the integer
//@A rtn_arr : is an unsigned char array of sizeof(int) representing the future byte representation of the integer
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 223;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000011011111
//@E BinaryToByteInt(dq, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E 223
//@X

void BinaryToByteInt(std::deque<bool> &x, unsigned char (&rtn_arr)[sizeof(int)]) {
  std::deque<bool> cur_dq = {};
  int i2;
  int cur_int;
  const int n = sizeof(int);
  for (int i = 0; i < n; i++) {
    cur_dq = {};
    for (i2 = i * 8; i2 < (i + 1) * 8; i2++) {
      cur_dq.push_back(x[i2]);
    };
    cur_int = binarydq2_to_int(cur_dq);
    rtn_arr[n - i - 1] = char(cur_int);
  };
  return;
};

//@T BinaryToByteShortInt
//@U void BinaryToByteShortInt(std::deque&lt;bool&gt; &x, unsigned char (&rtn_arr)[sizeof(short int)])
//@X
//@D Converts an short integer binary representation to its byte representation.
//@A x : is the binary representation of the short integer
//@A rtn_arr : is an unsigned char array of sizeof(short int) representing the future byte representation of the integer
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(short int)];
//@E ShortIntStore obj1;
//@E obj1.x = 223;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(short int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryShortInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(short int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0000000011011111
//@E BinaryToByteShortInt(dq, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(short int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E 223
//@X

void BinaryToByteShortInt(std::deque<bool> &x, unsigned char (&rtn_arr)[sizeof(short int)]) {
  std::deque<bool> cur_dq = {};
  int i2;
  int cur_int;
  const int n = sizeof(short int);
  for (int i = 0; i < n; i++) {
    cur_dq = {};
    for (i2 = i * 8; i2 < (i + 1) * 8; i2++) {
      cur_dq.push_back(x[i2]);
    };
    cur_int = binarydq2_to_int(cur_dq);
    rtn_arr[n - i - 1] = char(cur_int);
  };
  return;
};

//@T BinaryToByteLongInt
//@U void BinaryToByteLongInt(std::deque&lt;bool&gt; &x, unsigned char (&rtn_arr)[sizeof(long int)])
//@X
//@D Converts a long integer binary representation to its byte representation.
//@A x : is the binary representation of the long integer
//@A rtn_arr : is an unsigned char array of sizeof(long int) representing the future byte representation of the integer
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(long int)];
//@E LongIntStore obj1;
//@E obj1.x = -223;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(long int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryLongInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(long int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1111111111111111111111111111111111111111111111111111111100100001
//@E BinaryToByteLongInt(dq, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(long int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E -223
//@X

void BinaryToByteLongInt(std::deque<bool> &x, unsigned char (&rtn_arr)[sizeof(long int)]) {
  std::deque<bool> cur_dq = {};
  int i2;
  int cur_int;
  const int n = sizeof(long int);
  for (int i = 0; i < n; i++) {
    cur_dq = {};
    for (i2 = i * 8; i2 < (i + 1) * 8; i2++) {
      cur_dq.push_back(x[i2]);
    };
    cur_int = binarydq2_to_int(cur_dq);
    rtn_arr[n - i - 1] = char(cur_int);
  };
  return;
};

//@T BinaryToByteDouble
//@U void BinaryToByteDouble(std::deque&lt;bool&gt; &x, unsigned char (&rtn_arr)[sizeof(double)])
//@X
//@D Converts a long integer binary representation to its byte representation.
//@A x : is the binary representation of the long integer
//@A rtn_arr : is an unsigned char array of sizeof(long int) representing the future byte representation of the integer
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(double)];
//@E DoubleStore obj1;
//@E obj1.value = -10223.449;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1100000011000011111101111011100101111000110101001111110111110100
//@E BinaryToByteDouble(dq, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; obj1.value &lt;&lt; "\n";
//@E -1023.4
//@X

void BinaryToByteDouble(std::deque<bool> &x, unsigned char (&rtn_arr)[sizeof(double)]) {
  const int n = sizeof(double);
  std::deque<bool> cur_dq = {};
  int cur_int;
  int i2;
  for (int i = 0; i < n; i++) {
    cur_dq = {};
    for (i2 = i * 8; i2 < (i + 1) * 8; i2++) {
      cur_dq.push_back(x[i2]);
    };
    cur_int = binarydq2_to_int(cur_dq);
    rtn_arr[n - 1 - i] = char(cur_int);
  };
  return;
};

//@T BinaryToByteFloat
//@U void BinaryToByteFloat(std::deque&lt;bool&gt; &x, unsigned char (&rtn_arr)[sizeof(float)])
//@X
//@D Converts a float binary representation to its byte representation.
//@A x : is the binary representation of the float
//@A rtn_arr : is an unsigned char array of sizeof(float) representing the future byte representation of the float
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(float)];
//@E FloatStore obj1;
//@E obj1.value = -223.449;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 01000011010111110111001011110010
//@E BinaryToByteFloat(dq, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(float));
//@E std::cout &lt;&lt; obj1.value &lt;&lt; "\n";
//@E 223.449
//@X

void BinaryToByteFloat(std::deque<bool> &x, unsigned char (&rtn_arr)[sizeof(float)]) {
  const int n = sizeof(float);
  std::deque<bool> cur_dq = {};
  int cur_int;
  int i2;
  for (int i = 0; i < n; i++) {
    cur_dq = {};
    for (i2 = i * 8; i2 < (i + 1) * 8; i2++) {
      cur_dq.push_back(x[i2]);
    };
    cur_int = binarydq2_to_int(cur_dq);
    rtn_arr[n - 1 - i] = char(cur_int);
  };
  return;
};

void IntCPUSameTypeAddition(std::deque<bool> &x, std::deque<bool> &x2, int i_stop) {
  int i = 31;
  int i_bf;
  x[i_stop] = 0;
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

//@T AbstractionIntSameTypeAddition
//@U std::deque&lt;bool&gt; AbstractionIntSameTypeAddition(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Performs the binary addition with direct integers binary representation, including the 2's complement representation for negative integers.
//@A x : is the first integer binary representation
//@A x2 : is the first integer binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 10223;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000010011111101111
//@E IntStore obj2;
//@E obj2.x = -283;
//@E memcpy(rslt_arr, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq2[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11111111111111111111111011100101
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; obj1.x + obj2.x &lt;&lt; "\n";
//@E 9940
//@E std::deque&lt;bool&gt; dq3 = AbstractionIntSameTypeAddition(dq, dq2);
//@E BinaryToByteInt(dq3, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E 9940
//@X

std::deque<bool> AbstractionIntSameTypeAddition(std::deque<bool> x, std::deque<bool> x2) {
  int i = 1;
  if (x[0] == 1 && x2[0] == 0) {
    while (i < 32) {
      if (x[i] == 0 && x2[i] == 0) {
        IntCPUSameTypeAddition(x, x2, i);
        return x;
      } else if (x[i] == 1 && x2[i] == 1) {
        IntCPUSameTypeAddition(x2, x, i);
        return x2;
      } else if (x[i] == 0 && x2[i] == 1) {
        x[i] = 1;
        x2[i] = 0;
      };
      i += 1;
    };
    return x;
  } else if (x[0] == 0 && x2[0] == 1) {
    while (i < 32) {
      if (x[i] == 0 && x2[i] == 0) {
        IntCPUSameTypeAddition(x2, x, i);
        return x2;
      } else if (x[i] == 1 && x2[i] == 1) {
        IntCPUSameTypeAddition(x, x2, i);
        return x;
      } else if (x[i] == 1 && x2[i] == 0) {
        x[i] = 0;
        x2[i] = 1;
      };
      i += 1;
    };
    return x2;
  };
  if (x[0] == 0) {
    IntSameTypeAddition(x, x2);
  } else {
    while (i < 32) {
      if (x[i] == 0 || x2[i] == 0) {
        IntCPUSameTypeAddition(x2, x, i - 1);
        return x2;
      };
      i += 1;
    };
  };
  return x;
};

void IntCPUSameTypeSubstraction(std::deque<bool> &x, std::deque<bool> &x2, int i_stop) {
  int i = 31;
  int i_bf;
  
  while (i > i_stop) {
    if (x[i] == 1 && x2[i] == 1) {
      x[i] = 0;
    } else if (x[i] == 0 && x2[i] == 1) {
      x[i] = 1;
      i_bf = i - 1;
      if (i_bf == 0) {
        return;
      };
      while (x[i_bf] != 1) {
        x[i_bf] = 1;
        i_bf -= 1;
        if (i_bf == 0) {
          return;
        };
      };
      x[i_bf] = 0;
    };
    i -= 1;
  };
  return;
};

//@T AbstractionIntSameTypeSubstraction
//@U std::deque&lt;bool&gt; AbstractionIntSameTypeSubstraction(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Performs the binary substraction  with direct integers binary representation, including the 2's complement representation for negative integers.
//@A x : is the first integer binary representation
//@A x2 : is the first integer binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 224;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000011100000
//@E IntStore obj2;
//@E obj2.x = 283;
//@E memcpy(rslt_arr, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr);
//@E for (i = 0; i &lt; sizeof(int) * 8; i++) {
//@E   std::cout &lt;&lt; dq2[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000000000000000000100011011
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; obj1.x - obj2.x &lt;&lt; "\n";
//@E -59
//@E std::deque&lt;bool&gt; dq3 = AbstractionIntSameTypeSubstraction(dq, dq2);
//@E BinaryToByteInt(dq3, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E -59
//@X

std::deque<bool> AbstractionIntSameTypeSubstraction(std::deque<bool> x, std::deque<bool> x2) {
  int i = 1;
  if (x[0] == 0 && x2[0] == 1) {
    while (i < 32) {
      if (x[i] == 1 || x2[i] == 0) {
        x[i - 1] = 1;
        IntCPUSameTypeSubstraction(x, x2, i - 1);
        return x;
      };
      i += 1;
    };
  } else if (x[0] == 1 && x2[0] == 0) {
   while (i < 32) {
      if (x[i] == 0 || x2[i] == 1) {
        x[i - 1] = 1;
        IntCPUSameTypeSubstraction(x, x2, i - 1);
        return x;
      };
      i += 1;
    };
  } else if (x[0] == 1) {
    while (i < 32) {
      if (x[i] == 0 && x2[i] == 1) {
        x[i] = 1;
        IntCPUSameTypeSubstraction(x, x2, i);
        return x;
      } else if (x[i] == 1 && x2[i] == 0) {
        for (int i2 = 0; i2 < i; i2++) {
          x[i2] = 0;
        };
        x[i] = 1;
        IntCPUSameTypeSubstraction(x, x2, i);
        return x;
      };
      x[i] = 1;
      x2[i] = 1;
      i += 1;
    };
    IntCPUSameTypeSubstraction(x, x2, 0);
    x[0] = 0;
    return x;
  } else if (x[0] == 0) {
    while (i < 32) {
      if (x[i] == 1 && x2[i] == 0) {
        IntCPUSameTypeSubstraction(x, x2, 1);
        return x;
      } else if (x[i] == 0 && x2[i] == 1) {
        IntCPUSameTypeSubstraction(x, x2, 1);
        x[0] = 1;
        return x;
      };
      i += 1;
    };
    IntCPUSameTypeSubstraction(x, x2, 0);
    return x;
  };
  return x;
};

void IntSameTypeAddition2(std::deque<bool> &x, std::deque<bool> &x2) {
  const int n = x.size();
  int i = 0;
  int i2 = 0;
  int i_bf;
  while (i < n) {
    if (x[i] == 0 && x2[i2] == 1) {
      x[i] = 1;
    } else if (x[i] == 1 && x2[i2] == 1) {
      x[i] = 0;
      i_bf = i - 1;
      if (i_bf == -1) {
        x.push_front(1);
        i += 1;
      } else {
        while (x[i_bf] != 0) {
          x[i_bf] = 0;
          i_bf -= 1;
          if (i_bf < 0) {
            break;
          };
        };
        if (i_bf >= 0) {
          x[i_bf] = 1;
        } else {
          x[0] = 0;
          x.push_front(1);
          i += 1;
        };
      };
    };
    i += 1;
    i2 += 1;
  };
  return;
};

void IntSameTypeSubstraction2(std::deque<bool> &x, std::deque<bool> &x2) {
  int n = x.size();
  int i = 0;
  int i2 = 0;
  int i_bf;
  while (i < n) {
    if (x[i] == 1 && x2[i2] == 1) {
      x[i] = 0;
      if (i == 0) {
        x.pop_front();
        n -= 1;
        i -= 1;
      };
    } else if (x[i] == 0 && x2[i2] == 1) {
      x[i] = 1;
      i_bf = i - 1;
      if (i_bf == -1) {
        x.pop_front();
        i -= 1;
        n -= 1;
      } else {
        while (x[i_bf] != 1) {
          x[i_bf] = 1;
          i_bf -= 1;
          if (i_bf <= 0) {
            break;
          };
        };
        if (i_bf > 0) {
          x[i_bf] = 0;
        } else {
          x.pop_front();
          i -= 1;
          n -= 1;
        };
      };
    };
    i += 1;
    i2 += 1;
  };
  return;
};

//@T IEEE754DoubleToDoubleAddition
//@U std::deque&lt;bool&gt; IEEE754DoubleToDoubleAddition(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Performs the double addition algorithm with their binary representation.
//@A x : is the first double binary representation
//@A x2 : is the second double binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(double)];
//@E DoubleStore obj1;
//@E obj1.value = 224.8174;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000001101100000110100010100000100100000010110111100000000011
//@E DoubleStore obj2;
//@E obj2.value = -223.546;
//@E memcpy(rslt_arr, obj2.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; dq2[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1100000001101011111100010111100011010100111111011111001110110110
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; obj1.value + obj2.value &lt;&lt; "\n";
//@E 1.2714
//@E std::deque&lt;bool&gt; dq3 = IEEE754DoubleToDoubleAddition(dq, dq2);
//@E BinaryToByteDouble(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; obj1.value &lt;&lt; "\n";
//@E 1.2714
//@X

std::deque<bool> IEEE754DoubleToDoubleAddition(std::deque<bool> x, std::deque<bool> x2) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> mantissa_dq = {1};
  std::deque<bool> mantissa_dq2 = {1};
  std::deque<bool> exponent_dq = {};
  std::deque<bool> exponent_dq2 = {};
  std::deque<bool> just_one = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  std::deque<bool> ref_zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i = 1;
  int delta;
  int pre_delta;
  while (i < 12) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  delta = binarydq2_to_int(exponent_dq);
  i = 1;
  while (i < 12) {
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
  } else if (x[0] == x2[0]) {
    rtn_dq.push_back(x[0]);
    if (delta < 0) {
      while (delta < 0) {
        mantissa_dq.push_front(0);
        mantissa_dq2.push_back(x2[i]);
        delta += 1;
        i += 1;
      };
      while (i < 64) {
        mantissa_dq.push_back(x[i + pre_delta]);
        mantissa_dq2.push_back(x2[i]);
        i += 1; 
      };
      IntSameTypeAddition2(mantissa_dq, mantissa_dq2);
      mantissa_dq.pop_front();
      while (mantissa_dq.size() > 52) {
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
      while (i < 64) {
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_back(x2[i - pre_delta]);
        i += 1; 
      };
      IntSameTypeAddition2(mantissa_dq, mantissa_dq2);
      mantissa_dq.pop_front();
      while (mantissa_dq.size() > 52) {
        IntSameTypeAddition2(exponent_dq, just_one);
        mantissa_dq.pop_back();
      };
      rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
    };
    rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
  } else {
    if (delta < 0) {
      rtn_dq.push_back(x2[0]);
      while (delta < 0) {
        mantissa_dq.push_front(0);
        mantissa_dq2.push_back(x2[i]);
        delta += 1;
        i += 1;
      };
      while (i < 64) {
        mantissa_dq.push_back(x[i + pre_delta]);
        mantissa_dq2.push_back(x2[i]);
        i += 1; 
      };
      IntSameTypeSubstraction2(mantissa_dq2, mantissa_dq);
      while (mantissa_dq2[0] == 0) {
        mantissa_dq2.pop_front();
      };
      mantissa_dq2.pop_front();
      while (mantissa_dq2.size() < 52) {
        IntSameTypeSubstraction2(exponent_dq2, just_one);
        while (exponent_dq2.size() < 11) {
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
      while (i < 64) {
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_back(x2[i - pre_delta]);
        i += 1; 
      };
      IntSameTypeSubstraction2(mantissa_dq, mantissa_dq2);
      while (mantissa_dq[0] == 0) {
        mantissa_dq.pop_front();
      };
      mantissa_dq.pop_front();
      while (mantissa_dq.size() < 52) {
        IntSameTypeSubstraction2(exponent_dq, just_one);
        while (exponent_dq.size() < 11) {
          exponent_dq.push_front(0);
        };
        mantissa_dq.push_back(0);
      };
      rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
      rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
    } else {
      while (i < 64) {
        if (x[i] == 1 && x2[i] == 0) {
          rtn_dq.push_back(x[0]);
          while (i < 64) {
            mantissa_dq.push_back(x[i]);
            mantissa_dq2.push_back(x2[i]);
            i += 1; 
          };
          IntSameTypeSubstraction2(mantissa_dq, mantissa_dq2);
          while (mantissa_dq[0] == 0) {
            mantissa_dq.pop_front();
          };
          mantissa_dq.pop_front();
          while (mantissa_dq.size() < 52) {
            IntSameTypeSubstraction2(exponent_dq, just_one);
            while (exponent_dq.size() < 11) {
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
          rtn_dq.push_back(x2[0]);
          while (i < 64) {
            mantissa_dq.push_back(x[i]);
            mantissa_dq2.push_back(x2[i]);
            i += 1; 
          };
          IntSameTypeSubstraction2(mantissa_dq2, mantissa_dq);
          while (mantissa_dq2[0] == 0) {
            mantissa_dq2.pop_front();
          };
          mantissa_dq2.pop_front();
          while (mantissa_dq2.size() < 52) {
            IntSameTypeSubstraction2(exponent_dq2, just_one);
            while (exponent_dq2.size() < 11) {
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

//@T IEEE754DoubleToDoubleSubstraction
//@U std::deque&lt;bool&gt; IEEE754DoubleToDoubleSubstraction(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Performs the double substraction algorithm with their binary representation.
//@A x : is the first double binary representation
//@A x2 : is the second double binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(double)];
//@E DoubleStore obj1;
//@E obj1.value = -224.8174;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000001101100000110100010100000100100000010110111100000000011
//@E DoubleStore obj2;
//@E obj2.value = -223.546;
//@E memcpy(rslt_arr, obj2.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; dq2[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1100000001101011111100010111100011010100111111011111001110110110
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; obj1.value - obj2.value &lt;&lt; "\n";
//@E 1.2714
//@E std::deque&lt;bool&gt; dq3 = IEEE754DoubleToDoubleSubstraction(dq, dq2);
//@E BinaryToByteDouble(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; obj1.value &lt;&lt; "\n";
//@E -1.2714
//@X

std::deque<bool> IEEE754DoubleToDoubleSubstraction(std::deque<bool> x, std::deque<bool> x2) {
  std::deque<bool> rtn_dq = {};
  std::deque<bool> mantissa_dq = {1};
  std::deque<bool> mantissa_dq2 = {1};
  std::deque<bool> exponent_dq = {};
  std::deque<bool> exponent_dq2 = {};
  std::deque<bool> just_one = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  std::deque<bool> ref_zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i = 1;
  int delta;
  int pre_delta;
  while (i < 12) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  delta = binarydq2_to_int(exponent_dq);
  i = 1;
  while (i < 12) {
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
      while (i < 64) {
        mantissa_dq.push_back(x[i + pre_delta]);
        mantissa_dq2.push_back(x2[i]);
        i += 1; 
      };
      IntSameTypeAddition2(mantissa_dq, mantissa_dq2);
      mantissa_dq.pop_front();
      while (mantissa_dq.size() > 52) {
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
      while (i < 64) {
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_back(x2[i - pre_delta]);
        i += 1; 
      };
      IntSameTypeAddition2(mantissa_dq, mantissa_dq2);
      mantissa_dq.pop_front();
      while (mantissa_dq.size() > 52) {
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
      while (i < 64) {
        mantissa_dq.push_back(x[i + pre_delta]);
        mantissa_dq2.push_back(x2[i]);
        i += 1; 
      };
      IntSameTypeSubstraction2(mantissa_dq2, mantissa_dq);
      while (mantissa_dq2[0] == 0) {
        mantissa_dq2.pop_front();
      };
      mantissa_dq2.pop_front();
      while (mantissa_dq2.size() < 52) {
        IntSameTypeSubstraction2(exponent_dq2, just_one);
        while (exponent_dq2.size() < 11) {
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
      while (i < 64) {
        mantissa_dq.push_back(x[i]);
        mantissa_dq2.push_back(x2[i - pre_delta]);
        i += 1; 
      };
      IntSameTypeSubstraction2(mantissa_dq, mantissa_dq2);
      while (mantissa_dq[0] == 0) {
        mantissa_dq.pop_front();
      };
      mantissa_dq.pop_front();
      while (mantissa_dq.size() < 52) {
        IntSameTypeSubstraction2(exponent_dq, just_one);
        while (exponent_dq.size() < 11) {
          exponent_dq.push_front(0);
        };
        mantissa_dq.push_back(0);
      };
      rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
      rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
    } else {
      while (i < 64) {
        if (x[i] == 1 && x2[i] == 0) {
          rtn_dq.push_back(x[0]);
          while (i < 64) {
            mantissa_dq.push_back(x[i]);
            mantissa_dq2.push_back(x2[i]);
            i += 1; 
          };
          IntSameTypeSubstraction2(mantissa_dq, mantissa_dq2);
          while (mantissa_dq[0] == 0) {
            mantissa_dq.pop_front();
          };
          mantissa_dq.pop_front();
          while (mantissa_dq.size() < 52) {
            IntSameTypeSubstraction2(exponent_dq, just_one);
            while (exponent_dq.size() < 11) {
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
          while (i < 64) {
            mantissa_dq.push_back(x[i]);
            mantissa_dq2.push_back(x2[i]);
            i += 1; 
          };
          IntSameTypeSubstraction2(mantissa_dq2, mantissa_dq);
          while (mantissa_dq2[0] == 0) {
            mantissa_dq2.pop_front();
          };
          mantissa_dq2.pop_front();
          while (mantissa_dq2.size() < 52) {
            IntSameTypeSubstraction2(exponent_dq2, just_one);
            while (exponent_dq2.size() < 11) {
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

//@T IEEE754FloatToFloatAddition
//@U std::deque&lt;bool&gt; IEEE754FloatToFloatAddition(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Performs the float addition algorithm with their binary representation.
//@A x : is the first float binary representation
//@A x2 : is the second float binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(float)];
//@E FloatStore obj1;
//@E obj1.value = -224.8174;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11000011011000001101000101000001
//@E FloatStore obj2;
//@E obj2.value = -223.546;
//@E memcpy(rslt_arr, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; dq2[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11000011010111111000101111000111
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; obj1.value + obj2.value &lt;&lt; "\n";
//@E -448.363
//@E std::deque&lt;bool&gt; dq3 = IEEE754FloatToFloatAddition(dq, dq2);
//@E BinaryToByteFloat(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(float));
//@E std::cout &lt;&lt; obj1.value &lt;&lt; "\n";
//@E -448.363
//@X

std::deque<bool> IEEE754FloatToFloatAddition(std::deque<bool> x, std::deque<bool> x2) {
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
  } else if (x[0] == x2[0]) {
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
      rtn_dq.push_back(x2[0]);
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
          rtn_dq.push_back(x2[0]);
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

//@T IEEE754FloatToFloatSubstraction
//@U std::deque&lt;bool&gt; IEEE754FloatToFloatSubstraction(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Performs the float substraction algorithm with their binary representation.
//@A x : is the first float binary representation
//@A x2 : is the second float binary representation
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(float)];
//@E FloatStore obj1;
//@E obj1.value = -224.8174;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11000011011000001101000101000001
//@E FloatStore obj2;
//@E obj2.value = -223.546;
//@E memcpy(rslt_arr, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; dq2[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11000011010111111000101111000111
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; obj1.value - obj2.value &lt;&lt; "\n";
//@E -1.27139
//@E std::deque&lt;bool&gt; dq3 = IEEE754FloatToFloatSubstraction(dq, dq2);
//@E BinaryToByteFloat(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(float));
//@E std::cout &lt;&lt; obj1.value &lt;&lt; "\n";
//@E -1.27139
//@X

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

//@T IntSameTypeAddition
//@U std::deque&lt;bool&gt; IntSameTypeDivide(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Performs a division between signed integers.
//@A x : is the divided int
//@A x2 : is the divider int
//@X
//@E unsigned char rslt_arr[sizeof(int)];
//@E IntStore obj1;
//@E obj1.x = 21;
//@E memcpy(rslt_arr, obj1.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryInt(rslt_arr);
//@E IntStore obj2;
//@E obj2.x = 1246;
//@E memcpy(rslt_arr, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr);
//@E std::deque&lt;bool&gt; dq3 = IntSameTypeDivide(dq2, dq);
//@E BinaryToByteInt(dq3, rslt_arr);
//@E memcpy(obj1.x_array, rslt_arr, sizeof(int));
//@E std::cout &lt;&lt; obj1.x &lt;&lt; "\n";
//@E 59
//@X

std::deque<bool> IntSameTypeDivide(std::deque<bool> x, std::deque<bool> x2) {
  std::deque<bool> rtn_dq = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<bool> counter_dq = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int ref_max = binarydq2_to_int(x2);
  int i = 0;
  int i2;
  bool is_lower = 1;
  while (is_lower) {
    for (i = 0; i < ref_max; i++) {
      if (rtn_dq[31] == 1) {
        i2 = 31;
        while (rtn_dq[i2] == 1) {
          rtn_dq[i2] = 0;
          i2 -= 1;
        };
        rtn_dq[i2] = 1;
      } else {
        rtn_dq[31] = 1;
      };
    };
    for (i = 1; i < 32; i++) {
      if (rtn_dq[i] == 0 && x[i] == 1) {
        break;
      } else if (rtn_dq[i] == 1 && x[i] == 0) {
        is_lower = false;
        break;
      };
    };
    if (counter_dq[31] == 1) {
      i2 = 31;
      while (counter_dq[i2] == 1) {
        counter_dq[i2] = 0;
        i2 -= 1;
      };
      counter_dq[i2] = 1;
    } else {
      counter_dq[31] = 1;
    };
  };
  if (counter_dq[31] == 1) {
    counter_dq[31] = 0;
  } else {
    i2 = 31;
    while (counter_dq[i2] == 0) {
      counter_dq[i2] = 1;
      i2 -= 1;
    };
    counter_dq[i2] = 0;
  };
  return counter_dq;
};

//@T DecimalToBinary
//@U std::deque&lt;bool&gt; DecimalToBinary(std::deque&lt;int&gt; &x)
//@X
//@D Converts the given decimals into its binary representation.
//@A x : is the input deque containing the decimals to convert to binary
//@X
//@E std::deque&lt;int&gt; ref_decimal = {0, 0, 0, 5, 2, 4};
//@E std::deque&lt;bool&gt; x = DecimalToBinary(ref_decimal);
//@E for (int i2 = 0; i2 &lt; x.size(); i2++) {
//@E   std::cout &lt;&lt; x[i2];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00000000001000100101010
//@E std::cout &lt;&lt; "x size: " &lt;&lt; x.size() &lt;&lt; "\n";
//@E 23 //the output is always 23 bits long
//@X

std::deque<bool> DecimalToBinary(std::deque<int> &x) {
  int i2;
  int i3;
  std::deque<bool> rtn_dq = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<std::deque<bool>> ref_val = {
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  std::deque<bool> cur_decimal = {};
  std::deque<std::deque<std::deque<bool>>> ref_dq = {{
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
  
  {
        {0,0,1,1,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,0,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,0,1,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,1,0,0,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,0,1,1,0,1,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0}},
  
  {
        {0,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,1,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,0,1,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,1,0,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,1,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,0,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0}},
  
  {
        {0,0,1,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
        {0,0,1,1,1,1,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1},
        {0,0,1,1,1,0,1,1,0,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,1,0},
        {0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,0},
        {0,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0},
        {0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,1,0,0},
        {0,0,1,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0},
        {0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,1,0,0,1},
        {0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,1},
        {0,0,1,0,1,1,1,1,1,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0},
        {0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0},
        {0,0,1,0,1,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1},
        {0,0,1,0,0,1,1,1,0,1,0,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1},
        {0,0,1,0,0,1,0,1,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,1,0,0,1,1,1,0,0,0,0,0,0},
        {0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,1,0},
        {0,0,1,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,1,0,0,0},
        {0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0},
        {0,0,0,1,1,1,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0},
        {0,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,0,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0}},
  
  {
        {0,0,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,1,0,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,1,0,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,1,0,0,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,1,1,0,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,1,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,0,1,1,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,1,0,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,1,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,1,0,0,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0}},
  
  {
        {0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,0,1,1,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,0,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,0,1,1,1,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,0,1,1,1,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,0,0,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1}},
  
  {
        {0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
        {0,0,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1},
        {0,0,1,1,1,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,1,0},
        {0,0,1,1,1,0,1,0,0,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,0},
        {0,0,1,1,1,0,0,0,0,1,1,1,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0},
        {0,0,1,1,0,1,1,0,1,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,1,0,0},
        {0,0,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0},
        {0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,1,0,0,1},
        {0,0,1,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,1},
        {0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0},
        {0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0},
        {0,0,1,0,1,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1},
        {0,0,1,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1},
        {0,0,1,0,0,1,1,0,0,0,1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,1,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,0,1,1,0,0,1,1,1,0,0,0,0,0,0},
        {0,0,1,0,0,0,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,1,0},
        {0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,1,0,0,0},
        {0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0},
        {0,0,0,1,1,1,0,1,1,1,1,0,0,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0},
        {0,0,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,0,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0}},
  
  {
        {0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
        {0,0,1,1,1,1,0,1,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,1},
        {0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,1,0,1,1,0,0,0,0,0,0,1,0,0,0,0,1,0},
        {0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0},
        {0,0,1,1,1,0,0,0,1,0,0,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,1,1,0,1,1,1},
        {0,0,1,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,1,1,0,0,0,1,0,1,1},
        {0,0,1,1,0,1,0,1,0,0,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,0,0,0,1,0},
        {0,0,1,1,0,0,1,1,1,0,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,0,0,0},
        {0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,1,1},
        {0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,1,0,0,1,0,1},
        {0,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1},
        {0,0,1,0,1,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
        {0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,0,1,1},
        {0,0,1,0,0,1,1,0,0,1,0,0,1,0,0,1,1,1,0,0,0,0,1,0,1,1,1,0,0,0,1,0},
        {0,0,1,0,0,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,0,1,0,1,1,0,1,0,1},
        {0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,1},
        {0,0,1,0,0,0,0,1,0,1,0,0,1,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,0,0,0,1},
        {0,0,0,1,1,1,1,1,1,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,1,0,0,1,1,1},
        {0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,1,0,1,1,0,0},
        {0,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1}},
  
  {
        {0,0,1,1,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,1,0,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,1,0,0,1,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,1,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,1,0,0,0,0,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,1,0,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0}},
  
  {
        {0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
        {0,0,1,1,1,1,0,1,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,0},
        {0,0,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0},
        {0,0,1,1,1,0,1,0,0,1,1,0,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,0},
        {0,0,1,1,1,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,1,1,0,0,1,1,0,0,0,1,0},
        {0,0,1,1,0,1,1,1,0,0,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,0,1,0,1},
        {0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,1},
        {0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0},
        {0,0,1,1,0,0,1,0,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1},
        {0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,0,0,0,1,1,1,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,0,1,1,0},
        {0,0,1,0,1,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,0,1,1,1,1,0,1,0,1,1,0},
        {0,0,1,0,1,0,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,1,1,0,0,0},
        {0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,0,1},
        {0,0,1,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0},
        {0,0,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,1,0,0,1,1,1,1,1},
        {0,0,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,1,0,1,0,0,1,1,0,0},
        {0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1},
        {0,0,0,1,1,1,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0},
        {0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0},
        {0,0,0,1,1,0,1,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,0,0,1,1,0,1}}
  };
  bool is_greater;
  for (int i = 0; i < x.size(); i++) {
    cur_decimal = ref_dq[x[i]][i];
    for (i2 = 0; i2 < ref_val.size(); i2++) {
      is_greater = IsSuperiorFloat(cur_decimal, ref_val[i2]);
      if (is_greater) {
        if (!rtn_dq[i2]) {
          rtn_dq[i2] = 1;
        } else {
          rtn_dq[i2] = 0;
          i3 = i2 - 1;
          while (rtn_dq[i3]) {
            rtn_dq[i3] = 0;
            i3 -= 1;
          };
          rtn_dq[i3] = 1;
        };
        while (is_greater) {
          cur_decimal = IEEE754FloatToFloatSubstraction(cur_decimal, ref_val[i2]);
          is_greater = IsSuperiorFloat(cur_decimal, ref_val[i2]);
        };
        if (cur_decimal == ref_dq[0][0]) {
          break;
        };
      };
    };
  };
  return rtn_dq;
};

bool IsSuperiorFloatOrEq(std::deque<bool> &x1, std::deque<bool> &x2) {
  unsigned int n = x1.size();
  std::deque<bool> exponent1_dq = {};
  std::deque<bool> exponent2_dq = {};
  std::deque<bool> mantissa1_dq = {};
  std::deque<bool> mantissa2_dq = {};
  bool is_negative1 = 0;
  bool is_negative2 = 0;
  int val1;
  int val2;
  int i = 1;
  if (n != 32) {
    std::cout << "Error: first arg is not a float binary representation";
    return 0;
  };
  if (x2.size() != 32) {
    std::cout << "Error: second arg is not a float binary representation";
    return 0;
  };
  if (x1[0] == 1) {
    is_negative1 = 1;
    if (x2[0] == 0) {
      return 0;
    };
  };
  if (x2[0] == 1) {
    is_negative2 = 1;
    if (!is_negative1) {
      return 1;
    };
  };  
  while (i < 32) {
    if (x1[i] && !x2[i]) {
      if (is_negative1) {
        return 0;
      } else {
        return 1;
      };
    } else if (!x1[i] && x2[i]) {
      if (is_negative1) {
        return 1;
      } else {
        return 0;
      };
    };
    i += 1;
  };
  return 1; 
};

std::deque<bool> DecimalToBinary2(std::deque<int> &x, std::deque<bool> rtn_dq) {
  int i2;
  int i3;
  std::deque<std::deque<bool>> ref_val = {
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  std::deque<bool> cur_decimal = {};
  std::deque<std::deque<std::deque<bool>>> ref_dq = {{
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}},
  
  {
        {0,0,1,1,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,0,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,0,1,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,1,0,0,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,0,1,1,0,1,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0}},
  
  {
        {0,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,1,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,0,1,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,1,0,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,1,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,0,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0}},
  
  {
        {0,0,1,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
        {0,0,1,1,1,1,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1},
        {0,0,1,1,1,0,1,1,0,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,1,0},
        {0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,0},
        {0,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0},
        {0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,1,0,0},
        {0,0,1,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0},
        {0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,1,0,0,1},
        {0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,1},
        {0,0,1,0,1,1,1,1,1,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0},
        {0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0},
        {0,0,1,0,1,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1},
        {0,0,1,0,0,1,1,1,0,1,0,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1},
        {0,0,1,0,0,1,0,1,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,1,0,0,1,1,1,0,0,0,0,0,0},
        {0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,1,0},
        {0,0,1,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,1,0,0,0},
        {0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0},
        {0,0,0,1,1,1,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0},
        {0,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,0,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0}},
  
  {
        {0,0,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,1,0,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,1,0,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,1,0,0,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,1,1,0,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,1,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,0,1,1,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,1,0,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,1,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,1,0,0,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0}},
  
  {
        {0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,0,1,1,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,0,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,0,1,1,1,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,0,1,1,1,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,0,0,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1}},
  
  {
        {0,0,1,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
        {0,0,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1},
        {0,0,1,1,1,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,1,0},
        {0,0,1,1,1,0,1,0,0,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,0},
        {0,0,1,1,1,0,0,0,0,1,1,1,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0},
        {0,0,1,1,0,1,1,0,1,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,1,0,0},
        {0,0,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0},
        {0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,1,0,0,1},
        {0,0,1,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,1},
        {0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0},
        {0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0},
        {0,0,1,0,1,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1},
        {0,0,1,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1},
        {0,0,1,0,0,1,1,0,0,0,1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0},
        {0,0,1,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,0,1,1,0,0,1,1,1,0,0,0,0,0,0},
        {0,0,1,0,0,0,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,1,0},
        {0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,1,0,0,0},
        {0,0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0},
        {0,0,0,1,1,1,0,1,1,1,1,0,0,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0},
        {0,0,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,0,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,0}},
  
  {
        {0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
        {0,0,1,1,1,1,0,1,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,1},
        {0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,1,0,1,1,0,0,0,0,0,0,1,0,0,0,0,1,0},
        {0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0},
        {0,0,1,1,1,0,0,0,1,0,0,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,1,1,0,1,1,1},
        {0,0,1,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,1,1,0,0,0,1,0,1,1},
        {0,0,1,1,0,1,0,1,0,0,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,0,0,0,1,0},
        {0,0,1,1,0,0,1,1,1,0,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,0,0,0},
        {0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,1,1},
        {0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,1,0,0,1,0,1},
        {0,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,1},
        {0,0,1,0,1,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
        {0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,0,1,1},
        {0,0,1,0,0,1,1,0,0,1,0,0,1,0,0,1,1,1,0,0,0,0,1,0,1,1,1,0,0,0,1,0},
        {0,0,1,0,0,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,0,1,0,1,1,0,1,0,1},
        {0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,1},
        {0,0,1,0,0,0,0,1,0,1,0,0,1,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,0,0,0,1},
        {0,0,0,1,1,1,1,1,1,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,1,0,0,1,1,1},
        {0,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,1,0,1,1,0,0},
        {0,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
        {0,0,0,1,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1}},
  
  {
        {0,0,1,1,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
        {0,0,1,1,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
        {0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,1},
        {0,0,1,1,1,0,1,0,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1},
        {0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
        {0,0,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1},
        {0,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1},
        {0,0,1,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1},
        {0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0},
        {0,0,1,0,1,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1},
        {0,0,1,0,1,0,0,1,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0},
        {0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,1},
        {0,0,1,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0},
        {0,0,1,0,0,0,1,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,1},
        {0,0,1,0,0,0,0,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0},
        {0,0,0,1,1,1,1,0,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0},
        {0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1},
        {0,0,0,1,1,0,1,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0}},
  
  {
        {0,0,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
        {0,0,1,1,1,1,0,1,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,0},
        {0,0,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0},
        {0,0,1,1,1,0,1,0,0,1,1,0,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,0},
        {0,0,1,1,1,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,1,1,0,0,1,1,0,0,0,1,0},
        {0,0,1,1,0,1,1,1,0,0,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,0,1,0,1},
        {0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,1},
        {0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0},
        {0,0,1,1,0,0,1,0,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1},
        {0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,0,0,0,1,1,1,1,0,1,1,1,1,1},
        {0,0,1,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1},
        {0,0,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,0,1,1,0},
        {0,0,1,0,1,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,0,1,1,1,1,0,1,0,1,1,0},
        {0,0,1,0,1,0,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,1,1,0,0,0},
        {0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,0,1},
        {0,0,1,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0},
        {0,0,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,1,0,0,1,1,1,1,1},
        {0,0,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,1,0,1,0,0,1,1,0,0},
        {0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,1,0},
        {0,0,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1},
        {0,0,0,1,1,1,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0},
        {0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0},
        {0,0,0,1,1,0,1,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,0,0,1,1,0,1}}
  };
  bool is_greater;
  for (int i = 0; i < x.size(); i++) {
    cur_decimal = ref_dq[x[i]][i];
    for (i2 = 0; i2 < rtn_dq.size(); i2++) {
      is_greater = IsSuperiorFloat(cur_decimal, ref_val[i2]);
      if (is_greater) {
        if (!rtn_dq[i2]) {
          rtn_dq[i2] = 1;
        } else {
          rtn_dq[i2] = 0;
          i3 = i2 - 1;
          while (rtn_dq[i3]) {
            rtn_dq[i3] = 0;
            i3 -= 1;
          };
          rtn_dq[i3] = 1;
        };
        while (is_greater) {
          cur_decimal = IEEE754FloatToFloatSubstraction(cur_decimal, ref_val[i2]);
          is_greater = IsSuperiorFloat(cur_decimal, ref_val[i2]);
        };
        if (cur_decimal == ref_dq[0][0]) {
          break;
        };
      };
    };
  };
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

std::deque<bool> DecimalToBinary2b(std::deque<int> &x, std::deque<bool> rtn_dq) {
  int i2;
  int i3;
  std::deque<std::deque<bool>> ref_val = {
     {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

     {0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     
     {0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  std::deque<bool> cur_decimal = {};
  std::deque<std::deque<std::deque<bool>>> ref_dq = {{
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} },
  
    {
      {0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
      {0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1},
      {0,0,1,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,1,0,0},
      {0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,1,0,1},
      {0,0,1,1,1,1,1,0,1,1,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,0,1},
      {0,0,1,1,1,1,1,0,1,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1,1,0,0,0,1,1,0,1},
      {0,0,1,1,1,1,1,0,0,1,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,1,1,0,0,1,0,0,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,1,0},
      {0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,0,1,1},
      {0,0,1,1,1,1,0,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,1},
      {0,0,1,1,1,1,0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0},
      {0,0,1,1,1,1,0,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,1},
      {0,0,1,1,1,1,0,0,1,1,0,1,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0},
      {0,0,1,1,1,1,0,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,1,0,0},
      {0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,0,1,0,0,1,0,0,1,0,1,1,1},
      {0,0,1,1,1,1,0,0,0,0,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0,0,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1},
      {0,0,1,1,1,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,1,0,1,0,0,1,1,1,1},
      {0,0,1,1,1,0,1,1,0,1,0,1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,0},
      {0,0,1,1,1,0,1,0,1,1,1,1,0,0,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0},
      {0,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,0,1,1,0,1,0,0,0,0,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0},
      {0,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1},
      {0,0,1,1,1,0,1,0,0,1,0,1,0,0,1,1,1,1,0,0,1,1,1,0,1,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0},
      {0,0,1,1,1,0,1,0,0,0,0,1,1,1,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0},
      {0,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0},
      {0,0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,0},
      {0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,1,0,0,1,1,0,0,0,1,0,0,1,0,1,0},
      {0,0,1,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1},
      {0,0,1,1,1,0,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,0,1,1},
      {0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,1},
      {0,0,1,1,1,0,0,0,0,0,0,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,1},
      {0,0,1,1,0,1,1,1,1,0,1,0,0,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,1,0,1},
      {0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0},
      {0,0,1,1,0,1,1,1,0,0,1,1,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1},
      {0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,1,1,0,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0},
      {0,0,1,1,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0},
      {0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,0,0,1,0,1,1,0,1,0,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,1,0},
      {0,0,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,0},
      {0,0,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,1},
      {0,0,1,1,0,1,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,0},
      {0,0,1,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,1,0,0},
      {0,0,1,1,0,1,0,1,0,0,1,0,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0},
      {0,0,1,1,0,1,0,0,1,1,1,0,1,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,1,0,1} },
  
  {
      {0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
      {0,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1},
      {0,0,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,1,0,0},
      {0,0,1,1,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,1,0,1},
      {0,0,1,1,1,1,1,0,1,1,1,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,0,1},
      {0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1,1,0,0,0,1,1,0,1},
      {0,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,1,1,0,0,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,1,0},
      {0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,0,1,1},
      {0,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,1},
      {0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0},
      {0,0,1,1,1,1,0,1,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,1},
      {0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0},
      {0,0,1,1,1,1,0,0,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,1,0,0},
      {0,0,1,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,0,1,0,0,1,0,0,1,0,1,1,1},
      {0,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0,0,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,1,1,0,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1},
      {0,0,1,1,1,0,1,1,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,1,0,1,0,0,1,1,1,1},
      {0,0,1,1,1,0,1,1,0,1,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,1,0,1,1,0,0,1,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,0},
      {0,0,1,1,1,0,1,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0},
      {0,0,1,1,1,0,1,0,1,1,0,0,1,1,1,0,1,1,1,1,0,0,1,0,1,1,0,1,0,0,0,0,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0},
      {0,0,1,1,1,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1},
      {0,0,1,1,1,0,1,0,0,1,1,0,0,0,1,1,1,1,0,0,1,1,1,0,1,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0},
      {0,0,1,1,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0},
      {0,0,1,1,1,0,0,1,1,1,1,1,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,1,0,1,1,0,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0},
      {0,0,1,1,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,0},
      {0,0,1,1,1,0,0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,1,0,0,1,1,0,0,0,1,0,0,1,0,1,0},
      {0,0,1,1,1,0,0,0,1,0,1,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1},
      {0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,0,1,1},
      {0,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,1},
      {0,0,1,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,0,1,1,1,1,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,1},
      {0,0,1,1,0,1,1,1,1,0,1,1,0,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,1,0,1},
      {0,0,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0},
      {0,0,1,1,0,1,1,1,0,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1},
      {0,0,1,1,0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,1,1,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0},
      {0,0,1,1,0,1,1,0,1,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0},
      {0,0,1,1,0,1,1,0,0,1,1,1,0,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,1,0,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,1,0},
      {0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,0},
      {0,0,1,1,0,1,0,1,1,1,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,1},
      {0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,0},
      {0,0,1,1,0,1,0,1,0,1,1,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,1,0,0},
      {0,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0},
      {0,0,1,1,0,1,0,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,1,0,1} },
  
   {
      {0,0,1,1,1,1,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
      {0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0},
      {0,0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0},
      {0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,1},
      {0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,0,1,0,0,1},
      {0,0,1,1,1,1,1,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,0,1,0,0},
      {0,0,1,1,1,1,1,0,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,1,1,0,1,1,0},
      {0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,1,0,1,0,0,1,0,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1},
      {0,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1},
      {0,0,1,1,1,1,0,1,1,1,1,1,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0},
      {0,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,1,0,1,1,1,0,0,0,0},
      {0,0,1,1,1,1,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,0,0,1,1,0,1,1,1,1,1,0,0,0,1,1,0,1,0},
      {0,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,1},
      {0,0,1,1,1,1,0,1,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,0,1,1,0,1,0,0},
      {0,0,1,1,1,1,0,0,1,1,1,0,1,0,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1,0,1,1,1,0,0,1,0,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0},
      {0,0,1,1,1,1,0,0,1,0,1,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,0,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,1,0,0,1,1,0,1},
      {0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,1,1,0,1,1,1,0,0,0,1},
      {0,0,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,0,1,1,0,0,1,0,1,1,0,0,0,0,0,0,1},
      {0,0,1,1,1,1,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
      {0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0,0,1,0,0,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,1,1},
      {0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,0,1,0,1,1,1,1,0,1,0,0,0,1,1,1,1,0,1,1,1},
      {0,0,1,1,1,0,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,0,1},
      {0,0,1,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,1,1,0,1,0,1,0,1,1,1,1,1,1,0,1,1},
      {0,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,1,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,0,1,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1},
      {0,0,1,1,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,1,1,1},
      {0,0,1,1,1,0,1,0,0,1,1,0,1,1,0,1,1,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1},
      {0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,1},
      {0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,1,1,1,0,1,1,0},
      {0,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,0,1,1,0,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,1,0,0,0,1,1,1,1,1,1,0,0,0,0},
      {0,0,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,0,1,0,1,1,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,1,1,0,0,0,1,1,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,1,0,1,1,0,0,1,1,1},
      {0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,1,0,0,1,0,0,1,1,1,0,1,0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,0,0,1,0,1,0},
      {0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,1,1,0,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,0,0,1,1,0,1,1,1,0},
      {0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,1,0,1,0,0,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1},
      {0,0,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,0,0,0,1,0,1,1,0,0,1,1,0,0,0,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1},
      {0,0,1,1,1,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,0,1,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,0},
      {0,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0},
      {0,0,1,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0},
      {0,0,1,1,0,1,1,1,1,0,0,0,0,1,0,0,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,1,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,0,1,0,1,1,0},
      {0,0,1,1,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,0,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,1},
      {0,0,1,1,0,1,1,1,0,0,0,1,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0,0,1,0,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0},
      {0,0,1,1,0,1,1,0,1,1,1,0,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,1,1,1,0},
      {0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,1,1},
      {0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,1,1,0,0,1,1,1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,1,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
      {0,0,1,1,0,1,1,0,0,1,0,0,0,1,0,1,1,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,1,1,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,0},
      {0,0,1,1,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0,1,1,0,1,1,1,0,1,1,1,0,1,0,0,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0},
      {0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,0,1,1,0,0,1,0,0,1,1,1,0,0},
      {0,0,1,1,0,1,0,1,1,0,1,0,0,1,1,0,0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,0,1,0,0,0,0,1,0,1,1,1},
      {0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,1,0,0,1,1,1,1,0,0,1},
      {0,0,1,1,0,1,0,1,0,0,1,1,1,1,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,1,1,0,0,1,0,1,0,0,0},
      {0,0,1,1,0,1,0,1,0,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,1,0,0,1,0,0,0,0,1,1,0,1,0,1,0,1,0,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,0,1,1,1,0,1,0} },
   {
      {0,0,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
      {0,0,1,1,1,1,1,1,1,0,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1},
      {0,0,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,1,0,0},
      {0,0,1,1,1,1,1,1,0,0,1,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,1,0,1},
      {0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,0,1},
      {0,0,1,1,1,1,1,0,1,1,0,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1,1,0,0,0,1,1,0,1},
      {0,0,1,1,1,1,1,0,1,0,0,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,1,1,0,0,1,1,0,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,1,0},
      {0,0,1,1,1,1,1,0,0,0,1,1,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,0,1,1},
      {0,0,1,1,1,1,0,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,1,0,0,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,1},
      {0,0,1,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0},
      {0,0,1,1,1,1,0,1,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,1},
      {0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0},
      {0,0,1,1,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,1,0,0},
      {0,0,1,1,1,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,0,1,0,0,1,0,0,1,0,1,1,1},
      {0,0,1,1,1,1,0,0,0,1,0,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,1,0,0,0,0,0,1,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0,0,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1},
      {0,0,1,1,1,0,1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,1,0,1,0,0,1,1,1,1},
      {0,0,1,1,1,0,1,1,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,1,0,1,1,0,1,0,0,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,0},
      {0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0},
      {0,0,1,1,1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,0,1,0,1,1,0,1,0,0,0,0,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0},
      {0,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1},
      {0,0,1,1,1,0,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,1,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0},
      {0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0},
      {0,0,1,1,1,0,1,0,0,0,0,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0},
      {0,0,1,1,1,0,0,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,0},
      {0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,1,0,0,1,1,0,0,0,1,0,0,1,0,1,0},
      {0,0,1,1,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1},
      {0,0,1,1,1,0,0,0,1,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,0,1,1},
      {0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,1},
      {0,0,1,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,1},
      {0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,1,0,1},
      {0,0,1,1,0,1,1,1,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0},
      {0,0,1,1,0,1,1,1,0,1,0,1,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1},
      {0,0,1,1,0,1,1,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,1,1,1,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0},
      {0,0,1,1,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0},
      {0,0,1,1,0,1,1,0,1,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,0,1,0,0,1,1,0,1,0,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,1,0},
      {0,0,1,1,0,1,1,0,0,0,0,1,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,0},
      {0,0,1,1,0,1,0,1,1,1,1,0,0,0,1,0,1,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,1},
      {0,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,0},
      {0,0,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,1,0,0},
      {0,0,1,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0},
      {0,0,1,1,0,1,0,1,0,0,0,0,1,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,1,0,1} },
   {
      {0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
      {0,0,1,1,1,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1},
      {0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,1,0,0},
      {0,0,1,1,1,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,1,0,1},
      {0,0,1,1,1,1,1,0,1,1,0,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,0,1},
      {0,0,1,1,1,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1,1,0,0,0,1,1,0,1},
      {0,0,1,1,1,1,1,0,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,1,1,0,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,1,0},
      {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,1,1,0,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,0,1,1},
      {0,0,1,1,1,1,0,1,1,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,0,1,1,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,1},
      {0,0,1,1,1,1,0,1,0,0,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0},
      {0,0,1,1,1,1,0,0,1,1,1,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,1},
      {0,0,1,1,1,1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0},
      {0,0,1,1,1,1,0,0,1,0,0,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,1,0,0},
      {0,0,1,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,0,1,0,0,1,0,0,1,0,1,1,1},
      {0,0,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0,0,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1},
      {0,0,1,1,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,1,0,1,0,0,1,1,1,1},
      {0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,1,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1},
      {0,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0},
      {0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,1,1,0,0,1,0,1,1,0,1,0,0,0,0,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0},
      {0,0,1,1,1,0,1,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1},
      {0,0,1,1,1,0,1,0,0,1,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0},
      {0,0,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0},
      {0,0,1,1,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,0,0,1,1,0,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,0,1,1},
      {0,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,0},
      {0,0,1,1,1,0,0,0,1,1,0,1,0,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,1,0,0,1,1,0,0,0,1,0,0,1,0,0,1},
      {0,0,1,1,1,0,0,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0},
      {0,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,0,1,1},
      {0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,1,0,0,0,0},
      {0,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,0,1,1,1,1,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,1},
      {0,0,1,1,0,1,1,1,1,0,0,1,0,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,1,0,1},
      {0,0,1,1,0,1,1,1,0,1,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0},
      {0,0,1,1,0,1,1,1,0,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1},
      {0,0,1,1,0,1,1,0,1,1,1,1,0,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,1,0,1,1,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0},
      {0,0,1,1,0,1,1,0,1,0,0,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1,1,0,0,1},
      {0,0,1,1,0,1,1,0,0,1,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,0,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,1,0},
      {0,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,0},
      {0,0,1,1,0,1,0,1,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,1},
      {0,0,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1},
      {0,0,1,1,0,1,0,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,1,0,0},
      {0,0,1,1,0,1,0,1,0,0,0,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0} },
   {
      {0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1},
      {0,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0},
      {0,0,1,1,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0},
      {0,0,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,1},
      {0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,0,1,0,0,1},
      {0,0,1,1,1,1,1,0,1,1,0,1,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,0,1,0,0},
      {0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,1,1,0,1,1,0},
      {0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,1,0,1,0,0,1,0,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1},
      {0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1},
      {0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0},
      {0,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,0,1,0,1,1,0,1,1,1,0,0,0,0},
      {0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0,0,0,0,0,1,1,1,0,0,0,1,0,0,1,1,0,1,1,1,1,1,0,0,0,1,1,0,1,0},
      {0,0,1,1,1,1,0,1,0,1,1,0,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,1},
      {0,0,1,1,1,1,0,1,0,0,1,1,0,0,0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,1,0,1,1,0,1,0,0},
      {0,0,1,1,1,1,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,1,1,1,0,1,1,0,1,1,1,0,0,1,0,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0},
      {0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,1,1,0,0,1,1,1,1,0,0,0,0,0,0,1,0,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,1,0,0,1,1,0,1},
      {0,0,1,1,1,1,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,1,1,0,1,1,1,0,0,0,1},
      {0,0,1,1,1,1,0,0,0,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,0,1,1,0,0,1,0,1,1,0,0,0,0,0,0,1},
      {0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,0,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
      {0,0,1,1,1,0,1,1,1,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0,0,1,0,0,1,0,1,1,0,1,1,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,1,1},
      {0,0,1,1,1,0,1,1,1,0,1,1,1,1,0,0,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,0,1,0,1,1,1,1,0,1,0,0,0,1,1,1,1,0,1,1,1},
      {0,0,1,1,1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,0,1},
      {0,0,1,1,1,0,1,1,0,0,0,1,1,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,1,1,0,1,0,1,0,1,1,1,1,1,1,0,1,1},
      {0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,1,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,0,1,1,1,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1},
      {0,0,1,1,1,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,1,1,1},
      {0,0,1,1,1,0,1,0,0,1,1,1,1,1,0,1,1,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1},
      {0,0,1,1,1,0,1,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,1},
      {0,0,1,1,1,0,1,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,0,1,1,1,0,1,1,0},
      {0,0,1,1,1,0,0,1,1,1,0,1,1,1,1,0,0,1,1,0,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,1,0,0,0,1,1,1,1,1,1,0,0,0,0},
      {0,0,1,1,1,0,0,1,1,0,1,0,1,0,0,0,0,1,0,1,0,1,1,0,1,1,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,1,0,0,1,1,1,0,0,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,1,1,1,0,0,1,1,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,0,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,1,0,1,1,0,0,1,1,1},
      {0,0,1,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,1,1,0,1,0,1,1,1,1,0,0,0,1,1,0,0,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,0,0,1,0,1,0},
      {0,0,1,1,1,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,1,1,0,1,0,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,1,0,0,1,0,0,1,1,0,1,1,1,0},
      {0,0,1,1,1,0,0,0,1,1,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,1,0,1,0,0,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1},
      {0,0,1,1,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,0,0,0,1,1,0,1,0,0,1,1,0,0,0,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,1,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1},
      {0,0,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,0,1,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,0},
      {0,0,1,1,0,1,1,1,1,1,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,1,1,0,0},
      {0,0,1,1,0,1,1,1,1,0,0,1,0,1,0,0,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,1,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,0,1,0,1,1,0},
      {0,0,1,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,0,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,1},
      {0,0,1,1,0,1,1,1,0,0,1,0,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0,0,1,0,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0},
      {0,0,1,1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,1,1,1,0},
      {0,0,1,1,0,1,1,0,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,1,1},
      {0,0,1,1,0,1,1,0,1,0,0,0,1,0,1,1,0,1,1,0,0,1,1,1,0,0,1,1,0,1,0,1,0,0,1,1,0,1,1,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
      {0,0,1,1,0,1,1,0,0,1,0,1,0,1,0,1,1,1,1,0,1,1,0,0,0,0,1,0,1,0,1,0,1,0,0,1,0,0,0,1,1,1,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,0},
      {0,0,1,1,0,1,1,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,1,1,0,1,1,1,0,1,1,1,0,1,0,0,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0},
      {0,0,1,1,0,1,0,1,1,1,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,0,1,1,0,0,1,0,0,1,1,1,0,0},
      {0,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,0,1,0,0,0,0,1,0,1,1,1},
      {0,0,1,1,0,1,0,1,1,0,0,0,0,0,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,1,0,0,1,1,1,1,0,0,1},
      {0,0,1,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,1,1,0,0,1,0,1,0,0,0},
      {0,0,1,1,0,1,0,1,0,0,0,1,0,1,1,0,1,1,1,1,1,1,0,0,1,1,0,0,1,0,0,0,0,1,1,0,1,0,1,0,1,0,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,0,1,1,1,0,1,0} },
   {
      {0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0},
      {0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,0},
      {0,0,1,1,1,1,1,1,0,1,1,1,1,1,0,0,1,0,1,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,1},
      {0,0,1,1,1,1,1,1,0,1,0,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,1},
      {0,0,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,0,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,0,0,1,0},
      {0,0,1,1,1,1,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,1,1,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1},
      {0,0,1,1,1,1,1,0,1,0,1,0,0,1,1,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,0,1,0,1,1,1,1,1},
      {0,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,1,1,0,1,0,0,0,0,0,1,0,1,1,1,1,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,1,1,0,0,1,1},
      {0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,1,0,0,0,0,1,1,1,1,1,1,0,1,1,1,1,0,0,0,0,1,0,0},
      {0,0,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,1,1,1,1,0,1,0,0,1,1,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1},
      {0,0,1,1,1,1,0,1,1,1,0,1,0,0,1,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,0,1,0,1,0,0,1,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1},
      {0,0,1,1,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,1,1,0,0,1,1,0,0,1,1,1,1,0},
      {0,0,1,1,1,1,0,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,1,0,1,1,0,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,1,1,0,0,1,0},
      {0,0,1,1,1,1,0,1,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,0,0,0,0,0,1,1,0,0,0,1,0,1,0,0,0},
      {0,0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,0,1,0,1,0,1,1,0,0,0,0,1,0,1,0,0,1,1,0,1,0,1,1,0,1,0,1,0,0,1,1,0},
      {0,0,1,1,1,1,0,0,1,1,0,0,1,0,0,1,0,0,1,1,1,0,0,0,0,1,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,0,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,1},
      {0,0,1,1,1,1,0,0,1,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,0,1,0,1,1,0,1,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,1,0,0},
      {0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,1,1,0},
      {0,0,1,1,1,1,0,0,0,0,1,0,1,0,0,1,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,0,1,1,1},
      {0,0,1,1,1,0,1,1,1,1,1,1,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,1,1,0,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,0,1,1,1,1,1},
      {0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,0},
      {0,0,1,1,1,0,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,0,0,1,1,0,0,0,0,1,0,0,1},
      {0,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,1,0,0,0},
      {0,0,1,1,1,0,1,1,0,0,1,0,0,0,0,0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,1,0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,0,0,1,1},
      {0,0,1,1,1,0,1,0,1,1,1,0,1,0,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0,1,1,0,1,1,0,1,0,1,1,1,0,0,0,1,1,1,1,1,0,0,1,0,1,1,1,0,0,0,0,1,1,1,1,1},
      {0,0,1,1,1,0,1,0,1,0,1,1,0,1,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,0,1,0,1,0,1,1,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,1,0},
      {0,0,1,1,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,1,0,0,1,0,0,0,1,1,1,0},
      {0,0,1,1,1,0,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,0,1,0,1,0,1,0,0,0,0,0,1,0,1,1,0},
      {0,0,1,1,1,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,1,1,1,1,0,0,0,1,0,0,1,1,0,0,0,1,1,1,1,0,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,0,1,1,0,1,1,1,1,1},
      {0,0,1,1,1,0,0,1,1,1,1,0,0,0,0,1,1,0,1,1,1,1,1,1,0,1,0,0,0,0,1,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0},
      {0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,0,0,1,1,0,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0,1,1,0,1,1,1,0,0,0,0,1},
      {0,0,1,1,1,0,0,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,0,1,0,0,1,0,0,1,1,0,1},
      {0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,1,1,1,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,0,0,0,1,1,0,1,0,0,0,1,0,0,1,1,1,0,1,0,1,1,0,1,0,1,0,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,0,0,0,1},
      {0,0,1,1,1,0,0,0,1,1,0,1,0,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,1,1,0,1,0,1,0,1,1,0,0,1,1,0,1},
      {0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,1,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1},
      {0,0,1,1,1,0,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,0,0,1,1,0,0,1,1,0,1,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,1,0,0,0,0,1,1,1,0,1,1},
      {0,0,1,1,1,0,0,0,0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,0,1,0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,1,0,0,1,1,1,0,0,1,0,0,1},
      {0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,1,0,0,1,0,1,0,1,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,0,0},
      {0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1,1,0,1,1,0,0,1,0,1,0,1,0,0,1,1},
      {0,0,1,1,0,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,0,0,0,0,1,1,0,1,0,1,0,1,1,0,1,0,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1},
      {0,0,1,1,0,1,1,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,1,0,0,1,1,1,0,0,1,0},
      {0,0,1,1,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,1,0,1,1,0,0,1,0,0,1,0,1,0,1,1,0,1,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,0,1,0,1,0},
      {0,0,1,1,0,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,0,1,0,0,1,1,1,0,0,1,0,1,1,1,0,1,1},
      {0,0,1,1,0,1,1,0,1,1,0,0,0,0,1,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,1,0,0,0,0,0,1,1,1,1,1,0,1,0,1,1,0,0,0,1,0},
      {0,0,1,1,0,1,1,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,1,0,0,0,1,0,1,1,1,1,1,1,0,1,0,0,1,1,0,1,1,0,0,1,0,1,0,1,0,1,1,0,1,0,1,0},
      {0,0,1,1,0,1,1,0,0,1,0,1,1,0,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0},
      {0,0,1,1,0,1,1,0,0,0,1,0,0,1,0,0,0,1,1,1,0,1,1,0,0,0,0,0,0,1,0,1,1,0,0,1,1,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,0,1,1,1,0,1},
      {0,0,1,1,0,1,0,1,1,1,1,1,0,0,0,0,0,1,0,1,1,1,1,0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,0,1,1,0,0,0,1},
      {0,0,1,1,0,1,0,1,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,1,1},
      {0,0,1,1,0,1,0,1,1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},
      {0,0,1,1,0,1,0,1,0,1,0,1,0,0,0,0,1,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,1,1,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,0,0,1,0},
      {0,0,1,1,0,1,0,1,0,0,0,1,1,0,1,0,1,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,0,1,1,1,1,1,0,0,0,1,1,0,0,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1} },
  {
      {0,0,1,1,1,1,1,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0},
      {0,0,1,1,1,1,1,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1},
      {0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,1,0,0},
      {0,0,1,1,1,1,1,1,0,1,0,0,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,0,0,0,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,1,1,0,1},
      {0,0,1,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,0,1},
      {0,0,1,1,1,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1,1,0,0,0,1,1,0,1},
      {0,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,0,1,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,1,1,0,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,1,1,0,1,0},
      {0,0,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,1,0,0,0,0,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,0,1,1},
      {0,0,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,0,0,1,0,0,1,0,0,1,0,1,0,1},
      {0,0,1,1,1,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,0,0,0,1},
      {0,0,1,1,1,1,0,1,0,1,1,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0},
      {0,0,1,1,1,1,0,1,0,0,1,1,0,1,1,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,1},
      {0,0,1,1,1,1,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,1,0,1,1,0},
      {0,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,1,0,0},
      {0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,0,1,0,0,1,0,0,1,0,1,1,1},
      {0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,1,0,0,0,0,1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0,0,1,1,1,1,1,0,1,1,0,1,1,0,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,0},
      {0,0,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1},
      {0,0,1,1,1,0,1,1,1,1,0,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,1,1,0,1,0,0,1,1,0,1,1,0,1,0,0,1,1,1,1},
      {0,0,1,1,1,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,0,0,1,0,0,1,0,1,0,0,1,0},
      {0,0,1,1,1,0,1,1,0,0,1,0,0,0,1,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,1,0,1,0,0,1,1,0,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0},
      {0,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,1,0,0,1,0,1,1,0,1,0,0,0,0,1,1,1,1,0,1,0,1,1,1,0,1,1,0,1,0,0,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0},
      {0,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,1,0,1,0,1,1,1,0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1},
      {0,0,1,1,1,0,1,0,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1,0,1,0,0,1,1,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0},
      {0,0,1,1,1,0,1,0,0,1,0,0,1,1,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0},
      {0,0,1,1,1,0,1,0,0,0,0,1,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,0,0},
      {0,0,1,1,1,0,0,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,1,0,0,0,0,1,0,1,0,1,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,1,0,0,1,1,0,0,1,0,1,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,1,1,1,1,0,0,1,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0},
      {0,0,1,1,1,0,0,1,0,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,0},
      {0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,1,1,0,1,1,0,1,0,0,1,1,0,0,0,1,0,0,1,0,1,0},
      {0,0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,0,1,1,1,1},
      {0,0,1,1,1,0,0,0,1,0,1,0,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,0,1,1},
      {0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,1},
      {0,0,1,1,1,0,0,0,0,0,1,1,1,0,1,1,0,0,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,0,1,1,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,0},
      {0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,1,1,1,0,0,1,0,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,1},
      {0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,1,1,1,0,1},
      {0,0,1,1,0,1,1,1,1,0,0,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,0,1,0,0,0},
      {0,0,1,1,0,1,1,1,0,1,1,0,0,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1},
      {0,0,1,1,0,1,1,1,0,0,1,1,0,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,0,1,0,0,0},
      {0,0,1,1,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1,0,1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0},
      {0,0,1,1,0,1,1,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,1,1,1,0,0,0,1,0,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,0,1,1,0,0,1,0,1,1,1,0,1,0,0,1,1,1,0,1,0,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,0,1,1,1,1,0,0,1,1,1,0},
      {0,0,1,1,0,1,1,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,0},
      {0,0,1,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,1,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,0,0,1,0,0,1,1},
      {0,0,1,1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,0},
      {0,0,1,1,0,1,0,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,1,0,0,1,1,0,0},
      {0,0,1,1,0,1,0,1,0,1,0,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,1,0,1,1,0,1,0,0,0,0,0,1,1,1,1,0,1,1,1,0,0,0,0},
      {0,0,1,1,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,1,1,0,1} },
   {
      {0,0,1,1,1,1,1,1,1,1,1,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,1},
      {0,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,1,0},
      {0,0,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,0,0,1,1,1,0,1,1},
      {0,0,1,1,1,1,1,1,0,1,0,0,1,1,0,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,1,0,0,1,0,0,1,0},
      {0,0,1,1,1,1,1,1,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,0,0,0,0,0,1,1,1,1},
      {0,0,1,1,1,1,1,0,1,1,1,0,0,0,1,0,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,0,1,0,0,1,0,1,0,0,1,1,0,0,1,1,0,0,1,0,1,0,1,0,1,1,0,0,1,1,1,1,1,1},
      {0,0,1,1,1,1,1,0,1,0,1,0,1,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,1,1,1,0,1,1,1,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,1,1,0,0,0,1},
      {0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,0,1,0,0,0,1,1,0,0,0,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,0,1,1,0,1,0,0,1,1,1,0,1,1,1,0,0,0,0,0,1},
      {0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,1,0,1,0,1,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,0,1,0,0,1,0,1,0,1,1,1,0,1,1,0,0,0,1,0,1,1,0,0,1,1,1},
      {0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,0,0,0,1,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,1,0,1,0,1,1,1,0,0,1,0},
      {0,0,1,1,1,1,0,1,1,1,0,1,1,0,0,0,1,0,1,1,1,1,0,1,0,0,1,0,1,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,0},
      {0,0,1,1,1,1,0,1,1,0,1,0,0,0,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,1,0,0,1,1},
      {0,0,1,1,1,1,0,1,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,1,0},
      {0,0,1,1,1,1,0,1,0,0,1,1,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,1,1,0},
      {0,0,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,1,1,0,0,1,0,0,1,0,1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0},
      {0,0,1,1,1,1,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,1,1,1,1,0,1,0},
      {0,0,1,1,1,1,0,0,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,1,0,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1,0,0,1,0,1,0,0,1},
      {0,0,1,1,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1},
      {0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,0,0,0,0,0,0,1},
      {0,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,0,1,0,0,1,0,1,1,1,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,0,1,0,0,0},
      {0,0,1,1,1,0,1,1,1,1,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,1,0,1,0,1,0,0,0,0,0,1,1,0,1,1,1,0,1,0,1,1,1,0,0,1},
      {0,0,1,1,1,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1},
      {0,0,1,1,1,0,1,1,0,1,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,1,1,1,0,0},
      {0,0,1,1,1,0,1,1,0,0,1,0,0,1,0,1,1,1,0,0,0,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,0,1,1,1,1,1,0,0},
      {0,0,1,1,1,0,1,0,1,1,1,1,0,0,0,1,0,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,1,0,1,0,0,1,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,1,0,0,1,0,1,0},
      {0,0,1,1,1,0,1,0,1,0,1,1,1,0,1,1,1,1,0,1,1,0,1,0,1,0,0,0,1,0,0,0,1,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,1,1,1,0,1,1,1},
      {0,0,1,1,1,0,1,0,1,0,0,0,0,1,1,0,0,1,0,0,1,0,0,0,0,1,1,0,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1},
      {0,0,1,1,1,0,1,0,0,1,0,1,0,0,0,1,1,1,0,1,0,0,1,1,1,0,0,0,1,0,1,0,1,1,0,0,1,0,1,1,0,0,0,0,1,1,0,1,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0},
      {0,0,1,1,1,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,0,1,1,0,1,0,1,0,1,0,1,1,0,1,1,1,1,0,0,1,1,1,1,0,1,1,1,0,0,0,1,0,0,1,1,0,1,1,0,0,0,1},
      {0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,0,1,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,0,1,1,0,0,1,0,0,1,0,0,1,1,1,0,1,0,1,1,1,1,0,1,0,0},
      {0,0,1,1,1,0,0,1,1,0,1,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,1,1,0,1,0,1,0,0,1,0,1,0,1,1,0,0,1,0,0,0,0},
      {0,0,1,1,1,0,0,1,0,1,1,1,1,1,0,1,0,0,1,1,0,1,0,0,1,1,1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0},
      {0,0,1,1,1,0,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,1,0,1,1,0,0,0,0,1,1,0,1,0,1,0,0,1,1,0,1,0,1,1,0,0,0,1,0,1,1,0,0,0,1,1,0,1,0,0,0,1,1,1},
      {0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,1,0,1,0,0,1,1},
      {0,0,1,1,1,0,0,0,1,1,0,1,1,1,0,1,1,1,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,1,1,0,1,1,1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1},
      {0,0,1,1,1,0,0,0,1,0,1,0,0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,1,0,1,1,0,0,1,0,0,1,0,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,1},
      {0,0,1,1,1,0,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,0,1,0,1,1,0,1,1,0,1,1,1,1,0,1,1,1,0,1,0,0,0,0,0,0,0,1},
      {0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,0,1,0,1,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,1},
      {0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,0,0,1,1,0,0,1,1,0,1,0,1},
      {0,0,1,1,0,1,1,1,1,1,0,1,0,0,1,1,1,0,0,1,1,0,0,1,1,0,1,0,1,0,1,0,1,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,1},
      {0,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,1,0,1,1,1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,1,0,1,1,1,0,0,0,0,0,1,1,0,1,0,1,0,1,0,0,0,0,0,0,1},
      {0,0,1,1,0,1,1,1,0,1,1,0,1,0,0,1,0,0,0,1,0,1,1,0,1,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,0,1,1,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1},
      {0,0,1,1,0,1,1,1,0,0,1,1,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,1,0,0,1,1,1,1,0,1,1,0,1,1,0,1,0,0,1,1,0,0,1,1,0,1},
      {0,0,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,0,1,1,1,0,0,1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,1,0,1,1},
      {0,0,1,1,0,1,1,0,1,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,1,0,1,1,0,1,1,1,1,0,0,1,0,1,1,0,0,0,0,0,0,1,0,0,0,1},
      {0,0,1,1,0,1,1,0,1,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1},
      {0,0,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,0,1,1,0,1,0,1,0,1,1,0,1,1,0,0,0,1,1,0,0,1,1,0,1,0,0,1,0,0},
      {0,0,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,0,0,1,1,1,0,1,0,0,1,1,0,0,1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,0,1,0,1,1,1,0,1,0,0,1,1},
      {0,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,0,0,0,1,0,1,1,1,0,1,0,1,1,0,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,0,0,1,0,1,1,1,0,1,0,1},
      {0,0,1,1,0,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,0,1,0,1,0,0,0,0,1,0,1,1,1,1,0,1,0,0,1,1,1,1,0,0,1,0,0,0,1},
      {0,0,1,1,0,1,0,1,1,0,0,0,1,0,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,1,1,0,1,1,1,0,0,1,1,1,1,0,0,1,0,1,0,1,0,0,1,0,1,1,0,1,1,0,1,0,1},
      {0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,1,1,0},
      {0,0,1,1,0,1,0,1,0,0,1,0,0,0,0,1,0,0,1,1,1,1,0,1,1,0,0,1,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,0,0,1,0,1,1} }};
  bool is_greater;
  for (int i = 0; i < x.size(); i++) {
    cur_decimal = ref_dq[x[i]][i];
    for (i2 = 0; i2 < rtn_dq.size(); i2++) {
      is_greater = IsSuperiorDouble(cur_decimal, ref_val[i2]);
      if (is_greater) {
        if (!rtn_dq[i2]) {
          rtn_dq[i2] = 1;
        } else {
          rtn_dq[i2] = 0;
          i3 = i2 - 1;
          while (rtn_dq[i3]) {
            rtn_dq[i3] = 0;
            i3 -= 1;
          };
          rtn_dq[i3] = 1;
        };
        while (is_greater) {
          cur_decimal = IEEE754DoubleToDoubleSubstraction(cur_decimal, ref_val[i2]);
          is_greater = IsSuperiorDouble(cur_decimal, ref_val[i2]);
        };
        if (cur_decimal == ref_dq[0][0]) {
          break;
        };
      };
    };
  };
  return rtn_dq;
};

//@T FloatDivide
//@U std::deque&lt;bool&gt; FloatDivide(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Returns the result of the first float divided by the second float as its bynary format, IEEE754.
//@A x : is the first float, as its binary IEEE754 format
//@A x : is the second float, as its binary IEEE754 format
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(float)];
//@E FloatStore obj1;
//@E obj1.value = 4560.1;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 01000101100011101000000011001101
//@E FloatStore obj2;
//@E obj2.value = 3284.20;
//@E std::cout &lt;&lt; std::setprecision(9) &lt;&lt; "\n";
//@E memcpy(rslt_arr, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; dq2[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 01000101010011010100001100110011
//@E std::deque&lt;bool&gt; dq3 = FloatDivide(dq2, dq);
//@E float intended_rslt = obj2.value / obj1.value;
//@E std::cout &lt;&lt; "intended_result: " &lt;&lt; intended_rslt &lt;&lt; "\n intended_dq:\n";
//@E 0.720203459
//@E obj1.value = intended_rslt;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E dq = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; dq.size(); i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00111111001110000101111101000001
//@E BinaryToByteFloat(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(float));
//@E std::cout &lt;&lt; "dq3 size: " &lt;&lt; dq3.size() &lt;&lt; "\n";
//@E 32
//@E for (i = 0; i &lt; dq3.size(); i++) {
//@E   std::cout &lt;&lt; dq3[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 00111111001110000101111100111100
//@E std::cout &lt;&lt; obj1.value &lt;&lt; "\n";
//@E 0.720203161
//@X

std::deque<bool> FloatDivide(std::deque<bool> x, std::deque<bool> x2) {
  std::deque<bool> rtn_dq = {};
  if (x[0] == x2[0]) {
    rtn_dq.push_back(0);
  } else {
    rtn_dq.push_back(1);
  };
  std::deque<bool> ref_x = x;
  std::deque<bool> ref_x2 = x2;
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
  if (mantissa_dq != ref_zero) {
    while (!mantissa_dq[0]) {
      i += 1;
      if (i == 23) {
        break;
      };
      mantissa_dq.pop_front();
    };
    mantissa_dq.pop_front();
    exponent = 22 - i + 127;
    exponent_dq = int_to_binarydq(exponent);
  } else {
    is_greater = 0;
    cur_float = x;
    rest_iterations = 0;
    while (!is_greater) {
      cur_float = IEEE754FloatToFloatAddition(cur_float, cur_float);
      is_greater = IsSuperiorFloatOrEq(cur_float, x2); 
      rest_iterations += 1;
    };
    exponent = 127 - rest_iterations;
    exponent_dq = int_to_binarydq(exponent);
    mantissa_dq = {};
    i = 22;
  };
  while (exponent_dq.size() < 8) {
    exponent_dq.push_front(0);
  };
  std::deque<bool> mantissa_decimal_dq = {};
  while (mantissa_dq.size() + mantissa_decimal_dq.size() < 23) {
    mantissa_decimal_dq.push_back(0);
  };
  rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
  i = 22 - i;
  int hmn = 0;
  std::deque<int> decimals_dq = {};
  std::deque<std::deque<bool>> ref_one_decimal = {
     {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1},

     {0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0},

     {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1},

     {1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1},

     {0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0},

     {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},

     {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1},

     {0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1},

     {0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1},

     {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},

     {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

     {0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},

     {1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1},

     {0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0},

     {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1},

     {1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1},

     {0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0},

     {0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},

     {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0},

     {0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},

     {0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},

     {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},

     {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0},

};
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
      x = IEEE754FloatToFloatSubstraction(x, cur_float);
    } else {
      break;
    };
    };
    i += 1;
    decimals_dq.push_back(hmn);
    hmn = 0;
  };
  if (i == 23 || hmn != 9) {
    mantissa_decimal_dq = DecimalToBinary2(decimals_dq, mantissa_decimal_dq);
    bool is_greater = 0;
    for (i = 1; i < 32; i++) {
      if (ref_x2[i] && !ref_x[i]) {
        break;
      } else if (!ref_x2[i] && ref_x[i]) {
        is_greater = 1;
        break;
      };
    };
    if (!is_greater) {
      while (!mantissa_decimal_dq[0]) {
        mantissa_decimal_dq.pop_front();
        mantissa_decimal_dq.push_back(0);
      };
      mantissa_decimal_dq.pop_front();
      mantissa_decimal_dq.push_back(0);
    };
  } else {
    mantissa_decimal_dq = ref_one_decimal[i - 1];
  };

  rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
  rtn_dq.insert(rtn_dq.end(), mantissa_decimal_dq.begin(), mantissa_decimal_dq.end());
  return rtn_dq;
};

//@T FloatMultiplyPos
//@U std::deque&lt;bool&gt; FloatMultiplyIntPos(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Performs a multiplication on the first argument, that is a float, and the second, which is a positive integer.
//@A x : is the IEEE754 binary representation of the float to multiply
//@A x2 : is the binary representation of the integer
//@X
//@E FloatStore obj1;
//@E IntStore obj2;
//@E unsigned char rslt_arr[sizeof(float)];
//@E obj1.value = -44.36;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq1 = ByteToBinaryFloat(rslt_arr);
//@E obj2.x = 2;
//@E float intended_rslt = obj1.value * obj2.x;
//@E std::cout &lt;&lt; std::setprecision(9);
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; intended_rslt &lt;&lt; "\n";
//@E -88.7200012
//@E memcpy(rslt_arr, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr);
//@E std::deque&lt;bool&gt; dq3 = FloatMultiplyIntPos(dq1, dq2);
//@E BinaryToByteFloat(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(float));
//@E std::cout &lt;&lt; "result: " &lt;&lt; obj1.value &lt;&lt; "\n";
//@E -88.7200012
//@X

std::deque<bool> FloatMultiplyIntPos(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool> ref_cnt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i;
  bool is_equal = 1;
  for (i = 0; i < 32; i++) {
    if (ref_cnt[i] != x2[i]) {
      is_equal = 0;
      break;
    };
  };
  if (is_equal) {
    return ref_cnt;
  };
  std::deque<bool> rtn_dq = x;
  ref_cnt[31] = 1;
  is_equal = 1;
  for (i = 0; i < 32; i++) {
    if (ref_cnt[i] != x2[i]) {
      is_equal = 0;
      break;
    };
  };
  while (!is_equal) {
    rtn_dq = IEEE754FloatToFloatAddition(rtn_dq, x);
    if (!ref_cnt[31]) {
      ref_cnt[31] = 1;
    } else {
      i = 31;
      while (ref_cnt[i]) {
        ref_cnt[i] = 0;
        i -= 1;
      };
      ref_cnt[i] = 1;
    };
    is_equal = 1;
    for (i = 0; i < 32; i++) {
      if (ref_cnt[i] != x2[i]) {
        is_equal = 0;
        break;
      };
    };
  };
  return rtn_dq;
};

//@T FloatMultiplyNegative
//@U std::deque&lt;bool&gt; FloatMultiplyIntNegative(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Performs a multiplication on the first argument, that is a float, and the second, which is a negative integer.
//@A x : is the IEEE754 binary representation of the float to multiply
//@A x2 : is the binary representation of the integer
//@X
//@E FloatStore obj1;
//@E IntStore obj2;
//@E unsigned char rslt_arr[sizeof(float)];
//@E obj1.value = -44.36;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq1 = ByteToBinaryFloat(rslt_arr);
//@E obj2.x = -12;
//@E float intended_rslt = obj1.value * obj2.x;
//@E std::cout &lt;&lt; std::setprecision(9);
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; intended_rslt &lt;&lt; "\n";
//@E 532.320007
//@E memcpy(rslt_arr, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr);
//@E std::deque&lt;bool&gt; dq3 = FloatMultiplyIntNegative(dq1, dq2);
//@E BinaryToByteFloat(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(float));
//@E std::cout &lt;&lt; "result: " &lt;&lt; obj1.value &lt;&lt; "\n";
//@E 532.319885
//@X

std::deque<bool> FloatMultiplyIntNegative(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool> ref_cnt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i;
  bool is_equal = 1;
  for (i = 0; i < 32; i++) {
    if (ref_cnt[i] != x2[i]) {
      is_equal = 0;
      break;
    };
  };
  if (is_equal) {
    return ref_cnt;
  };
  std::deque<bool> rtn_dq = x;
  ref_cnt = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  is_equal = 1;
  for (i = 0; i < 32; i++) {
    if (ref_cnt[i] != x2[i]) {
      is_equal = 0;
      break;
    };
  };
  while (!is_equal) {
    rtn_dq = IEEE754FloatToFloatAddition(rtn_dq, x);
    if (ref_cnt[31]) {
      ref_cnt[31] = 0;
    } else {
      i = 31;
      while (!ref_cnt[i]) {
        ref_cnt[i] = 1;
        i -= 1;
      };
      ref_cnt[i] = 0;
    };
    is_equal = 1;
    for (i = 0; i < 32; i++) {
      if (ref_cnt[i] != x2[i]) {
        is_equal = 0;
        break;
      };
    };
  };
  rtn_dq[0] = !rtn_dq[0];
  return rtn_dq;
};

//@T DoubleMultiplyPos
//@U std::deque&lt;bool&gt; DoubleMultiplyIntPos(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Performs a multiplication on the first argument, that is a double, and the second, which is a positive integer.
//@A x : is the IEEE754 binary representation of the double to multiply
//@A x2 : is the binary representation of the integer
//@X
//@E DoubleStore obj1;
//@E IntStore obj2;
//@E unsigned char rslt_arr[sizeof(double)];
//@E unsigned char rslt_arr2[sizeof(int)];
//@E obj1.value = 44.36;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq1 = ByteToBinaryDouble(rslt_arr);
//@E obj2.x = 112;
//@E double intended_rslt = obj1.value * obj2.x;
//@E std::cout &lt;&lt; std::setprecision(9);
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; intended_rslt &lt;&lt; "\n";
//@E 4968.32
//@E memcpy(rslt_arr2, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr2);
//@E std::deque&lt;bool&gt; dq3 = DoubleMultiplyIntPos(dq1, dq2);
//@E BinaryToByteDouble(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; "result: " &lt;&lt; obj1.value &lt;&lt; "\n";
//@E 4968.32
//@X

std::deque<bool> DoubleMultiplyIntPos(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool> ref_cnt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i;
  bool is_equal = 1;
  for (i = 0; i < 32; i++) {
    if (ref_cnt[i] != x2[i]) {
      is_equal = 0;
      break;
    };
  };
  if (is_equal) {
    return ref_cnt;
  };
  std::deque<bool> rtn_dq = x;
  ref_cnt[31] = 1;
  is_equal = 1;
  for (i = 0; i < 32; i++) {
    if (ref_cnt[i] != x2[i]) {
      is_equal = 0;
      break;
    };
  };
  while (!is_equal) {
    rtn_dq = IEEE754DoubleToDoubleAddition(rtn_dq, x);
    if (!ref_cnt[31]) {
      ref_cnt[31] = 1;
    } else {
      i = 31;
      while (ref_cnt[i]) {
        ref_cnt[i] = 0;
        i -= 1;
      };
      ref_cnt[i] = 1;
    };
    is_equal = 1;
    for (i = 0; i < 32; i++) {
      if (ref_cnt[i] != x2[i]) {
        is_equal = 0;
        break;
      };
    };
  };
  return rtn_dq;
};

//@T DoubleMultiplyNegative
//@U std::deque&lt;bool&gt; DoubleMultiplyIntNegative(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Performs a multiplication on the first argument, that is a double, and the second, which is a negative integer.
//@A x : is the IEEE754 binary representation of the double to multiply
//@A x2 : is the binary representation of the integer
//@X
//@E DoubleStore obj1;
//@E IntStore obj2;
//@E unsigned char rslt_arr[sizeof(double)];
//@E unsigned char rslt_arr2[sizeof(int)];
//@E obj1.value = 44.36;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq1 = ByteToBinaryDouble(rslt_arr);
//@E obj2.x = -112;
//@E double intended_rslt = obj1.value * obj2.x;
//@E std::cout &lt;&lt; std::setprecision(9);
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; intended_rslt &lt;&lt; "\n";
//@E -4968.32
//@E memcpy(rslt_arr2, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr2);
//@E std::deque&lt;bool&gt; dq3 = DoubleMultiplyIntNegative(dq1, dq2);
//@E BinaryToByteDouble(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; "result: " &lt;&lt; obj1.value &lt;&lt; "\n";
//@E -4968.32
//@X

std::deque<bool> DoubleMultiplyIntNegative(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool> ref_cnt = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 00, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i;
  bool is_equal = 1;
  for (i = 0; i < 32; i++) {
    if (ref_cnt[i] != x2[i]) {
      is_equal = 0;
      break;
    };
  };
  if (is_equal) {
    return ref_cnt;
  };
  std::deque<bool> rtn_dq = x;
  ref_cnt = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  is_equal = 1;
  for (i = 0; i < 32; i++) {
    if (ref_cnt[i] != x2[i]) {
      is_equal = 0;
      break;
    };
  };
  while (!is_equal) {
    rtn_dq = IEEE754DoubleToDoubleAddition(rtn_dq, x);
    if (ref_cnt[31]) {
      ref_cnt[31] = 0;
    } else {
      i = 31;
      while (!ref_cnt[i]) {
        ref_cnt[i] = 1;
        i -= 1;
      };
      ref_cnt[i] = 0;
    };
    is_equal = 1;
    for (i = 0; i < 32; i++) {
      if (ref_cnt[i] != x2[i]) {
        is_equal = 0;
        break;
      };
    };
  };
  rtn_dq[0] = !rtn_dq[0];
  return rtn_dq;
};

//@T FloatMultiplyInt
//@U std::deque&lt;bool&gt; FloatMultiplyInt(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Performs a multiplication between the first argument, a float, and the second argument, the integer (positive or negative).
//@A x : the binary representation of the float
//@A x2 : the binary representation of the signed integer
//@X
//@E FloatStore obj1;
//@E IntStore obj2;
//@E unsigned char rslt_arr[sizeof(float)];
//@E unsigned char rslt_arr2[sizeof(int)];
//@E obj1.value = 44.36;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq1 = ByteToBinaryFloat(rslt_arr);
//@E obj2.x = -112;
//@E float intended_rslt = obj1.value * obj2.x;
//@E std::cout &lt;&lt; std::setprecision(9);
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; intended_rslt &lt;&lt; "\n";
//@E -4968.32031
//@E memcpy(rslt_arr2, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr2);
//@E std::deque&lt;bool&gt; dq3 = FloatMultiplyInt(dq1, dq2);
//@E BinaryToByteFloat(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(float));
//@E std::cout &lt;&lt; "result: " &lt;&lt; obj1.value &lt;&lt; "\n";
//@E -4968.31006
//@X

std::deque<bool> FloatMultiplyInt(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool> rtn_dq = {};
  if (x2[0]) {
    rtn_dq = FloatMultiplyIntNegative(x, x2);
  } else {
    rtn_dq = FloatMultiplyIntPos(x, x2);
  };
  return rtn_dq;
};

//@T DoubleMultiplyInt
//@U std::deque&lt;bool&gt; DoubleMultiplyInt(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Performs a multiplication between the first argument, a double, and the second argument, the integer (positive or negative).
//@A x : the binary representation of the double
//@A x2 : the binary representation of the signed integer
//@X
//@E DoubleStore obj1;
//@E IntStore obj2;
//@E unsigned char rslt_arr[sizeof(double)];
//@E unsigned char rslt_arr2[sizeof(int)];
//@E obj1.value = 44.36;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq1 = ByteToBinaryDouble(rslt_arr);
//@E obj2.x = 112;
//@E double intended_rslt = obj1.value * obj2.x;
//@E std::cout &lt;&lt; std::setprecision(9);
//@E std::cout &lt;&lt; "intended result: " &lt;&lt; intended_rslt &lt;&lt; "\n";
//@E 4968.32
//@E memcpy(rslt_arr2, obj2.x_array, sizeof(int));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryInt(rslt_arr2);
//@E std::deque&lt;bool&gt; dq3 = DoubleMultiplyInt(dq1, dq2);
//@E BinaryToByteDouble(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; "result: " &lt;&lt; obj1.value &lt;&lt; "\n";
//@E 4968.32
//@X

std::deque<bool> DoubleMultiplyInt(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool> rtn_dq = {};
  if (x2[0]) {
    rtn_dq = DoubleMultiplyIntNegative(x, x2);
  } else {
    rtn_dq = DoubleMultiplyIntPos(x, x2);
  };
  return rtn_dq;
};

//@T DoubleDivide
//@U std::deque&lt;bool&gt; DoubleDivide(std::deque&lt;bool&gt; x, std::deque&lt;bool&gt; x2)
//@X
//@D Returns the result of the first double divided by the second double as its bynary format, IEEE754.
//@A x : is the first double, as its binary IEEE754 format
//@A x : is the second double, as its binary IEEE754 format
//@X
//@E int i;
//@E unsigned char rslt_arr[sizeof(double)];
//@E DoubleStore obj1;
//@E obj1.value = 1.16;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0011111111110010100011110101110000101000111101011100001010001111
//@E DoubleStore obj2;
//@E obj2.value = 230.142;
//@E std::cout &lt;&lt; std::setprecision(9) &lt;&lt; "\n";
//@E memcpy(rslt_arr, obj2.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; dq2[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000001101100110001001000101101000011100101011000000100000110
//@E std::deque&lt;bool&gt; dq3 = DoubleDivide(dq2, dq);
//@E double intended_rslt = obj2.value / obj1.value;
//@E std::cout &lt;&lt; "intended_result: " &lt;&lt; intended_rslt &lt;&lt; "\n intended_dq:\n";
//@E 198.398276
//@E obj1.value = intended_rslt;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E dq = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; dq.size(); i++) {
//@E   std::cout &lt;&lt; dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000001101000110011001011111010101101000001010100101111101011
//@E BinaryToByteDouble(dq3, rslt_arr);
//@E memcpy(obj1.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; "dq3 size: " &lt;&lt; dq3.size() &lt;&lt; "\n";
//@E 64
//@E for (i = 0; i &lt; dq3.size(); i++) {
//@E   std::cout &lt;&lt; dq3[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0100000001101000110011001011111010101101000001010100101111110111
//@E std::cout &lt;&lt; obj1.value &lt;&lt; "\n";
//@E 198.398276
//@X

std::deque<bool> DoubleDivide(std::deque<bool> x, std::deque<bool> x2) {
  std::deque<bool> rtn_dq = {};
  if (x[0] == x2[0]) {
    rtn_dq.push_back(0);
  } else {
    rtn_dq.push_back(1);
  };
  std::deque<bool> ref_x = x;
  std::deque<bool> ref_x2 = x2;
  int i = 0;
  int i2;
  int exponent;
  int rest_iterations = 0;
  std::deque<bool> ref_zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<bool> exponent_dq = {};
  std::deque<bool> cur_double = x2;
  std::deque<bool> mantissa_dq = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<bool> just_one = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  std::deque<bool> just_one2 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
  bool is_greater = IsSuperiorDouble(cur_double, x);
  double decimal_counter = 0;
  while (!is_greater) {
    cur_double = IEEE754DoubleToDoubleAddition(cur_double, x2);
    IntSameTypeAddition2(mantissa_dq, just_one);
    is_greater = IsSuperiorDouble(cur_double, x);
  };
  if (cur_double != x) {
    cur_double = IEEE754DoubleToDoubleSubstraction(cur_double, x2);
    x = IEEE754DoubleToDoubleSubstraction(x, cur_double);
  };
  i = 0;
  if (mantissa_dq != ref_zero) {
    while (!mantissa_dq[0]) {
      i += 1;
      if (i == 52) {
        break;
      };
      mantissa_dq.pop_front();
    };
    mantissa_dq.pop_front();
    exponent = 51 - i + 1023;
    exponent_dq = int_to_binarydq(exponent);
  } else {
    is_greater = 0;
    cur_double = x;
    rest_iterations = 0;
    while (!is_greater) {
      cur_double = IEEE754DoubleToDoubleAddition(cur_double, cur_double);
      is_greater = IsSuperiorDoubleOrEq(cur_double, x2);
      rest_iterations += 1;
    };
    exponent = 1023 - rest_iterations;
    exponent_dq = int_to_binarydq(exponent);
    mantissa_dq = {};
    i = 22;
  };
  while (exponent_dq.size() < 11) {
    exponent_dq.push_front(0);
  };
  std::deque<bool> mantissa_decimal_dq = {};
  while (mantissa_dq.size() + mantissa_decimal_dq.size() < 52) {
    mantissa_decimal_dq.push_back(0);
  };
  rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
  i = 51 - i;
  int hmn = 0;
  std::deque<int> decimals_dq = {};
  std::deque<std::deque<bool>> ref_one_decimal = {
     {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0},

     {0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1},

     {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},

     {1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1},

     {0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},

     {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1},

     {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0},

     {0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0},

     {0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1},

     {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1},

     {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1},

     {0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1},

     {1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0},

     {0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1},

     {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0},

     {1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0},

     {0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1},

     {0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0},

     {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0},

     {0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1},

     {0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1},

     {1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0},

     {1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0},

     {0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0},

     {1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0},

     {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1},

     {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},

     {1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0},

     {1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0},

     {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0},

     {0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},

     {1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0},

     {0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0},

     {0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0},

     {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1},

     {0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1},

     {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1},

     {1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0},

     {0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1},

     {0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1},

     {1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0},

     {0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1},

     {0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1},

     {1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0},

     {0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0},

     {0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1},

     {1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0},

     {0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},

     {0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1},

     {1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0},

     {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0},

     {0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0},

     {0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1}};
  while (i < 52) {
    cur_double = x;
    for (i2 = 0; i2 < 9; i2++) {
      x = IEEE754DoubleToDoubleAddition(x, cur_double);
    };
    cur_double = x2;
    is_greater = IsSuperiorDoubleOrEq(cur_double, x);
    if (!is_greater) {
      while (!is_greater) {
        hmn += 1;
        cur_double = IEEE754DoubleToDoubleAddition(cur_double, x2);
        IntSameTypeAddition2(mantissa_dq, just_one);
        is_greater = IsSuperiorDoubleOrEq(cur_double, x);
      }; 
      if (cur_double != x) {
        cur_double = IEEE754DoubleToDoubleSubstraction(cur_double, x2);
        x = IEEE754DoubleToDoubleSubstraction(x, cur_double);
      } else {
        break;
      };
    };
    i += 1;
    decimals_dq.push_back(hmn);
    hmn = 0;
  };
  if (i == 52 || hmn != 9) {
    mantissa_decimal_dq = DecimalToBinary2b(decimals_dq, mantissa_decimal_dq);
    bool is_greater = 0;
    for (i = 1; i < 64; i++) {
      if (ref_x2[i] && !ref_x[i]) {
        break;
      } else if (!ref_x2[i] && ref_x[i]) {
        is_greater = 1;
        break;
      };
    };
    if (!is_greater) {
      while (!mantissa_decimal_dq[0]) {
        mantissa_decimal_dq.pop_front();
        mantissa_decimal_dq.push_back(0);
      };
      mantissa_decimal_dq.pop_front();
      mantissa_decimal_dq.push_back(0);
    };
  } else {
    mantissa_decimal_dq = ref_one_decimal[i - 1];
  };
  rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
  rtn_dq.insert(rtn_dq.end(), mantissa_decimal_dq.begin(), mantissa_decimal_dq.end());
  return rtn_dq;
};

//@T DecimalFloat
//@U std::deque&lt;bool&gt; DecimalFloat(std::deque&lt;bool&gt; &x)
//@X
//@D Converts the input IEEE754 binary representation of the input float to its IEEE754 binary representation for its decimal.
//@A x : the IEEE754 binary representation of the float
//@X
//@E FloatStore obj1;
//@E obj1.value = -20.90687;
//@E int i;
//@E unsigned char rslt_arr[sizeof(float)];
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; cur_dq = ByteToBinaryFloat(rslt_arr);
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; cur_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11000001101001110100000101000101
//@E cur_dq = DecimalFloat(cur_dq);
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; cur_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 10111111011010000010100010100000
//@E float intended_val = obj1.value - int(obj1.value);
//@E std::cout &lt;&lt; "intended_val: \n";
//@E -0.90687
//@E std::cout &lt;&lt; intended_val &lt;&lt; "\n";
//@E obj1.value = intended_val;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E cur_dq = ByteToBinaryFloat(rslt_arr);
//@E std::cout &lt;&lt; "intended deque: \n";
//@E for (i = 0; i &lt; 32; i++) {
//@E   std::cout &lt;&lt; cur_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 10111111011010000010100010100000
//@X

std::deque<bool> DecimalFloat(std::deque<bool> &x) {
  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> exponent_dq = {};
  double exponent_val = 0;
  double exponent_val2 = 128;
  int i = 1;
  int i2;
  std::deque<bool> mantissa_dq = {};
  while (i < 8) {
    exponent_dq.push_back(x[i]);
    if (x[i]) {
      exponent_val += exponent_val2;
    };
    exponent_val2 /= 2;
    i += 1;
  };
  exponent_dq.push_back(x[i]);
  if (x[i]) {
    exponent_val += 1;
  };
  if (exponent_val < 127) {
    return x;
  };
  while (exponent_val > 127) {
    exponent_val -= 1;
    i += 1;
  };
  i += 1;
  while (!x[i]) {
    i += 1;
  };
  i += 1;
  while (i < 32) {
    mantissa_dq.push_back(x[i]);
    i += 1;
  };
  while (mantissa_dq.size() < 23) {
    mantissa_dq.push_back(0);
    if (exponent_dq[7]) {
      exponent_dq[7] = 0;
    } else {
      i2 = 7;
      while (!exponent_dq[i2]) {
        exponent_dq[i2] = 1;
        i2 -= 1;
      };
      exponent_dq[i2] = 0;
    };
  };
  rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
  rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
  return rtn_dq;
};

//@T DecimalDouble
//@U std::deque&lt;bool&gt; DecimalDouble(std::deque&lt;bool&gt; &x)
//@X
//@D Converts the input IEEE754 binary representation of the input double to its IEEE754 binary representation for its decimal.
//@A x : the IEEE754 binary representation of the double
//@X
//@E DoubleStore obj1;
//@E DoubleStore obj2;
//@E obj1.value = 20.90687;
//@E int i;
//@E unsigned char rslt_arr[sizeof(double)];
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; cur_dq = ByteToBinaryDouble(rslt_arr);
//@E for (i = 0; i &lt; 64; i++) {
//@E   std::cout &lt;&lt; cur_dq[i];
//@E };
//@E 0100000000110100111010000010100010100001110111111011100100111001
//@E std::cout &lt;&lt; "\n cur_dq:\n";
//@E cur_dq = DecimalDouble(cur_dq);
//@E for (i = 0; i &lt; 64; i++) {
//@E   std::cout &lt;&lt; cur_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0011111111101101000001010001010000111011111101110010011100100000
//@E BinaryToByteDouble(cur_dq, rslt_arr);
//@E memcpy(obj2.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; "actual value: " &lt;&lt; obj2.value &lt;&lt; "\n";
//@E 0.90687
//@E double intended_val = obj1.value - int(obj1.value);
//@E std::cout &lt;&lt; "intended_val: \n";
//@E std::cout &lt;&lt; intended_val &lt;&lt; "\n";
//@E 0.90687
//@E obj1.value = intended_val;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E cur_dq = ByteToBinaryDouble(rslt_arr);
//@E std::cout &lt;&lt; "intended deque: \n";
//@E for (i = 0; i &lt; 64; i++) {
//@E   std::cout &lt;&lt; cur_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 0011111111101101000001010001010000111011111101110010011100100000
//@X

std::deque<bool> DecimalDouble(std::deque<bool> &x) {
  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> exponent_dq = {};
  double exponent_val = 0;
  double exponent_val2 = 1024;
  int i = 1;
  int i2;
  std::deque<bool> mantissa_dq = {};
  while (i < 11) {
    exponent_dq.push_back(x[i]);
    if (x[i]) {
      exponent_val += exponent_val2;
    };
    exponent_val2 /= 2;
    i += 1;
  };
  exponent_dq.push_back(x[i]);
  if (x[i]) {
    exponent_val += 1;
  };
  if (exponent_val < 1024) {
    return x;
  };
  while (exponent_val > 1024) {
    exponent_val -= 1;
    i += 1;
  };
  i += 1;
  while (!x[i]) {
    i += 1;
  };
  i += 1;
  while (i < 64) {
    mantissa_dq.push_back(x[i]);
    i += 1;
  };
  while (mantissa_dq.size() < 52) {
    mantissa_dq.push_back(0);
    if (exponent_dq[10]) {
      exponent_dq[10] = 0;
    } else {
      i2 = 10;
      while (!exponent_dq[i2]) {
        exponent_dq[i2] = 1;
        i2 -= 1;
      };
      exponent_dq[i2] = 0;
    };
  };
  rtn_dq.insert(rtn_dq.end(), exponent_dq.begin(), exponent_dq.end());
  rtn_dq.insert(rtn_dq.end(), mantissa_dq.begin(), mantissa_dq.end());
  return rtn_dq;
};

//@T FloatMultiplyFloat
//@U std::deque&lt;bool&gt; FloatMultiplyFloat(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Multiplies 2 float together, from their IEEE754 binary format.
//@A x : is the first float
//@A x : is the second float
//@X
//@E FloatStore obj1;
//@E FloatStore obj2;
//@E FloatStore intended_obj;
//@E FloatStore result_obj;
//@E unsigned char rslt_arr[sizeof(float)];
//@E obj1.value = -45.63;
//@E obj2.value = 0.456;
//@E int i;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq1 = ByteToBinaryFloat(rslt_arr);
//@E memcpy(rslt_arr, obj2.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryFloat(rslt_arr);
//@E float intended_result = obj1.value * obj2.value;
//@E intended_obj.value = intended_result;
//@E memcpy(rslt_arr, intended_obj.byte_rep, sizeof(float));
//@E std::deque&lt;bool&gt; intended_dq = ByteToBinaryFloat(rslt_arr);
//@E std::cout &lt;&lt; "intended_dq;\n";
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; intended_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11000001101001100111010101001111
//@E std::cout &lt;&lt; "intended_result: " &lt;&lt; intended_result &lt;&lt; "\n";
//@E -20.8073
//@E std::deque&lt;bool&gt; result_dq = FloatMultiplyFloat(dq1, dq2);
//@E for (i = 0; i &lt; sizeof(float) * 8; i++) {
//@E   std::cout &lt;&lt; intended_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 11000001101001100111010101001111
//@E BinaryToByteFloat(result_dq, rslt_arr);
//@E memcpy(result_obj.byte_rep, rslt_arr, sizeof(float));
//@E std::cout &lt;&lt; "result: " &lt;&lt; result_obj.value &lt;&lt; "\n";
//@E -20.8073
//@X

std::deque<bool> FloatMultiplyFloat(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool> ref_one = {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<bool> ref_zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i = 0;
  bool is_negative = 0;
  if (x[0] != x2[0]) {
    is_negative = 1;
  };
  x[0] = 0;
  x2[0] = 0;
  bool is_zero_int = 1;
  for (i = 1; i < 32; i++) {
    if (ref_zero[i] != x[i]) {
      is_zero_int = 0;
      break;
    };
  };
  if (is_zero_int) {
    return ref_zero;
  };
  is_zero_int = 1;
  for (i = 1; i < 32; i++) {
    if (ref_zero[i] != x2[i]) {
      is_zero_int = 0;
      break;
    };
  };
  if (is_zero_int) {
    return ref_zero;
  };
  std::deque<bool> int_part_dq = FloatToIntBinary(x2);
  std::deque<bool> int_mult_dq = {};
  std::deque<bool> dec_part_dq = {};
  std::deque<bool> decimal_add_dq = {};
  is_zero_int = 1;
  for (i = 0; i < 32; i++) {
    if (ref_zero[i] != int_part_dq[i]) {
      is_zero_int = 0;
      break;
    };
  };
  if (!is_zero_int) {
    int_mult_dq = FloatMultiplyInt(x, int_part_dq);
  } else {
    int_mult_dq = ref_zero;
  };
  dec_part_dq = DecimalFloat(x2);
  dec_part_dq = FloatDivide(ref_one, dec_part_dq);
  decimal_add_dq = FloatDivide(x, dec_part_dq);
  int_mult_dq = IEEE754FloatToFloatAddition(int_mult_dq, decimal_add_dq);
  if (is_negative) {
    int_mult_dq[0] = 1;
  };
  return int_mult_dq;
};

//@T DoubleMultiplyDouble
//@U std::deque&lt;bool&gt; DoubleMultiplyDouble(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)
//@X
//@D Multiplies 2 double together, from their IEEE754 binary format.
//@A x : is the first double
//@A x : is the second double
//@X
//@E DoubleStore obj1;
//@E DoubleStore obj2;
//@E DoubleStore intended_obj;
//@E DoubleStore result_obj;
//@E unsigned char rslt_arr[sizeof(double)];
//@E obj1.value = -45.63;
//@E obj2.value = 0.456;
//@E int i;
//@E memcpy(rslt_arr, obj1.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq1 = ByteToBinaryDouble(rslt_arr);
//@E memcpy(rslt_arr, obj2.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; dq2 = ByteToBinaryDouble(rslt_arr);
//@E double intended_result = obj1.value * obj2.value;
//@E intended_obj.value = intended_result;
//@E memcpy(rslt_arr, intended_obj.byte_rep, sizeof(double));
//@E std::deque&lt;bool&gt; intended_dq = ByteToBinaryDouble(rslt_arr);
//@E std::cout &lt;&lt; "intended_dq;\n";
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; intended_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1100000000110100110011101010100111100110111011101011011100000011
//@E std::cout &lt;&lt; "intended_result: " &lt;&lt; intended_result &lt;&lt; "\n";
//@E -20.8073
//@E std::deque&lt;bool&gt; result_dq = DoubleMultiplyDouble(dq1, dq2);
//@E for (i = 0; i &lt; sizeof(double) * 8; i++) {
//@E   std::cout &lt;&lt; intended_dq[i];
//@E };
//@E std::cout &lt;&lt; "\n";
//@E 1100000000110100110011101010100111100110111011101011011100000011
//@E BinaryToByteDouble(result_dq, rslt_arr);
//@E memcpy(result_obj.byte_rep, rslt_arr, sizeof(double));
//@E std::cout &lt;&lt; "result: " &lt;&lt; result_obj.value &lt;&lt; "\n";
//@E -20.8073
//@X

std::deque<bool> DoubleMultiplyDouble(std::deque<bool> &x, std::deque<bool> &x2) {
  std::deque<bool> ref_one = {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  std::deque<bool> ref_zero = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i = 0;
  bool is_negative = 0;
  if (x[0] != x2[0]) {
    is_negative = 1;
  };
  x[0] = 0;
  x2[0] = 0;
  bool is_zero_int = 1;
  for (i = 1; i < 64; i++) {
    if (ref_zero[i] != x[i]) {
      is_zero_int = 0;
      break;
    };
  };
  if (is_zero_int) {
    return ref_zero;
  };
  is_zero_int = 1;
  for (i = 1; i < 64; i++) {
    if (ref_zero[i] != x2[i]) {
      is_zero_int = 0;
      break;
    };
  };
  if (is_zero_int) {
    return ref_zero;
  };
  std::deque<bool> int_part_dq = DoubleToIntBinary(x2);
  std::deque<bool> int_mult_dq = {};
  std::deque<bool> dec_part_dq = {};
  std::deque<bool> decimal_add_dq = {};
  is_zero_int = 1;
  for (i = 0; i < 32; i++) {
    if (ref_zero[i] != int_part_dq[i]) {
      is_zero_int = 0;
      break;
    };
  };
  if (!is_zero_int) {
    int_mult_dq = DoubleMultiplyInt(x, int_part_dq);
  } else {
    int_mult_dq = ref_zero;
  };
  dec_part_dq = DecimalDouble(x2);
  dec_part_dq = DoubleDivide(ref_one, dec_part_dq);
  decimal_add_dq = DoubleDivide(x, dec_part_dq);
  int_mult_dq = IEEE754DoubleToDoubleAddition(int_mult_dq, decimal_add_dq);
  if (is_negative) {
    int_mult_dq[0] = 1;
  };
  return int_mult_dq;
};



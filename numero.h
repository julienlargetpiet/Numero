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
    std::cout << "Error: first arg is not a double binary representation";
    return 0;
  };
  if (x2.size() != 32) {
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
  while (i < 9)  {
    exponent1_dq.push_back(x1[i]);
    exponent2_dq.push_back(x2[i]);
    i += 1;
  };
  val1 = binarydq_to_int(exponent1_dq);
  val2 = binarydq_to_int(exponent2_dq);
  if (val1 < val2) {
    if (is_negative1) {
      return 1;
    } else {
      return 0;
    };
  } else if (val1 > val2) {
    if (is_negative1) {
      return 0;
    } else {
      return 1;
    };
  };
  while (i < 32)  {
    mantissa1_dq.push_back(x1[i]);
    mantissa2_dq.push_back(x2[i]);
    i += 1;
  };
  val1 = binarydq_to_int(mantissa1_dq);
  val2 = binarydq_to_int(mantissa2_dq);
  if (val1 < val2) {
    if (is_negative1) {
      return 1;
    } else {
      return 0;
    };
  } else if (val1 > val2) {
    if (is_negative1) {
      return 0;
    } else {
      return 1;
    };
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
  int exponent_vl = binarydq_to_int(exponent_dq);
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
  while (i < 12)  {
    exponent1_dq.push_back(x1[i]);
    exponent2_dq.push_back(x2[i]);
    i += 1;
  };
  val1 = binarydq_to_int(exponent1_dq);
  val2 = binarydq_to_int(exponent2_dq);
  if (val1 < val2) {
    if (is_negative1) {
      return 1;
    } else {
      return 0;
    };
  } else if (val1 > val2) {
    if (is_negative1) {
      return 0;
    } else {
      return 1;
    };
  };
  while (i < 64)  {
    mantissa1_dq.push_back(x1[i]);
    mantissa2_dq.push_back(x2[i]);
    i += 1;
  };
  val1 = binarydq_to_int(mantissa1_dq);
  val2 = binarydq_to_int(mantissa2_dq);
  if (val1 < val2) {
    if (is_negative1) {
      return 1;
    } else {
      return 0;
    };
  } else if (val1 > val2) {
    if (is_negative1) {
      return 0;
    } else {
      return 1;
    };
  };
  return 0; 
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
  std::deque<bool> exponent_dq = {};
  std::deque<bool> rtn_dq = {};
  int shift_over;
  while (i < 9) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  shift_over = binarydq_to_int(exponent_dq);
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
  rtn_dq.push_back(1);
  i = 0;
  while (i < shift_over) {
    rtn_dq.push_back(x[i + 9]);
    i += 1;
  };
  while (rtn_dq.size() != 31) {
    rtn_dq.push_front(0);
  };
  rtn_dq.push_front(x[0]);
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
  std::deque<bool> exponent_dq = {};
  std::deque<bool> rtn_dq = {};
  int shift_over;
  while (i < 12) {
    exponent_dq.push_back(x[i]);
    i += 1;
  };
  shift_over = binarydq_to_int(exponent_dq);
  shift_over -= 1023;
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
  rtn_dq.push_back(1);
  i = 0;
  while (i < shift_over) {
    rtn_dq.push_back(x[i + 12]);
    i += 1;
  };
  while (rtn_dq.size() != 31) {
    rtn_dq.push_front(0);
  };
  rtn_dq.push_front(x[0]);
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
  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> cur_dq = {};
  int exponent;
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
  std::deque<bool> rtn_dq = {x[0]};
  std::deque<bool> cur_dq = {};
  int exponent;
  while (i < 32) {
    if (x[i] == 1) {
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
  while (i < 32) {
    rtn_dq.push_back(x[i]);
    i += 1;
  };
  while (rtn_dq.size() != 64) {
    rtn_dq.push_back(0);
  };
  return rtn_dq;
};

//@T FloatToDouble
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
  exponent_vl = binarydq_to_int(exponent_dq);
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


<div class="Div2"><i><b>README</b></i></div><br><div class="container">
<div class="Divb">
<div class="box1"><a><i>Table Of Contents</i></a><br><br><ul>
<a href="#int_to_binarydq" style="margin-left:20px;">int_to_binarydq</a>
<br>
<a href="#binarydq_to_int" style="margin-left:20px;">binarydq_to_int</a>
<br>
<a href="#FloatStore" style="margin-left:20px;">FloatStore</a>
<br>
<a href="#IEEE754ToFloat" style="margin-left:20px;">IEEE754ToFloat</a>
<br>
<a href="#FloatToIEEE754" style="margin-left:20px;">FloatToIEEE754</a>
<br>
<a href="#ByteToBinaryFloat" style="margin-left:20px;">ByteToBinaryFloat</a>
<br>
<a href="#AreEqualFloat" style="margin-left:20px;">AreEqualFloat</a>
<br>
<a href="#IsSuperiorFloat" style="margin-left:20px;">IsSuperiorFloat</a>
<br>
<a href="#DoubleStore" style="margin-left:20px;">DoubleStore</a>
<br>
<a href="#IEEE754ToDouble" style="margin-left:20px;">IEEE754ToDouble</a>
<br>
<a href="#DoubleToIEEE754" style="margin-left:20px;">DoubleToIEEE754</a>
<br>
<a href="#ByteToBinaryDouble" style="margin-left:20px;">ByteToBinaryDouble</a>
<br>
<a href="#AreEqualDouble" style="margin-left:20px;">AreEqualDouble</a>
<br>
<a href="#IsSuperiorDouble" style="margin-left:20px;">IsSuperiorDouble</a>
<br>
<a href="#DoubleToIntBinary" style="margin-left:20px;">DoubleToIntBinary</a>
<br>
</ul><br>
</div>
</div>
<div class="box2">
<h2 id="int_to_binarydq" style="test-align: left;">int_to_binarydq</h2>
<h3>#Usage</h3>
<div class="Div"><code>std::deque&lt;bool&gt; int_to_binarydq(unsigned int x)</code></div>
<h3>#Description</h3>
<p>Converts an unsigned int to a binary format as a boolean deque</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input unsigned int</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>std::deque&lt;bool&gt; rtn_dq  = int_to_binarydq(1286);</code>
<br><code>1 0 1 0 0 0 0 0 1 1 0</code>
</div>
<br>
<hr class="hr">
<h2 id="binarydq_to_int" style="test-align: left;">binarydq_to_int</h2>
<h3>#Usage</h3>
<div class="Div"><code>unsigned int binarydq_to_int(std::deque&lt;bool&gt; &x)</code></div>
<h3>#Description</h3>
<p>Converts a binary format as a boolean deque to an unsigned int</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input boolean std deque</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>std::deque&lt;bool&gt; dq_input = {1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0};</code>
<br><code>unsigned int out = binarydq_to_int(rtn_dq);</code>
<br><code>1286</code>
</div>
<br>
<hr class="hr">
<h2 id="FloatStore" style="test-align: left;">FloatStore</h2>
<h3>#Usage</h3>
<div class="Div"><code>union FloatStore {<br>  float value;<br>  unsigned char byte_rep[sizeof(float)];<br>};</code></div>
<h3>#Description</h3>
<p>A simple union for a float representation and its byte representation according to the IEEE754 standard</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
no args </th><th> no def</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>float x = 43.13;</code>
<br><code>FloatStore obj1;</code>
<br><code>obj1.value = x;</code>
</div>
<br>
<hr class="hr">
<h2 id="IEEE754ToFloat" style="test-align: left;">IEEE754ToFloat</h2>
<h3>#Usage</h3>
<div class="Div"><code>float IEEE754ToFloat(std::deque&lt;bool&gt; &binary_rep)</code></div>
<h3>#Description</h3>
<p>Converts a IEE754 standardized binary representation of a single value precision decimal to a float.</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input binary representation</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>int i;</code>
<br><code>//Store float to memory and get its byte representation</code>
<br><code>float x = 43.13;</code>
<br><code>FloatStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(float)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(float));</code>
<br><code>////</code>
<br><code>//Convert to binary format, default to IEEE754</code>
<br><code>std::deque&lt;bool&gt; binary_rep = ByteToBinaryFloat(byte_rep);</code>
<br><code>i = 0;</code>
<br><code>while (i &lt; binary_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; binary_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>////</code>
<br><code>//Convert binary representation (IEEE754) to double</code>
<br><code>float rslt = IEEE754ToFloat(binary_rep);</code>
<br><code>std::cout &lt;&lt; std::setprecision(9);</code>
<br><code>std::cout &lt;&lt; rslt &lt;&lt; "\n";</code>
</div>
<br>
<hr class="hr">
<h2 id="FloatToIEEE754" style="test-align: left;">FloatToIEEE754</h2>
<h3>#Usage</h3>
<div class="Div"><code>std::deque&lt;bool&gt; FloatToIEEE754(float &x)</code></div>
<h3>#Description</h3>
<p>Converts a float to a boolean <code>std::deque</code> representing its binary representation according to the IEEE754 standard for single value precison.</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input float</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>float rslt = 833.726;</code>
<br><code>std::deque&lt;bool&gt; binary_rep = FloatToIEEE754(rslt);</code>
<br><code>int i = 0;</code>
<br><code>while (i &lt; binary_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; binary_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>01000100010100000110111001110110</code>
</div>
<br>
<hr class="hr">
<h2 id="ByteToBinaryFloat" style="test-align: left;">ByteToBinaryFloat</h2>
<h3>#Usage</h3>
<div class="Div"><code>std:deque&lt;bool&gt; ByteToBinaryFloat(unsigned char (&byte_rep)[sizeof(float)])</code></div>
<h3>#Description</h3>
<p>Converts a byte representation of a float to its binary representation</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input byte array</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>float x = 43.13;</code>
<br><code>FloatStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(float)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(float));</code>
<br><code>std::deque&lt;bool&gt; binary_rep = ByteToBinaryFloat(byte_rep);</code>
<br><code>i = 0;</code>
<br><code>while (i &lt; binary_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; binary_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>01000010001011001000010100011111</code>
</div>
<br>
<hr class="hr">
<h2 id="AreEqualFloat" style="test-align: left;">AreEqualFloat</h2>
<h3>#Usage</h3>
<div class="Div"><code>bool AreEqualFloat(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)</code></div>
<h3>#Description</h3>
<p>Returns a boolean, if the binary (IEEE754) representation of 2 differents float are equal it will return 1, 0 either.</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the first IEEE754 binary representation of a float, the first double</th></tr>
<tr><th>x </th><th> is the second IEEE754 binary representation of a float, the second double</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>//Store double to memory and get its byte representation</code>
<br><code>float x = 43232.1813;</code>
<br><code>FloatStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(float)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(float));</code>
<br><code>std::deque&lt;bool&gt; bit_rep = ByteToBinaryFloat(byte_rep);</code>
<br><code>float x2 = 123232.112;</code>
<br><code>FloatStore obj2;</code>
<br><code>obj2.value = x2;</code>
<br><code>unsigned char byte_rep2[sizeof(float)];</code>
<br><code>memcpy(byte_rep2, obj2.byte_rep, sizeof(float));</code>
<br><code>std::deque&lt;bool&gt; bit_rep2 = ByteToBinaryFloat(byte_rep2);</code>
<br><code>int i = 0;</code>
<br><code>while (i &lt; bit_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; bit_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>i = 0;</code>
<br><code>while (i &lt; bit_rep2.size()) {</code>
<br><code>  std::cout &lt;&lt; bit_rep2[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>////</code>
<br><code>bool test = AreEqualFloat(bit_rep, bit_rep2);</code>
<br><code>std::cout &lt;&lt; test &lt;&lt; "\n";</code>
</div>
<br>
<hr class="hr">
<h2 id="IsSuperiorFloat" style="test-align: left;">IsSuperiorFloat</h2>
<h3>#Usage</h3>
<div class="Div"><code>bool IsSuperiorFloat(std::deque&lt;bool&gt; &x1, std::deque&lt;bool&gt; &x2)</code></div>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the first IEEE754 binary representation of a float, the first float</th></tr>
<tr><th>x </th><th> is the second IEEE754 binary representation of a float, the second float</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>//Store double to memory and get its byte representation</code>
<br><code>float x = 43232.1813;</code>
<br><code>FloatStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(float)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(float));</code>
<br><code>std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);</code>
<br><code>float x2 = 23232.18;</code>
<br><code>FoatStore obj2;</code>
<br><code>obj2.value = x2;</code>
<br><code>unsigned char byte_rep2[sizeof(float)];</code>
<br><code>memcpy(byte_rep2, obj2.byte_rep, sizeof(float));</code>
<br><code>std::deque&lt;bool&gt; bit_rep2 = ByteToBinaryDouble(byte_rep2);</code>
<br><code>int i = 0;</code>
<br><code>while (i &lt; bit_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; bit_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>i = 0;</code>
<br><code>while (i &lt; bit_rep2.size()) {</code>
<br><code>  std::cout &lt;&lt; bit_rep2[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>////</code>
<br><code>bool test = IsSuperiorFloat(bit_rep, bit_rep2);</code>
<br><code>std::cout &lt;&lt; test &lt;&lt; "\n";</code>
<br><code>1</code>
</div>
<br>
<hr class="hr">
<h2 id="DoubleStore" style="test-align: left;">DoubleStore</h2>
<h3>#Usage</h3>
<div class="Div"><code>union DoubleStore {<br>  double value;<br>  unsigned char byte_rep[sizeof(double)];<br>};</code></div>
<h3>#Description</h3>
<p>A simple union for a double representation and its byte representation according to the IEEE754 standard</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
no args </th><th> no def</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>double x = 43.13;</code>
<br><code>DoubleStore obj1;</code>
<br><code>obj1.value = x;</code>
</div>
<br>
<hr class="hr">
<h2 id="IEEE754ToDouble" style="test-align: left;">IEEE754ToDouble</h2>
<h3>#Usage</h3>
<div class="Div"><code>float IEEE754ToDouble(std::deque&lt;bool&gt; &binary_rep)</code></div>
<h3>#Description</h3>
<p>Converts a IEE754 standardized binary representation of a double value precision decimal to a double.</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input binary representation</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>int i;</code>
<br><code>//Store float to memory and get its byte representation</code>
<br><code>double x = 43.13;</code>
<br><code>DoubleStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(double)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(double));</code>
<br><code>////</code>
<br><code>//Convert to binary format, default to IEEE754</code>
<br><code>std::deque&lt;bool&gt; binary_rep = ByteToBinaryDouble(byte_rep);</code>
<br><code>i = 0;</code>
<br><code>while (i &lt; binary_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; binary_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>////</code>
<br><code>//Convert binary representation (IEEE754) to double</code>
<br><code>float rslt = IEEE754ToDouble(binary_rep);</code>
<br><code>std::cout &lt;&lt; std::setprecision(9);</code>
<br><code>std::cout &lt;&lt; rslt &lt;&lt; "\n";</code>
</div>
<br>
<hr class="hr">
<h2 id="DoubleToIEEE754" style="test-align: left;">DoubleToIEEE754</h2>
<h3>#Usage</h3>
<div class="Div"><code>std::deque&lt;bool&gt; DoubleToIEEE754(float &x)</code></div>
<h3>#Description</h3>
<p>Converts a double to a boolean <code>std::deque</code> representing its binary representation according to the IEEE754 standard for double value precison.</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input double</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>double rslt = 833.726;</code>
<br><code>std::deque&lt;bool&gt; binary_rep = DoubleToIEEE754(rslt);</code>
<br><code>int i = 0;</code>
<br><code>while (i &lt; binary_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; binary_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>0100000010001010000011011100111011011001000101101000011100101010</code>
</div>
<br>
<hr class="hr">
<h2 id="ByteToBinaryDouble" style="test-align: left;">ByteToBinaryDouble</h2>
<h3>#Usage</h3>
<div class="Div"><code>std:deque&lt;bool&gt; ByteToBinaryDouble(unsigned char (&byte_rep)[sizeof(double)])</code></div>
<h3>#Description</h3>
<p>Converts a byte representation of a double to its binary representation</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input byte array</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>double x = 43.13;</code>
<br><code>DoubleStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(double)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(double));</code>
<br><code>std::deque&lt;bool&gt; binary_rep = ByteToBinaryDouble(byte_rep);</code>
<br><code>i = 0;</code>
<br><code>while (i &lt; binary_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; binary_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>0100000001111011000000101110010101100000010000011000100100110111</code>
</div>
<br>
<hr class="hr">
<h2 id="AreEqualDouble" style="test-align: left;">AreEqualDouble</h2>
<h3>#Usage</h3>
<div class="Div"><code>bool AreEqualDouble(std::deque&lt;bool&gt; &x, std::deque&lt;bool&gt; &x2)</code></div>
<h3>#Description</h3>
<p>Returns a boolean, if the binary (IEEE754) representation of 2 differents double are equal it will return 1, 0 either.</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the first IEEE754 binary representation of a double, the first double</th></tr>
<tr><th>x </th><th> is the second IEEE754 binary representation of a double, the second double</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>//Store double to memory and get its byte representation</code>
<br><code>double x = 43232.181342;</code>
<br><code>DoubleStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(double)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(double));</code>
<br><code>std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);</code>
<br><code>double x2 = 23232.181342;</code>
<br><code>DoubleStore obj2;</code>
<br><code>obj2.value = x2;</code>
<br><code>unsigned char byte_rep2[sizeof(double)];</code>
<br><code>memcpy(byte_rep2, obj2.byte_rep, sizeof(double));</code>
<br><code>std::deque&lt;bool&gt; bit_rep2 = ByteToBinaryDouble(byte_rep2);</code>
<br><code>int i = 0;</code>
<br><code>while (i &lt; bit_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; bit_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>i = 0;</code>
<br><code>while (i &lt; bit_rep2.size()) {</code>
<br><code>  std::cout &lt;&lt; bit_rep2[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>////</code>
<br><code>bool test = AreEqualDouble(bit_rep, bit_rep2);</code>
<br><code>std::cout &lt;&lt; test &lt;&lt; "\n";</code>
<br><code>0</code>
</div>
<br>
<hr class="hr">
<h2 id="IsSuperiorDouble" style="test-align: left;">IsSuperiorDouble</h2>
<h3>#Usage</h3>
<div class="Div"><code>bool IsSuperiorDouble(std::deque&lt;bool&gt; &x1, std::deque&lt;bool&gt; &x2)</code></div>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the first IEEE754 binary representation of a double, the first double</th></tr>
<tr><th>x </th><th> is the second IEEE754 binary representation of a double, the second double</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>//Store double to memory and get its byte representation</code>
<br><code>double x = 43232.181342;</code>
<br><code>DoubleStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(double)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(double));</code>
<br><code>std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);</code>
<br><code>double x2 = 23232.181342;</code>
<br><code>DoubleStore obj2;</code>
<br><code>obj2.value = x2;</code>
<br><code>unsigned char byte_rep2[sizeof(double)];</code>
<br><code>memcpy(byte_rep2, obj2.byte_rep, sizeof(double));</code>
<br><code>std::deque&lt;bool&gt; bit_rep2 = ByteToBinaryDouble(byte_rep2);</code>
<br><code>int i = 0;</code>
<br><code>while (i &lt; bit_rep.size()) {</code>
<br><code>  std::cout &lt;&lt; bit_rep[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>i = 0;</code>
<br><code>while (i &lt; bit_rep2.size()) {</code>
<br><code>  std::cout &lt;&lt; bit_rep2[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
<br><code>////</code>
<br><code>bool test = IsSuperiorDouble(bit_rep, bit_rep2);</code>
<br><code>std::cout &lt;&lt; test &lt;&lt; "\n";</code>
<br><code>1</code>
</div>
<br>
<hr class="hr">
<h2 id="DoubleToIntBinary" style="test-align: left;">DoubleToIntBinary</h2>
<h3>#Usage</h3>
<div class="Div"><code>std::deque&lt;bool&gt; DoubleToIntBinary(std::deque&lt;bool&gt; &x)</code></div>
<h3>#Description</h3>
<p>Converts the IEEE754 standard binary representation of a double to an int (32 bits)</p>
<h3>#Arguments</h3>
<table><tr><th>Name</th><th>Definition</th></tr><tr><th>
x </th><th> is the input binary representation as a boolean deque</th></tr>
</table>
<br>
<h3>#Example(s)</h3>
<div class = "Div"><code>//Store double to memory and get its byte representation</code>
<br><code>double x = 43232.1813;</code>
<br><code>DoubleStore obj1;</code>
<br><code>obj1.value = x;</code>
<br><code>unsigned char byte_rep[sizeof(double)];</code>
<br><code>memcpy(byte_rep, obj1.byte_rep, sizeof(double));</code>
<br><code>std::deque&lt;bool&gt; bit_rep = ByteToBinaryDouble(byte_rep);</code>
<br><code>std::deque&lt;bool&gt; int_dq = DoubleToIntBinary(bit_rep);</code>
<br><code>int i = 0;</code>
<br><code>while (i &lt; int_dq.size()) {</code>
<br><code>  std::cout &lt;&lt; int_dq[i];</code>
<br><code>  i += 1;</code>
<br><code>};</code>
<br><code>std::cout &lt;&lt; "\n";</code>
</div>
<br>
<hr class="hr">
</div>
</div>

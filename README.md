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
<a href="#ByteToBinary" style="margin-left:20px;">ByteToBinary</a>
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
<br><code>  std::cout &lt&lt; binary_rep[i];</code>
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
<h2 id="ByteToBinary" style="test-align: left;">ByteToBinary</h2>
<h3>#Usage</h3>
<div class="Div"><code>std:deque&lt;bool&gt; ByteToBinary(unsigned char (&byte_rep)[sizeof(float)])</code></div>
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
</div>
</div>

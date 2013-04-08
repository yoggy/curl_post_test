curl_post_test
============
libcurlを使ってmultipart/form-dataのPOSTをするサンプル。

how to use
============
for Ubuntu 12.04
<pre>
  $ sudo apt-get install build-essential
  $ sudo apt-get install libcurl4-openssl-dev
  $ sudo apt-get install cmake
  $ git clone github.com/yoggy/curl_post_test.git
  $ cd curl_post_test
  $ cmake .
  $ make
  $ ./curl_post_test nike.jpg http://example.com/path/to/form
</pre>



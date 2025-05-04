## VERIFYING INTEGRITY

> The integrity of data can be verified using hashing algorithms such as MD5, SHA1, SHA2, SHA3 etc. This section of my notes explains how to use OpenSSL to verify data integrity.

<br>

### Verifying integrity of OpenSSL tarball.

#### - Download OpenSSL source tarball and SHA256 checksum.
<pre>
hashi@ub-ch1:~$ wget https://github.com/openssl/openssl/releases/download/openssl-3.5.0/openssl-3.5.0.tar.gz
hashi@ub-ch1:~$ wget https://github.com/openssl/openssl/releases/download/openssl-3.5.0/openssl-3.5.0.tar.gz.sha256
hashi@ub-ch1:~$ ls openssl*
openssl-3.5.0.tar.gz  openssl-3.5.0.tar.gz.sha256
</pre>

#### - Verify hash as shown below.
<pre>
hashi@ub-ch1:~$ cat openssl-3.5.0.tar.gz.sha256 
344d0a79f1a9b08029b0744e2cc401a43f9c90acd1044d09a530b4885a8e9fc0 *openssl-3.5.0.tar.gz

hashi@ub-ch1:~$ openssl sha256 openssl-3.5.0.tar.gz
SHA2-256(openssl-3.5.0.tar.gz)= 344d0a79f1a9b08029b0744e2cc401a43f9c90acd1044d09a530b4885a8e9fc0
</pre>

#### - Verifying hash using sha256sum.
<pre>
hashi@ub-ch1:~$ sha256sum openssl-3.5.0.tar.gz
344d0a79f1a9b08029b0744e2cc401a43f9c90acd1044d09a530b4885a8e9fc0  openssl-3.5.0.tar.gz
</pre>

<br>
### Listing supported Hashing Algorithms.

#### - To list all supported hashing algorithm.
<pre>
hashi@ub-ch1:~$ openssl list -digest-algorithms
Legacy:
  RSA-MD4 => MD4
  RSA-MD5 => MD5
  RSA-RIPEMD160 => RIPEMD160
  RSA-SHA1 => SHA1
  RSA-SHA1-2 => RSA-SHA1
  RSA-SHA224 => SHA224
  RSA-SHA256 => SHA256
  RSA-SHA3-224 => SHA3-224
  RSA-SHA3-256 => SHA3-256
  RSA-SHA3-384 => SHA3-384
  RSA-SHA3-512 => SHA3-512
  RSA-SHA384 => SHA384
  RSA-SHA512 => SHA512
  RSA-SHA512/224 => SHA512-224
  RSA-SHA512/256 => SHA512-256
  RSA-SM3 => SM3
  BLAKE2b512
</pre>

#### - Another way to list all supported hashing algorithms.
<pre>
hashi@ub-ch1:~$ openssl dgst -list
Supported digests:
-blake2b512                -blake2s256                -md4                      
-md5                       -md5-sha1                  -mdc2                     
-ripemd                    -ripemd160                 -rmd160                   
-sha1                      -sha224                    -sha256                   
-sha3-224                  -sha3-256                  -sha3-384                 
-sha3-512                  -sha384                    -sha512                   
-sha512-224                -sha512-256                -shake128                 
-shake256                  -sm3                       -ssl3-md5                 
-ssl3-sha1                 -whirlpool
</pre>

<br>

### Generating checksum.

**SYNTAX :** openssl digest_algorithm -hex -out output_file file_to_hash

#### - Here's the file I'll be using to generate a checksum.
<pre>
hashi@ub-ch1:~$ echo -n "Earth is the third planet of our Solar System." > earth.txt
</pre>

#### - Generating SHA1 checksum.
<pre>
hashi@ub-ch1:~$ openssl sha1 -hex -out earth.txt.sha1 earth.txt 
hashi@ub-ch1:~$ cat earth.txt.sha1 
SHA1(earth.txt)= 1746134c2e64000f50fa62aa4c515e4640b2a804
</pre>

#### - Generating SHA256 checksum.
<pre>
hashi@ub-ch1:~$ openssl sha256 -hex -out earth.txt.sha256 earth.txt 
hashi@ub-ch1:~$ cat earth.txt.sha256 
SHA2-256(earth.txt)= 8989795a7c217d87e73ad6371090b91a91840a68dedf2a97e91f4b4e884de122
</pre>

#### - Here's another way to generate hash using OpenSSL.
<pre>
hashi@ub-ch1:~$ openssl dgst -sha1 -hex -out earth.txt.sha1 earth.txt
hashi@ub-ch1:~$ openssl dgst -sha256 -hex -out earth.txt.sha256 earth.txt
</pre>

#### - Most Linux distributions includes sha1sum and sha256sum utilities to perform the same task.
<pre>
hashi@ub-ch1:~$ sha1sum earth.txt > earth.txt.sha1 
hashi@ub-ch1:~$ sha256sum earth.txt > earth.txt.sha256
</pre>

<br>

[Back to main page](README.md)
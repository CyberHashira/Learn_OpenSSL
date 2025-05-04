## ENCRYPTION USING OPENSSL

> This section covers how to encrypt data using OpenSSL. OpenSSL supports different symmetric and asymmetric algorithms that can be utilised for the purpose of encryption.

<br>

### Generating secret (Symmetric) keys .
> Symmetric keys, also known as secret keys, are plain byte sequences. Unlike asymmetric keys, they do not require ASN.1 encoding. Therefore, the openssl rand command can be used to generate secret keys.

**SYNTAX :** openssl rand -hex -out out_file data_size

#### - Generating 16 bytes of random data equivalent to an AES-128 key.
<pre>
hashi@ub-ch1:~$ openssl rand -out aes-128.bin 16
hashi@ub-ch1:~$ ls -l aes-128.bin 
-rw-rw-r-- 1 hashi hashi 16 May  1 17:50 aes-128.bin
</pre>

#### - Generating 32 bytes of hex formatted random data equivalent to an AES-256 key.
<pre>
hashi@ub-ch1:~$ openssl rand -hex -out aes256.key 32
</pre>

#### - Generating 32 bytes of random base64 encoded data.
<pre>
hashi@ub-ch1:~$ openssl rand -base64 -out aes256.b64 32
hashi@ub-ch1:~$ cat aes256.b64 
njy110fz9Yvskend26Ju5lc0uGOP2zeWAfVFeWO+yek=
</pre>

<br>

### Generating key pair (Asymmetric Keys).
> Asymmetric keys or key pairs are ASN.1 encoded. OpenSSL includes built-in commands to generate them based on the algorithm type.
> I wil cover key pair generation in more detail in the **Generating Asymmetric Keys** section.
> I'm also using the traditional method for generating a key pair.

#### - Generating a RSA private key

>[!NOTE]
> Newer versions of OpenSSL defaults to RSA-2048 when keysize if not specified.

<pre>
hashi@ub-ch1:~$ openssl genrsa -out rsa.pri
</pre>

#### - Generating a RSA private key with a specified keysize
<pre>
hashi@ub-ch1:~$ openssl genrsa -out rsa-1024.key 1024
hashi@ub-ch1:~$ openssl genrsa -out rsa-2048.key 2048
</pre>

#### - Examining a private key
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa-1024.key -noout -text
Private-Key: (1024 bit, 2 primes)
modulus:
    00:bf:12:9d:a1:be:4e:e0:ff:4b:28:36:c6:bd:79:
publicExponent: 65537 (0x10001)
privateExponent:
    01:e8:1f:a6:93:8d:99:f5:32:3c:ce:7f:21:38:eb:
prime1:
    00:f1:be:11:75:79:8b:88:8e:27:d5:9b:f3:6e:4c:
prime2:
    00:ca:57:83:97:47:69:34:75:d5:d6:3b:4d:b6:b3:
exponent1:
    24:2e:8f:8c:66:a9:c4:c1:8a:22:c5:99:b4:cb:9a:
exponent2:
    00:c7:9e:64:cd:06:ff:6c:68:df:e8:75:23:2b:1e:
coefficient:
    00:9b:23:f3:ab:6f:00:d3:74:94:32:b8:fd:bc:fe:
</pre>

#### - Extracting public key from a private key.
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa-2048.key -pubout -out rsa-2048.pub
writing RSA key

hashi@ub-ch1:~$ cat rsa-2048.pub 
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqyrN47/oqU+yq7CG7Aet
8Y6c5ieGtDitm6gYINHbcB0AUQtkJlQ7FO8w36OKmAWhryN2BtTMAyy56sXs2Vuz
EhtrKxj/ag1O50pxU4eXTHB5fv186TH+dls807OP/xLEt4Dm2QGEiheb6IKUPwub
HQIDAQAB
-----END PUBLIC KEY-----
</pre>

<br>

### ENCRYPTING USING A SECRET KEY.

**SYNTAX :** openssl encryption-algorithm -in file-to-encrypt -out output-file -iter number-of-iterations -pbkdf2 -e/-d -kfile key-file
| OPTIONS | DESCRIPTION |
| --- | --- |
| encryption-algorithm | encryption algorithm to use. |
| -in file-to-encrypt | input file to encrypt. |
| -out out-file | encrypted output file name. |
| -iter iteration | number of iterations. |
| -pbkdf2 | use Password based Key Derivation Function (KDF). |
| -e/-d | -e for encrypt, -d for decrypt. |
| -a | use base64 encoding. |
| - kfile | key file to use for encryption. |


#### - This is the file I will use to demonstrate encryption using OpenSSL.
<pre>
hashi@ub-ch1:~$ echo -n "Earth is the third planet of our Solar System." > earth.txt
</pre>

#### - Here's how I generated the AES key for encrypting data.
<pre>
hashi@ub-ch1:~$ openssl rand -out encryption.key 32
</pre>

#### - Encrypting this file using AES-256-CBC method of encryption.
> [NOTE!]
> A warning is displayed because the -iter or -pbkdf2 option is not being used, as recommended by OpenSSL.

<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt -out earth.txt.enc -e -kfile encryption.key 
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.
</pre>

#### - Here's how to decrypt an encrypted file.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt.enc -out earth.txt.dec -d -kfile encryption.key 
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.

hashi@ub-ch1:~$ cat earth.txt.dec 
Earth is the third planet of our Solar System.
</pre>

#### - Using the wrong encryption key to decrypt data should result in an error.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt.enc -out earth.txt.dec -d -kfile another.key 
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.
bad decrypt
80BB7784E8730000:error:1C800064:Provider routines:ossl_cipher_unpadblock:bad decrypt:providers/implementations/ciphers/ciphercommon_block.c:107:
</pre>

#### - Encrypt the file and output as Base64 encoded encrypted data.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt -out earth.txt.enc -e -a -kfile encryption.key 
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.
hashi@ub-ch1:~$ cat earth.txt.enc 
U2FsdGVkX19vxttVTGrzE7kp+FxbDfw7EkFHsQyW3v1rwnQKwV8Tx2d3tEF3JHxX
qXJyIZMgoGFe+MvUqxQRSw==
</pre>

#### - Decrypting a Base64 encoded encrypted data.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt.enc -out earth.txt.dec -d -a -kfile encryption.key 
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.
hashi@ub-ch1:~$ cat earth.txt.dec 
Earth is the third planet of our Solar System.
</pre>


#### - Using another algorithm to perform encryption.
> [!NOTE]
> If a key file is not provided, OpenSSL will prompt for a password, which will be used to derive the encryption key.

<pre>
hashi@ub-ch1:~$ openssl aria-256-cbc -in earth.txt -out earth.txt.enc -e -a 
enter ARIA-256-CBC encryption password:
Verifying - enter ARIA-256-CBC encryption password:
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.

hashi@ub-ch1:~$ openssl aria-256-cbc -in earth.txt.enc -d -a 
enter ARIA-256-CBC decryption password:
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.
Earth is the third planet of our Solar System.
</pre>

#### - Using a different password from the one used for encryption will result in an error.
<pre>
ashi@ub-ch1:~$ openssl aria-256-cbc -in earth.txt.enc -d -a 
enter ARIA-256-CBC decryption password:
*** WARNING : deprecated key derivation used.
Using -iter or -pbkdf2 would be better.
bad decrypt
80FB7105F4750000:error:1C800064:Provider routines:ossl_cipher_unpadblock:bad decrypt:providers/implementations/ciphers/ciphercommon_block.c:107:
</pre>


#### - Using a password-based key derivation function to prevent warning messages from being displayed.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt -out earth.txt.enc -e -a -pbkdf2 -kfile encryption.key 
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt.enc -out earth.txt.dec -d -a -pbkdf2 -kfile encryption.key 
hashi@ub-ch1:~$ cat earth.txt.dec 
Earth is the third planet of our Solar System.
</pre>

#### - Using PBKDF2 with a specified number of iterations.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt -out earth.txt.enc -e -a -pbkdf2 -iter 10000 -kfile encryption.key 
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt.enc -d -a -pbkdf2 -iter 10000 -kfile encryption.key 
Earth is the third planet of our Solar System.
</pre>

#### - Using a different number of iterations from the one used during encryption will result in an error.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt.enc -d -a -pbkdf2 -iter 10010 -kfile encryption.key 
bad decrypt
806B2ADDAF720000:error:1C800064:Provider routines:ossl_cipher_unpadblock:bad decrypt:providers/implementations/ciphers/ciphercommon_block.c:112:
</pre>

#### - Print verbose output during encryption.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt -out earth.txt.enc -e -a -pbkdf2 -iter 10000 -kfile encryption.key -v
bufsize=8192
bytes read   :       46
bytes written:       90
</pre>

#### - Printing debug information during encryption.
<pre>
hashi@ub-ch1:~$ openssl aes-256-cbc -in earth.txt -out earth.txt.enc -e -a -pbkdf2 -iter 10000 -kfile encryption.key -debug
BIO[0x651be4159f00]: ctrl(6) - base64 encoding
BIO[0x651be4159e70]: ctrl(6) - FILE pointer
BIO[0x651be4159e70]: ctrl return 0
BIO[0x651be4159f00]: ctrl return 0
</pre>

<br>

### ENCRYPTING USING ASYMMETRIC KEYS (OLD METHOD).

#### - Encrypting using RSA public key.
> [!IMPORTANT]
> This method is deprecated in OpenSSL 3.x and is only available for legacy use cases.

<pre>
hashi@ub-ch1:~$ openssl rsautl --encrypt -inkey rsa-2048.pub -pubin -in earth.txt -out earth.txt.enc
The command rsautl was deprecated in version 3.0. Use 'pkeyutl' instead.
</pre>

#### - Decrypting using RSA private key.
<pre>
hashi@ub-ch1:~$ openssl rsautl --decrypt -inkey rsa-2048.key -in earth.txt.enc -out earth.txt.dec
The command rsautl was deprecated in version 3.0. Use 'pkeyutl' instead.
hashi@ub-ch1:~$ cat earth.txt.dec 
Earth is the third planet of our Solar System.
</pre>

<BR>

### ENCRYPTING USING ASYMMETRIC KEYS (NEW METHOD).

**SYNTAX :** openssl pkeyutl -encrypt -inkey key -in file-to-encrypt -out out-file.

| OPTIONS | DESCRIPTION |
| --- | --- |
| -in file-to-encrypt | input file to encrypt. |
| -out out-file | encrypted output file name. |
| -inkey key | key file to use. |
| -pubin | must be used if key file is a public key. |
| -encrypt | to encrypt. |
| -decrypt | to decrypt. |
| -pkeyopt | asymmetric key options. |

#### - Encrypting using RSA public key (PKCS1)
<pre>
hashi@ub-ch1:~$ openssl pkeyutl -encrypt -inkey rsa-2048.pub -pubin -in earth.txt -out earth.txt.enc
</pre>

#### - Decrypting using RSA private key (PKCS1)
<pre>
hashi@ub-ch1:~$ openssl pkeyutl -decrypt -inkey rsa-2048.key -in earth.txt.enc -out earth.txt.dec
hashi@ub-ch1:~$ cat earth.txt.dec 
Earth is the third planet of our Solar System.
</pre>

#### - Data size limit of RSA Encryption.
> [!NOTE]
> RSA encryption in OpenSSL uses PKCS#1 padding by default. PKCS#1 has a size limit of (key size − 11 bytes).
> https://www.rfc-editor.org/rfc/rfc2313.
> "The length of the data D shall not be more than k-11 octets"

#### - Encryption should work as expected, provided the size of the data is within the permitted limit.
<pre>
hashi@ub-ch1:~$ openssl rand -out data 245
hashi@ub-ch1:~$ openssl pkeyutl -encrypt -inkey rsa-2048.key -in data -out data.enc
</pre>

#### - Encryption should fail if the size of data exceeds the permitted limit.
<pre>
hashi@ub-ch1:~$ openssl rand -out data 246
hashi@ub-ch1:~$ openssl pkeyutl -encrypt -inkey rsa-2048.key -in data -out data.enc
Public Key operation error
804B3D20647F0000:error:0200006E:rsa routines:ossl_rsa_padding_add_PKCS1_type_2_ex:data too large for key size:crypto/rsa/rsa_pk1.c:133:
</pre>

#### - Encrypting using RSA public key (OAEP).
<pre>
hashi@ub-ch1:~$ openssl pkeyutl -encrypt -inkey rsa-2048.pub -pubin -pkeyopt rsa_padding_mode:oaep -in earth.txt -out earth.txt.enc
</pre>

#### - Decrypting using RSA private key (OAEP).
<pre>
hashi@ub-ch1:~$ openssl pkeyutl -decrypt -inkey rsa-2048.key -pkeyopt rsa_padding_mode:oaep -in earth.txt.enc -out earth.txt.dec
hashi@ub-ch1:~$ sha1sum earth.txt earth.txt.dec 
3cba1292047b33e15d180b1ed07e78438c144ae0  earth.txt
3cba1292047b33e15d180b1ed07e78438c144ae0  earth.txt.dec
</pre>

#### - Encrypting / Decrypting using a user specified hash algorithm for masking function.
<pre>
hashi@ub-ch1:~$ openssl pkeyutl -encrypt -inkey rsa-2048.pub -pubin -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_mgf1_md:sha256 -pkeyopt rsa_oaep_md:sha256 -in earth.txt -out earth.txt.enc

hashi@ub-ch1:~$ openssl pkeyutl -decrypt -inkey rsa-2048.key -pkeyopt rsa_padding_mode:oaep -pkeyopt rsa_mgf1_md:sha256 -pkeyopt rsa_oaep_md:sha256 -in earth.txt.enc -out earth.txt.dec 

hashi@ub-ch1:~$ sha1sum earth.txt earth.txt.dec 
3cba1292047b33e15d180b1ed07e78438c144ae0  earth.txt
3cba1292047b33e15d180b1ed07e78438c144ae0  earth.txt.dec
</pre>

<br>

### KEY TRANSPORTATION USING RSA.

> Aside from encryption and digital signing, RSA can also be used for key transport in hybrid encryption schemes, where it securely delivers symmetric keys over insecure channels.

#### - Assume this is the encryption key that requires secure transmission.
<pre>
hashi@ub-ch1:~$ openssl rand -out encryption.key 32
</pre>

#### - Here's how you'd encrypt the secret key.
<pre>
openssl pkeyutl -encrypt -inkey rsa-2048.pub -pubin -in encryption.key -out encryption.key.enc
</pre>

#### - Here's how you'd decrypt the encrypted key.
<pre>
hashi@ub-ch1:~$ openssl pkeyutl -decrypt -inkey rsa-2048.key -in encryption.key.enc -out encryption.key.dec
</pre>

#### - Checksum to prove that both encryption keys are the same.
<pre>
hashi@ub-ch1:~$ sha1sum encryption.key.dec 
4adfa8d566a0071f8647bae8dedf68ec1fd356ea  encryption.key.dec

hashi@ub-ch1:~$ sha1sum encryption.key
4adfa8d566a0071f8647bae8dedf68ec1fd356ea  encryption.key
</pre>

<br>

[Back to main page](README.md)
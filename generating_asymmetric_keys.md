## GENERATING ASYMMETRIC KEYS (KEY PAIRS)

In this section of my notes, I will demonstrate how you can generate key pairs, a.k.a asymmetric keys.

<br>

### Generating RSA keypair

*This section covers aspects of generating an RSA key pair.*

#### - Generating RSA private key.
> [!NOTE]
> **openssl genrsa** defaults to 2048

<pre>
hashi@ub-ch1:~$ openssl genrsa
</pre>

#### - Generate RSA keys of different key sizes.
<pre>
hashi@ub-ch1:~$ openssl genrsa 1024
hashi@ub-ch1:~$ openssl genrsa 2048
</pre>

#### - Storing generated rsa private key to a file.
<pre>
hashi@ub-ch1:~$ openssl genrsa -out rsa.pri
</pre>

*You could also pipe the outoput to a file*
<pre>
hashi@ub-ch1:~$ openssl genrsa > rsa.pri
</pre>

#### - Extracting public key from the private key.
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.pri -pubout
writing RSA key
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxIgDzUVI0A9HmFdXaKwQ
lOi95peeQof84KkURDb0CBq8jGnvFHLAgdEO/n7rmHuNq2YWtn3rDY8OsAAagHQg
xi90GYKvmaqqlPbL/Z8F7AzPsf4+VTC5Yem7lFvKB86u6l4iZwfGHxhbEMMYvajI
wod5WNjftEmAFUEowRFRne0FOwiD9/Z948dlSNobJwDMMR2vfSm9ZdSM3bAOO8Xm
WEAO4+xIYKnT67cyn9Sc4UWYdbV31o/lfrx9LsNy4z1KyYDzZr3wt/Q/Czog4jmk
dDDIuaabmYYWKxaYFQ2t85k1NOApJuVOH+N5AyM8qhbwP4XwvymVqLUmcJZPR14O
MQIDAQAB
-----END PUBLIC KEY-----
</pre>

#### - Output public key to a file.
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.pri -pubout -out rsa.pub 
writing RSA key
hashi@ub-ch1:~$ cat rsa.pub 
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAxIgDzUVI0A9HmFdXaKwQ
lOi95peeQof84KkURDb0CBq8jGnvFHLAgdEO/n7rmHuNq2YWtn3rDY8OsAAagHQg
xi90GYKvmaqqlPbL/Z8F7AzPsf4+VTC5Yem7lFvKB86u6l4iZwfGHxhbEMMYvajI
wod5WNjftEmAFUEowRFRne0FOwiD9/Z948dlSNobJwDMMR2vfSm9ZdSM3bAOO8Xm
WEAO4+xIYKnT67cyn9Sc4UWYdbV31o/lfrx9LsNy4z1KyYDzZr3wt/Q/Czog4jmk
dDDIuaabmYYWKxaYFQ2t85k1NOApJuVOH+N5AyM8qhbwP4XwvymVqLUmcJZPR14O
MQIDAQAB
-----END PUBLIC KEY-----
</pre>

*An alternate method*
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.pri -pubout > rsa.pub
writing RSA key
</pre>

#### - Here's how to inspect RSA private key
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.pri -noout -text
Private-Key: (2048 bit, 2 primes)
modulus:
    00:9b:b7:fa:3a:3e:be:8b:20:88:a5:be:2b:ba:30:
    60:8b:30:9f:8c:7e:af:0a:e2:2b:11:73:bb:90:98:
    70:ed
publicExponent: 65537 (0x10001)
privateExponent:
    03:cf:2f:cf:54:49:d5:ac:59:84:4e:3d:6e:f0:7a:
    c0:1b:fb:78:38:fa:b0:3c:d3:32:2c:f0:3b:df:4e:
    a1
prime1:
    00:cb:84:88:f2:a3:ca:b4:ef:7d:9a:8f:e1:89:e6:
    d5:85:48:d5:92:12:17:d5:45
prime2:
    00:c3:df:ee:51:a6:7f:cf:b6:1c:41:56:b3:c5:aa:
    ab:4b:7e:5d:9c:d5:a5:56:ce:30:12:ec:a0:52:ea:
    8d:e1:e4:c5:4e:7f:b1:83:89
exponent1:
    00:a6:22:94:14:d6:e6:73:db:89:02:32:70:1f:bc:
    a7:87:fd:53:f2:c0:f8:59:ad
exponent2:
    00:bd:b6:3e:c9:c0:5c:9c:5e:42:c1:99:eb:26:71:
    7c:7b:f3:19:52:14:af:3e:03:18:16:cc:2e:80:00:
    83:89:1d:9d:68:69:4f:12:19
coefficient:
    0b:7c:7c:22:52:3f:50:af:2c:39:de:a3:4d:c1:b8:
    b2:8b:45:64:75:5c:86:b8:12:da:99:81:1a:58:10:
    41:2a:ce:3f:ee:3d:fa:0b
</pre>

#### - Inspecting RSA public key.
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.pub -pubin -noout -text
Public-Key: (2048 bit)
Modulus:
    00:9b:b7:fa:3a:3e:be:8b:20:88:a5:be:2b:ba:30:
    61:7d:03:68:4c:5e:2d:ca:54:c4:96:bf:f7:c7:4f:
    be:37:d5:af:99:fb:53:8e:26:60:24:ff:53:36:75:
    60:8b:30:9f:8c:7e:af:0a:e2:2b:11:73:bb:90:98:
    70:ed
Exponent: 65537 (0x10001)
</pre>

#### - Here's how to convert PEM encoded keys to DER encoding.
> [!TIP]
> DER-encoded keys are ASN.1-encoded binary keys.
> Unlike PEM-encoded keys, they have a smaller footprint.
> DER encoding is useful when keys need to be stored or transmitted with minimal overhead.

<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.pri -out rsa.pri.der -outform der
writing RSA key

hashi@ub-ch1:~$ ls rsa.pri rsa.pri.der -l
-rw------- 1 hashi hashi 1704 May  3 12:53 rsa.pri
-rw------- 1 hashi hashi 1218 May  3 12:59 rsa.pri.der
</pre>


#### - Here's how to inspect a DER encoded private key.
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.pri -inform der -noout -text
Private-Key: (2048 bit, 2 primes)
modulus:
    00:8f:1a:26:77:75:db:b4:c7:0a:fd:d1:07:7b:fb:
    ff:ae:e0:c3:6e:d4:5a:96:8e:f1:d3:1a:b4:cd:59:
    12:e0:a7:c5:2a:64:ea:dc:b7:8f:a5:5b:05:d5:9a:
    28:fb
publicExponent: 65537 (0x10001)
privateExponent:
    17:41:5e:56:49:8e:9e:83:e8:e8:5d:eb:83:c8:41:
    c4:c1:3c:c3:59:d9:3a:f8:43:ee:82:a7:ed:c4:20:
    19
prime1:
    00:c3:dc:c1:c9:5a:16:79:67:da:37:cc:09:f1:5a:
    c9:dd:e1:b2:e4:16:05:90:1f
prime2:
    00:bb:0a:51:69:d1:8d:9e:95:04:65:2a:ce:ec:d7:
    70:62:0c:37:b8:ea:da:72:10:b8:14:53:cc:3b:8f:
    05:61:8a:f6:be:e4:0f:1b:a5
exponent1:
    12:06:ef:8f:28:cb:88:c1:16:44:ac:72:fd:d3:e6:
    52:df:c5:ad:63:de:39:d5:3b:9d:b7:0e:80:dc:89:
    17:f2:fe:14:20:32:c2:e1
exponent2:
    7c:83:45:e7:57:e5:1b:28:c0:a0:48:7b:30:17:bb:
    5d:f8:1c:30:31:c7:f2:1f:47:2b:e1:0d:18:a9:01:
    40:0a:2e:70:ad:89:91:31
coefficient:
    1d:41:5d:60:22:b8:92:b2:77:58:47:04:09:4f:da:
    b0:39:29:b7:87:90:2c:f7
</pre>

#### - Generating encrypted private key file.
> [!IMPORTANT]
> Private keys are intended to be kept safe and confidential.
> By default, RSA keys generated using OpenSSL are unencrypted.
> However, it is strongly recommended to generate encrypted private keys, which can be done by specifying a cipher during key generation.

<pre>
hashi@ub-ch1:~$ openssl genrsa -aes-256-cbc -out rsa.pri
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:

hashi@ub-ch1:~$ cat rsa.pri 
-----BEGIN ENCRYPTED PRIVATE KEY-----
MIIFNTBfBgkqhkiG9w0BBQ0wUjAxBgkqhkiG9w0BBQwwJAQQSyfxF4yl99yqhEsE
pinCTlVSgUxbJIvUS91U1GN7I2ZH2FLSEoXS434WsMWXFh6vTXyt8M1DORZjm6fp
4AQA2qJWfRfh2emYT6b67/jQ6m2widCV1R5vNDssb8VnUzhcNs0OH8UtTjhKX/AW
9efc3qjt3rVqMyhyjo9QDVM8I98mtkg/LE9td+HOUpVKT4g9ihoL9ho=
-----END ENCRYPTED PRIVATE KEY-----
</pre>

<br>

## Generate DSA keys
*This section covers aspects of generating an DSA key pair.*

#### - To generate DSA keypair, a DSA param is required.
<pre>
hashi@ub-ch1:~$ openssl dsaparam -out dsa.param 2048
hashi@ub-ch1:~$ cat dsa.param 
-----BEGIN DSA PARAMETERS-----
MIICKAKCAQEA1lICCaJSIZp24cWvkmeBWJqzIMtlr661jcZsZ5htqS7BUkTZ7Qe6
VYG4pEz0x5NJT4qihVMHp/qM/kU6CrQwcVZ8IjDgkLncLNBNi7A7FmFXWoKrJHNF
MPPtdmOlPU7YzX7YqkGvEfD9+G3MYijbhCEugSIWme3y5E6cgZIUPRb4qbVeMEZL
HGhokJfqM6GYhBGV2IREHDA06nAjWwr5K8FZCg==
-----END DSA PARAMETERS-----
</pre>

#### - Now we can use this param file to generate DSA keypair.
<pre>
hashi@ub-ch1:~$ openssl gendsa -out dsa.pri dsa.param 
hashi@ub-ch1:~$ cat dsa.pri 
-----BEGIN PRIVATE KEY-----
MIICXAIBADCCAjUGByqGSM44BAEwggIoAoIBAQDWUgIJolIhmnbhxa+SZ4FYmrMg
y2WvrrWNxmxnmG2pLsFSRNntB7pARo9fM/AeKt8mUsQbYWQO5/jP161Nlrcn7fSB
syMdydFeE6qCRKNCiLDcb0mvDSEXkwxildFsnQ3PZvlPQkprugLFZ0HQI3l4HDRE
BB4CHHyH410Hj2OCsYXG4IqlOFoaZyxUcBNQjo+v20U=
-----END PRIVATE KEY-----
</pre>

#### - Here's how to generate an encrypted DSA private key.
<pre>
hashi@ub-ch1:~$ openssl gendsa -aes-128-cbc -out dsa.pri dsa.param 
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:

hashi@ub-ch1:~$ cat dsa.pri 
-----BEGIN ENCRYPTED PRIVATE KEY-----
MIIC1TBfBgkqhkiG9w0BBQ0wUjAxBgkqhkiG9w0BBQwwJAQQRFWVK0n3Jj9SnLPK
7Tz2a7cmfBRakx0ZzYDHbkACkJBV9GQFtHF6rDft6cOgFNoxz7x2vswccIge98/w
eD22cVCyPfEs
-----END ENCRYPTED PRIVATE KEY-----
</pre>

#### - Extracting DSA public key from the private key.
> [!NOTE]  
> As I am using a password-protected private key, OpenSSL will prompt me to enter the password each time it is accessed.

<pre>
hashi@ub-ch1:~$ openssl dsa -in dsa.pri -pubout > dsa.pub
read DSA key
Enter pass phrase for dsa.pri:
writing DSA key

hashi@ub-ch1:~$ cat dsa.pub 
-----BEGIN PUBLIC KEY-----
MIIDQjCCAjUGByqGSM44BAEwggIoAoIBAQDWUgIJolIhmnbhxa+SZ4FYmrMgy2Wv
e1t3IxTfEzLOGs+oeiUL3x30B/dkT2llDLPJsZKBntl5H7yp4LZjDOw4dpcBN3aw
tDSrcRLT5Sxx/kMCMSG8FmqfFMCEjw==
-----END PUBLIC KEY-----
</pre>

#### - Another way to get the public key.
<pre>
hashi@ub-ch1:~$ openssl dsa -in dsa.pri -pubout -out dsa.pub 
read DSA key
Enter pass phrase for dsa.pri:
writing DSA key
</pre>

#### - Inspecting a DSA keypair.
<pre>
hashi@ub-ch1:~$ openssl dsa -in dsa.pri -noout -text
read DSA key
Enter pass phrase for dsa.pri:
Private-Key: (2048 bit)
priv:
    00:89:09:cb:4c:15:a6:8b:7c:9c:10:13:fa
pub: 
    38:67:e6:e7:ac:12:2c:c6:35:e0:2b:6b:9c:42:d9:
    0c:ec:38:76:97:01:37:76:b0:b4:34:ab:71:12:d3:
    e5:2c:71:fe:43:02:31:21:bc:16:6a:9f:14:c0:84:
    8f
P:   
    00:d6:52:02:09:a2:52:21:9a:76:e1:c5:af:92:67:
    81:58:9a:b3:20:cb:65:af:ae:b5:8d:c6:6c:67:98:
    8f:a9
Q:   
    00:87:7f:65:8b:56:c7:11:3a:07:f7:95:c9:bf:8c:
    23:b8:c2:fd:af:27:7e:7e:29:7d:e1:5c:d7:f5
G:   
    37:34:90:a4:44:71:da:3f:f2:55:65:1c:9c:59:87:
    30:46:4b:1c:68:68:90:97:ea:33:a1:98:84:11:95:
    d8:84:44:1c:30:34:ea:70:23:5b:0a:f9:2b:c1:59:
    0a

hashi@ub-ch1:~$ openssl dsa -in dsa.pub -pubin -noout -text
read DSA key
Public-Key: (2048 bit)
pub: 
    38:67:e6:e7:ac:12:2c:c6:35:e0:2b:6b:9c:42:d9:
    23:c4:6e:49:bb:2f:ae:b6:9a:43:94:d6:5f:5c:fb:
    8f
P:   
    00:d6:52:02:09:a2:52:21:9a:76:e1:c5:af:92:67:
    35:40:03:e5:65:91:f6:2e:85:cb:44:56:f6:2c:e0:
    8f:a9
Q:   
    00:87:7f:65:8b:56:c7:11:3a:07:f7:95:c9:bf:8c:

G:   
    37:34:90:a4:44:71:da:3f:f2:55:65:1c:9c:59:87:

</pre>

<BR>


## Generating ECDSA keypair.

*This section covers aspects of generating an ECDSA key pair.*

#### - Listing all ECDSA curves supported by OpenSSL.
<pre>
hashi@ub-ch1:~$ openssl ecparam -list_curves | grep secp
  secp112r1 : SECG/WTLS curve over a 112 bit prime field
  secp112r2 : SECG curve over a 112 bit prime field
  secp128r1 : SECG curve over a 128 bit prime field
  secp128r2 : SECG curve over a 128 bit prime field
  secp160k1 : SECG curve over a 160 bit prime field
  secp160r1 : SECG curve over a 160 bit prime field
  secp160r2 : SECG/WTLS curve over a 160 bit prime field
</pre>

#### - Generating ecdsa param file using an ECDSA curve.
<pre>
hashi@ub-ch1:~$ openssl ecparam -name secp384r1 -out ecdsa.param

hashi@ub-ch1:~$ cat ecdsa.param 
-----BEGIN EC PARAMETERS-----
BgUrgQQAIg==
-----END EC PARAMETERS-----
</pre>

#### - Generating ECDSA private key using the param file.
<pre>
hashi@ub-ch1:~$ openssl ecparam -genkey -in ecdsa.param -out ecdsa.pri -noout

hashi@ub-ch1:~$ cat ecdsa.pri 
-----BEGIN EC PRIVATE KEY-----
MIGkAgEBBDDZ0ke8Elyz0+28XDgbaFfBUxjH9JufnsM0PaL6PRDNhwcnldGs9mNq
LL1yVtGHijsWMcjXvBWWjNR2t5SSQm0=
-----END EC PRIVATE KEY-----
</pre>

#### - You could also generate ECDSA private key without generating a param file.
<pre>
hashi@ub-ch1:~$ openssl ecparam -genkey -name secp384r1 -noout -out ecdsa.pri

hashi@ub-ch1:~$ cat ecdsa.pri 
-----BEGIN EC PRIVATE KEY-----
MIGkAgEBBDCv+h+hEit3PEI14PtOAMURixMRLgUbXvGQ+QXYKG69ogkrwuealK5n
C/yVF/sjL1eaAz4EQdO4ks1k/pe90nY=
-----END EC PRIVATE KEY-----
</pre>

#### - Here's how to extract a public key from the private key.
<pre>
hashi@ub-ch1:~$ openssl ec -in ecdsa.pri -pubout -out ecdsa.pub
read EC key
writing EC key

hashi@ub-ch1:~$ cat ecdsa.pub 
-----BEGIN PUBLIC KEY-----
moVjEdnekle2NdRXRVUflGRvz7AMU3K8
-----END PUBLIC KEY-----
</pre>

#### - Here's how to inspect an ecdsa private key.
<pre>
hashi@ub-ch1:~$ openssl ec -in ecdsa.pri -noout -text
read EC key
Private-Key: (384 bit)
priv:
    e3:55:7a:d2:fc:66:11:32:7c:3c:34:91:90:9f:ed:
    e8:b1:0c
pub:
    04:37:7b:a2:d7:dd:4a:55:4b:3a:84:79:9a:e2:8b:
    6f:cf:b0:0c:53:72:bc
ASN1 OID: secp384r1
NIST CURVE: P-384
</pre>

#### - Inspecting an ecdsa public key.
<pre>
hashi@ub-ch1:~$ openssl ec -in ecdsa.pub -pubin -noout -text
read EC key
Public-Key: (384 bit)
pub:
    04:37:7b:a2:d7:dd:4a:55:4b:3a:84:79:9a:e2:8b:
    6f:cf:b0:0c:53:72:bc
ASN1 OID: secp384r1
NIST CURVE: P-384
</pre>

#### - Generating an encrypted ecdsa private key.
<pre>
hashi@ub-ch1:~$ openssl ecparam -name secp384r1 -genkey | openssl ec -aes-256-cbc -out ecdsa.pri
read EC key
writing EC key
Enter pass phrase for PEM:
Verifying - Enter pass phrase for PEM:
hashi@ub-ch1:~$ cat ecdsa.pri 
-----BEGIN EC PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-256-CBC,756458GHS1JGF891911E3CD00D8564H12

YZeW9wPYkGu1RLDXziErngrA6LryQbWUT6QcqOx0anoGUz40aH3echyrjsD9B+H8
IcAUKusyPdkcluvJ3ncLcDjUuFuTRPzX2FmfxGuUPdI=
-----END EC PRIVATE KEY-----
</pre>

<br>

## Generating keypairs using 'openssl genpkey' option.

'openssl genpkey' is a modern alternative to legacy commands like genrsa, gendsa, and ecparam. Instead of remembering a separate command for each key type (RSA, DSA, ECDSA, etc.), genpkey provides a unified interface for generating all types of asymmetric keys. It also supports generating keypairs for algorithms that OpenSSL may add in the future.

#### - Generating RSA private key.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm rsa -quiet -out rsa.pri
</pre>

#### - Generating RSA private key with a specified keysize and public exponent.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm rsa -pkeyopt rsa_keygen_bits:2048 -pkeyopt rsa_keygen_pubexp:17 -quiet -out rsa.pri
</pre>

#### - Generating an encrypted ECDSA private key.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm ec -pkeyopt ec_paramgen_curve:secp384r1 -aes-256-cbc -out ec.pri
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:
</pre>

#### - # Generating an EDDSA private key.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm ED25519 -out eddsa.pri
</pre>

#### - Extract public key from a private key.
<pre>
hashi@ub-ch1:~$ openssl pkey -in rsa.pri -pubout -out rsa.pub

ashi@ub-ch1:~$ openssl pkey -in ec.pri -pubout -out ec.pub
Enter pass phrase for ec.pri:
</pre>


#### - Inspecting a private key.
<pre>
hashi@ub-ch1:~$ openssl pkey -in rsa.pri -noout -text
Private-Key: (2048 bit, 2 primes)
modulus:
    00:c7:13:b8:da:25:42:a0:ff:2c:4e:cc:b8:5a:e7:
    d6:bc:c1:da:76:16:a7:51:da:9a:ee:06:08:97:d3:
    b2:3d
publicExponent: 17 (0x11)
privateExponent:
    00:a3:f2:1f:c2:b5:45:ed:ff:51:aa:4e:3d:78:0a:
    84:4a:a8:36:c6:6a:10:4e:38:6d:df:64:6e:c0:3f:
    b0:78:06:0b:ba:f0:60:dc:f9:bc:27:1e:9d:7d:eb:
    4e:f9
prime1:
    00:f7:2a:ee:e9:2c:1c:8c:93:39:f3:f6:97:49:5c:
    e0:af:32:de:2a:43:65:7a:3b:74:ed:34:ea:cf:5c:
    d4:62:0d:1d:d0:be:af:f2:1b
prime2:
    00:ce:30:dc:26:23:95:d6:c3:2c:64:da:44:18:d8:
    20:1d:8c:d5:54:09:61:72:87
exponent1:
    3a:28:38:36:dd:33:e4:d7:58:ee:1b:e7:5c:8e:52:
    35:30:43:40:2c:de:1a:d9
exponent2:
    3c:a4:f5:74:a1:0d:f3:df:0d:0e:9a:8c:7f:c7:43:
    cc:74:b7:36:d5:95:21:af
coefficient:
    50:5e:98:a0:3d:c3:2d:d8:a4:02:f5:be:9d:e7:bf:
    b6:9b:63:99:27:d2:21:28

hashi@ub-ch1:~$ openssl pkey -in ec.pri -noout -text
Enter pass phrase for ec.pri:
Private-Key: (384 bit)
priv:
    60:af:c9:e2:de:d8:07:6c:e7:08:4f:97:f5:78:5e:
    f2:ee:b4
pub:
    04:59:f9:40:2d:86:7b:09:b2:3e:28:d0:c7:8b:20:
    ec:ac:f8:39:a9:ae:d8
ASN1 OID: secp384r1
NIST CURVE: P-384

hashi@ub-ch1:~$ openssl pkey -in eddsa.pri -noout -text
ED25519 Private-Key:
priv:
    52:80:8b:1b:2b:98:0c:0c:ec:4b:51:a9:60:8f:e5:
    a5:a6:22:16:ea:47:76:e2:77:21:cd:79:c6:a6:54:
    0e:a3
pub:
    ef:68:6a:b2:67:ea:44:e5:29:29:ed:75:48:44:05:
    ca:39:97:e2:ef:4e:31:f9:5c:3c:19:d6:b5:80:4e:
    ef:7f
</pre>

<BR>

[Back to main page](README.md)
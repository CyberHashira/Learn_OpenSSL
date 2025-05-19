## USING OQSPROVIDER

In this note, I will demonstrate how to use the OQSProvider with OpenSSL. The OQSProvider is part of the Open Quantum Safe project and allows users to experiment with post-quantum secure algorithms. It leverages the liboqs library to perform all post-quantum cryptographic operations.


### PREREQUISITES

Both the OQSProvider and liboqs must be built from source. Therefore, the following development packages are expected to be installed on the system.
- **CMake :** a cross-platform build generator.
- **Ninja :** a building tool.
- **Doxygen :** document generator.
- **OpenSSL-dev :** OpenSSL's development files.

**OpenSSL version**
<pre>
hashi@ub-ch1:~$ openssl version
OpenSSL 3.0.13 30 Jan 2024 (Library: OpenSSL 3.0.13 30 Jan 2024)
</pre>

**Installing prerequisites on Ubuntu**
<pre>
hashi@ub-ch1:~$ sudo apt install cmake ninja-build doxygen libssl-dev -y

hashi@ub-ch1:~$ cmake -version
cmake version 3.28.3

hashi@ub-ch1:~$ ninja --version
1.11.1

hashi@ub-ch1:~$ doxygen -v
1.9.8
</pre>

**Download the source tarball for oqsprovider and liboqs.**
<pre>
hashi@ub-ch1:~$ mkdir pqc
hashi@ub-ch1:~$ cd pqc/

hashi@ub-ch1:~/pqc$ wget https://github.com/open-quantum-safe/liboqs/archive/refs/tags/0.13.0.tar.gz
hashi@ub-ch1:~/pqc$ wget https://github.com/open-quantum-safe/oqs-provider/archive/refs/tags/0.8.0.tar.gz

hashi@ub-ch1:~/pqc$ ls -l
total 16268
-rw-rw-r-- 1 hashi hashi 16432953 May 18 18:49 0.13.0.tar.gz
-rw-rw-r-- 1 hashi hashi   222427 May 18 18:51 0.8.0.tar.gz
</pre>

<br>

### STEP 1: BUILD AND INSTALL LIBOQS LIBRARY.

We will begin by building and installing the liboqs library from source. This library will be used in the next step to build the oqsprovider.

#### - Untar liboqs tarball.
<pre>
hashi@ub-ch1:~/pqc$ tar xf 0.13.0.tar.gz 
hashi@ub-ch1:~/pqc$ cd liboqs-0.13.0/
</pre>

#### - Generate build files using cmake.
<pre>
hashi@ub-ch1:~/pqc/liboqs-0.13.0$ cmake -DCMAKE_INSTALL_PREFIX=/opt/oqs -DBUILD_SHARED_LIBS=ON -GNinja .
-- The C compiler identification is GNU 13.3.0
-- The ASM compiler identification is GNU
-- Found assembler: /usr/bin/cc
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Performing Test CC_SUPPORTS_WA_NOEXECSTACK
-- Performing Test CC_SUPPORTS_WA_NOEXECSTACK - Success
-- Performing Test LD_SUPPORTS_WL_Z_NOEXECSTACK
-- Performing Test LD_SUPPORTS_WL_Z_NOEXECSTACK - Success
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Alg enablement unchanged
-- Found OpenSSL: /usr/lib/x86_64-linux-gnu/libcrypto.so (found suitable version "3.0.13", minimum required is "1.1.1")  
-- Looking for aligned_alloc
-- Looking for aligned_alloc - found
-- Looking for posix_memalign
-- Looking for posix_memalign - found
-- Looking for memalign
-- Looking for memalign - found
-- Looking for explicit_bzero
-- Looking for explicit_bzero - found
-- Looking for explicit_memset
-- Looking for explicit_memset - not found
-- Looking for memset_s
-- Looking for memset_s - not found
-- Found Doxygen: /usr/bin/doxygen (found version "1.9.8") found components: doxygen missing components: dot
-- Configuring done (0.7s)
-- Generating done (0.1s)
-- Build files have been written to: /home/hashi/pqc/liboqs-0.13.0
</pre>

#### - Execute build process using ninja.
<pre>
hashi@ub-ch1:~/pqc/liboqs-0.13.0$ ninja
[1968/1968] Linking C executable tests/vectors_ke
</pre>

#### - Install liboqs library.
<pre>
hashi@ub-ch1:~/pqc/liboqs-0.13.0$ sudo ninja install
[0/1] Install the project...
-- Install configuration: ""
-- Installing: /opt/oqs/lib/cmake/liboqs/liboqsConfig.cmake
-- Installing: /opt/oqs/lib/cmake/liboqs/liboqsConfigVersion.cmake
-- Installing: /opt/oqs/lib/pkgconfig/liboqs.pc
-- Installing: /opt/oqs/lib/liboqs.so.0.13.0
-- Up-to-date: /opt/oqs/lib/liboqs.so.7
-- Up-to-date: /opt/oqs/lib/liboqs.so
-- Installing: /opt/oqs/lib/cmake/liboqs/liboqsTargets.cmake
-- Installing: /opt/oqs/lib/cmake/liboqs/liboqsTargets-noconfig.cmake
-- Up-to-date: /opt/oqs/include/oqs/oqs.h
-- Up-to-date: /opt/oqs/include/oqs/aes_ops.h
-- Up-to-date: /opt/oqs/include/oqs/common.h
-- Up-to-date: /opt/oqs/include/oqs/rand.h
-- Up-to-date: /opt/oqs/include/oqs/sha2_ops.h
-- Up-to-date: /opt/oqs/include/oqs/sha3_ops.h
-- Up-to-date: /opt/oqs/include/oqs/sha3x4_ops.h
-- Up-to-date: /opt/oqs/include/oqs/kem.h
-- Up-to-date: /opt/oqs/include/oqs/sig.h
-- Up-to-date: /opt/oqs/include/oqs/sig_stfl.h
-- Up-to-date: /opt/oqs/include/oqs/kem_bike.h
-- Up-to-date: /opt/oqs/include/oqs/kem_frodokem.h
-- Up-to-date: /opt/oqs/include/oqs/kem_ntruprime.h
-- Up-to-date: /opt/oqs/include/oqs/kem_classic_mceliece.h
-- Up-to-date: /opt/oqs/include/oqs/kem_kyber.h
-- Up-to-date: /opt/oqs/include/oqs/kem_ml_kem.h
-- Up-to-date: /opt/oqs/include/oqs/sig_dilithium.h
-- Up-to-date: /opt/oqs/include/oqs/sig_ml_dsa.h
-- Up-to-date: /opt/oqs/include/oqs/sig_falcon.h
-- Up-to-date: /opt/oqs/include/oqs/sig_sphincs.h
-- Up-to-date: /opt/oqs/include/oqs/sig_mayo.h
-- Up-to-date: /opt/oqs/include/oqs/sig_cross.h
-- Up-to-date: /opt/oqs/include/oqs/sig_uov.h
-- Installing: /opt/oqs/include/oqs/oqsconfig.h
hashi@ub-ch1:~/pqc/liboqs-0.13.0$ cd ..
</pre>

<br>

### STEP 2: BUILD AND INSTALL OQSPROVIDER.

Now that liboqs library is installed, we'll use it to build oqsprovider.

#### - Untar OQSProvider tarball.
<pre>
hashi@ub-ch1:~/pqc$ tar xf 0.8.0.tar.gz 
hashi@ub-ch1:~/pqc$ cd oqs-provider-0.8.0/
</pre>

#### - Generate build files for OQSProvider using cmake.
<pre>
hashi@ub-ch1:~/pqc/oqs-provider-0.8.0$ liboqs_DIR=/opt/oqs/ cmake -S . -B _build
-- The C compiler identification is GNU 13.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Creating Release build
-- Build will store public keys in PKCS#8 structures
-- Found OpenSSL: /usr/lib/x86_64-linux-gnu/libcrypto.so (found suitable version "3.0.13", minimum required is "3.0")  
-- liboqs found: Include dir at /opt/oqs/include;/opt/oqs/include/oqs (SHARED_LIBRARY)
-- Building commit  in /home/hashi/pqc/oqs-provider-0.8.0
Adding liboqs as dynamic dependency for the debian package
-- Configuring done (0.2s)
-- Generating done (0.0s)
-- Build files have been written to: /home/hashi/pqc/oqs-provider-0.8.0/_build
</pre>

#### - Start building OQSProvider
<pre>
hashi@ub-ch1:~/pqc/oqs-provider-0.8.0$ cmake --build _build/
[  3%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqsprov.c.o
[  6%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqsprov_capabilities.c.o
[  9%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqsprov_keys.c.o
[ 12%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqs_kmgmt.c.o
[ 16%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqs_sig.c.o
[ 19%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqs_kem.c.o
[ 22%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqs_encode_key2any.c.o
[ 25%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqs_endecoder_common.c.o
[ 29%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqs_decode_der2key.c.o
[ 32%] Building C object oqsprov/CMakeFiles/oqsprovider.dir/oqsprov_bio.c.o
[ 35%] Linking C shared module ../lib/oqsprovider.so
[ 35%] Built target oqsprovider
[ 38%] Building C object test/CMakeFiles/oqs_test_signatures.dir/oqs_test_signatures.c.o
[ 41%] Building C object test/CMakeFiles/oqs_test_signatures.dir/test_common.c.o
[ 45%] Linking C executable oqs_test_signatures
[ 45%] Built target oqs_test_signatures
[ 48%] Building C object test/CMakeFiles/oqs_test_kems.dir/oqs_test_kems.c.o
[ 51%] Building C object test/CMakeFiles/oqs_test_kems.dir/test_common.c.o
[ 54%] Linking C executable oqs_test_kems
[ 54%] Built target oqs_test_kems
[ 58%] Building C object test/CMakeFiles/oqs_test_groups.dir/oqs_test_groups.c.o
[ 61%] Building C object test/CMakeFiles/oqs_test_groups.dir/test_common.c.o
[ 64%] Building C object test/CMakeFiles/oqs_test_groups.dir/tlstest_helpers.c.o
[ 67%] Linking C executable oqs_test_groups
[ 67%] Built target oqs_test_groups
[ 70%] Building C object test/CMakeFiles/oqs_test_tlssig.dir/oqs_test_tlssig.c.o
[ 74%] Building C object test/CMakeFiles/oqs_test_tlssig.dir/test_common.c.o
[ 77%] Building C object test/CMakeFiles/oqs_test_tlssig.dir/tlstest_helpers.c.o
[ 80%] Linking C executable oqs_test_tlssig
[ 80%] Built target oqs_test_tlssig
[ 83%] Building C object test/CMakeFiles/oqs_test_endecode.dir/oqs_test_endecode.c.o
[ 87%] Building C object test/CMakeFiles/oqs_test_endecode.dir/test_common.c.o
[ 90%] Linking C executable oqs_test_endecode
[ 90%] Built target oqs_test_endecode
[ 93%] Building C object test/CMakeFiles/oqs_test_evp_pkey_params.dir/oqs_test_evp_pkey_params.c.o
[ 96%] Building C object test/CMakeFiles/oqs_test_evp_pkey_params.dir/test_common.c.o
[100%] Linking C executable oqs_test_evp_pkey_params
[100%] Built target oqs_test_evp_pkey_params
</pre>

#### - Install OQSProvider.
<pre>
hashi@ub-ch1:~/pqc/oqs-provider-0.8.0$ sudo cmake --install _build/
-- Install configuration: ""
-- Installing: /usr/lib/x86_64-linux-gnu/ossl-modules/oqsprovider.so
-- Set non-toolchain portion of runtime path of "/usr/lib/x86_64-linux-gnu/ossl-modules/oqsprovider.so" to ""
-- Up-to-date: /usr/local/include/oqs-provider/oqs_prov.h
</pre>

<BR>

###  ACTIVATING OQSPROVIDER.

#### - Test if OpenSSL is able to load OQSProvider.
<pre>
hashi@ub-ch1:~$ export LD_LIBRARY_PATH=/opt/oqs/lib/

hashi@ub-ch1:~$ openssl list -provider oqsprovider -providers
Providers:
  oqsprovider
    name: OpenSSL OQS Provider
    version: 0.8.0
    status: active
</pre>

#### - Edit openssl.cnf file to add entries to load oqsprovider.
<pre>
hashi@ub-ch1:~$ openssl version -d
OPENSSLDIR: "/usr/lib/ssl"

hashi@ub-ch1:~$ sudo nano /usr/lib/ssl/openssl.cnf
</pre>

#### - Add oqsprovider entry under provider_sect.
<pre>
[provider_sect]
default = default_sect
oqsprovider = oqs_sect

[oqs_sect]
activate = 1

[default_sect]
activate = 1
</pre>

#### - Save the changes and then list providers again.
<pre>
hashi@ub-ch1:~$ openssl list -providers
Providers:
  default
    name: OpenSSL Default Provider
    version: 3.0.13
    status: active
  oqsprovider
    name: OpenSSL OQS Provider
    version: 0.8.0
    status: active
</pre>

<BR>

### USING OQSPROVIDER.


#### - List key encapsulation algorithms.
<pre>
hashi@ub-ch1:~$ openssl list -kem-algorithms
  frodo640aes @ oqsprovider
  p256_frodo640aes @ oqsprovider
  x25519_frodo640aes @ oqsprovider
  frodo640shake @ oqsprovider
  p256_frodo640shake @ oqsprovider
  x25519_frodo640shake @ oqsprovider
  frodo976aes @ oqsprovider
</pre>

#### - List signature algorithms.
<pre>
hashi@ub-ch1:~$ openssl list -signature-algorithms
  dilithium2 @ oqsprovider
  p256_dilithium2 @ oqsprovider
  rsa3072_dilithium2 @ oqsprovider
  dilithium3 @ oqsprovider
  p384_dilithium3 @ oqsprovider
  dilithium5 @ oqsprovider
  p521_dilithium5 @ oqsprovider
</pre>

#### - List public key algorithms.
<pre>
hashi@ub-ch1:~$ openssl list -public-key-algorithms | grep oqsprovider
    IDs: dilithium2 @ oqsprovider
    IDs: p256_dilithium2 @ oqsprovider
    IDs: rsa3072_dilithium2 @ oqsprovider
    IDs: dilithium3 @ oqsprovider
    IDs: p384_dilithium3 @ oqsprovider
    IDs: dilithium5 @ oqsprovider
</pre>

#### - Generating key.
**FALCON-512**
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm falcon512 -out falcon-512.key

hashi@ub-ch1:~$ openssl pkey -in falcon-512.key -noout -text
falcon512 private key:
PQ key material:
    59:ec:00:7e:10:31:84:f3:de:7a:0f:ef:c8:03:f0:
    c4:f4:2f:3c:07:ff:44:0c:00:01:f4:1f:fc:f4:40:
    00:fb:f0:fe:f0:20:02:00:0e:44:f3:ed:c1:10:11:
</pre>

**MLDSA-44**
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm mldsa44 -out mldsa-44.key

hashi@ub-ch1:~$ openssl pkey -in mldsa-44.key -noout -text
mldsa44 private key:
PQ key material:
    ad:82:ad:64:c4:98:14:d2:37:5f:eb:69:b6:85:d8:
    e9:92:7e:46:4d:71:96:ab:36:99:9e:0d:e2:3f:a5:

hashi@ub-ch1:~$ openssl pkey -in mldsa-44.key -pubout -out mldsa-44.pub
</pre>

#### - Signing a file and verifying its signature.
<pre>
hashi@ub-ch1:~$ echo -n "Hello World, I've been waiting for the chance to see your face." > earth.txt

hashi@ub-ch1:~$ openssl pkeyutl -sign -inkey mldsa-44.key -digest sha256 -rawin -in earth.txt -out earth.txt.sig

hashi@ub-ch1:~$ openssl pkeyutl -verify -inkey mldsa-44.pub -pubin -digest sha256 -rawin -sigfile earth.txt.sig -in earth.txt
Signature Verified Successfully
</pre>

<br>

[Back to main page](README.md)
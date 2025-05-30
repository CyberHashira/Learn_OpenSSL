### OpenSSL C Samples

These are the C sample codes that makes using of OpenSSL API. All these codes were testing using OpenSSL 3.5.0.
I have also categorised the samples codes as follow:


| Categories | Description                  |
|-----------:|------------------------------|
| `encryption` | samples to demonstrate encryption using OpenSSL API. |
| `signing` | samples demonstrating digital signing using OpenSSL API. |
| `hashing` | samples demonstrating message digest operation. |
| `using_keys` | samples demonstrating how to generate keys and load them. |
| `misc` | Some uncategorised sample codes. |

<br>

### Prerequisites:
- GCC  : for compiling and building the code.
- Make : For executing the Makefile.

<br>

### Display Help.

<pre>
hashi@ub-ch1:~/sample_codes$ make help
make            : Builds all sample code.
make all        : Builds all sample code.
make using_keys : Builds all samples demonstrating key generation.
make hashing    : Builds all message digest samples.
make encryption : Builds all encryption related samples.
make signing    : Builds all digital signing samples.
make misc       : Builds all micellaneous samples.
make clean      : Deletes all sample binaries.

</pre>

<br>

### Building the Sample Codes.

#### - Building all samples at once.
*Running 'make all' will also build all sample code files.*
<pre>
hashi@ub-ch1:~/sample_codes$ make
  - using_keys samples build successfully. Executables are inside bin/using_keys.
  - hashing samples build successfully. Executables are inside bin/hashing.
  - encryption samples build successfully. Executables are inside bin/encryption.
  - signing samples build successfully. Executables are inside bin/signing.
  - misc samples build successfully. Executables are inside bin/misc.
</pre>

#### - Alternatively, you can specify the category of samples you wish to compile.
<pre>
hashi@ub-ch1:~/sample_codes$ make signing
signing samples build successfully. Executables are inside bin/signing.

hashi@ub-ch1:~/sample_codes$ make encryption
encryption samples build successfully. Executables are inside bin/encryption.

hashi@ub-ch1:~/sample_codes$ tree bin/
bin/
├── encryption
│   ├── encrypting_using_aes_cbc
│   ├── encrypting_using_rsa_oaep
│   ├── encrypting_using_rsa_oaep2
│   └── encrypting_using_rsa_pkcs1
└── signing
    ├── sign_using_mldsa
    ├── sign_using_rsa_pss
    ├── sign_using_sha256_digest
    ├── sign_using_sha512_digest
    ├── verify_mldsa_signature
    └── verify_sha256_digest_signature
</pre>

#### - Cleaning up.
<pre>
hashi@ub-ch1:~/sample_codes$ make clean
All executables removed.
</pre>

<br>

### Manually Compiling the Sample Codes.
*Here is how you can manually compile a sample code if you choose not to use the Makefile.*

#### - Compiling the code
<pre>
hashi@ub-ch1:~/sample_codes$ gcc signing/sign_using_mldsa.c -o sign_using_mldsa -I/opt/openssl/include -L/opt/openssl/lib64 -lcrypto
</pre>

#### - Running the executable without argument will display the syntax.
<pre>
hashi@ub-ch1:~/sample_codes$ ./sign_using_mldsa 
Usage :-
./sign_using_mldsa <file_to_sign> <PEM_encoded_private_key>
</pre>

#### - Running the executable.
<pre>
hashi@ub-ch1:~/sample_codes$ openssl genpkey -algorithm mldsa44 -out mldsa.key

hashi@ub-ch1:~/sample_codes$ ./sign_using_mldsa Makefile mldsa.key 
 --> File to sign read.
 --> Signing key loaded.
 --> Data signed.
 --> Signature written to Makefile.sig.

hashi@ub-ch1:~/sample_codes$ openssl pkeyutl -verify -inkey mldsa.key -sigfile Makefile.sig -in Makefile
Signature Verified Successfully
</pre>

<br>

[Back to main page](../README.md)
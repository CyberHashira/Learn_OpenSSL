## DIGITAL SIGNING USING OPENSSL

In this section of my notes, I will demonstrate how to digitally sign a file using OpenSSL.

#### - Here's how I generated all signing keys.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm rsa -quiet -out rsa.pri && openssl pkey -in rsa.pri -pubout -out rsa.pub
hashi@ub-ch1:~$ openssl ecparam -name secp384r1 -genkey -out ecdsa.pri && openssl pkey -in ecdsa.pri -pubout -out ecdsa.pub
hashi@ub-ch1:~$ ls -l
total 16
-rw------- 1 hashi hashi  359 May  4 18:32 ecdsa.pri
-rw-rw-r-- 1 hashi hashi  215 May  4 18:32 ecdsa.pub
-rw------- 1 hashi hashi 1704 May  4 18:32 rsa.pri
-rw-rw-r-- 1 hashi hashi  451 May  4 18:32 rsa.pub
</pre>

#### - Here's the file I'll be signing.
<pre>
hashi@ub-ch1:~$ echo "Earth is the third planet of our Solar System." > earth.txt
</pre>

<BR>

### SIGNING AND VERIFYING USING RSAUTL
[!NOTE]
> 'openssl rsautl' is deprecated from version 3.x onwards. Please use 'openssl pkeyutl' instead.

#### - Signing earth.txt using rsautl (OLD DEPRECATED METHOD).
<pre>
hashi@ub-ch1:~$ openssl rsautl -sign -inkey rsa.pri -in earth.txt -out earth.txt.sig
The command rsautl was deprecated in version 3.0. Use 'pkeyutl' instead.
</pre>

#### - Verifying Signature using rsautl (OLD DEPRECATED METHOD).
<pre>
hashi@ub-ch1:~$ openssl rsautl -verify -inkey rsa.pub -pubin -in earth.txt.sig 
The command rsautl was deprecated in version 3.0. Use 'pkeyutl' instead.
Earth is the third planet of our Solar System.
</pre>

<BR>

### GENERATING HASH BASED SIGNATURES.

#### - Generating sha1WithRSA signature and verifying it.
<pre>
hashi@ub-ch1:~$ openssl sha1 -sign rsa.pri -out earth.txt.sig earth.txt 

hashi@ub-ch1:~$ openssl sha1 -verify rsa.pub -signature earth.txt.sig earth.txt
Verified OK
</pre>

#### - Generating sha256WithRSA signature and verifying it.
<pre>
hashi@ub-ch1:~$ openssl sha256 -sign rsa.pri -out earth.txt.sig earth.txt

hashi@ub-ch1:~$ openssl sha256 -verify rsa.pub -signature earth.txt.sig earth.txt
Verified OK
</pre>

#### - Generating sha1withECDSA signature and verifying it.
<pre>
hashi@ub-ch1:~$ openssl sha1 -sign ecdsa.pri -out earth.txt.sig earth.txt

hashi@ub-ch1:~$ openssl sha1 -verify ecdsa.pub -signature earth.txt.sig earth.txt
Verified OK
</pre>

#### - Generating sha256WithECDSA signature and verifying it.
<pre>
hashi@ub-ch1:~$ openssl sha256 -sign ecdsa.pri -out earth.txt.sig earth.txt

hashi@ub-ch1:~$ openssl sha256 -verify ecdsa.pub -signature earth.txt.sig earth.txt
Verified OK
</pre>

<br>

### HASH BASED DIGITAL SIGNING USING **openssl dgst** COMMAND.

**SYNTAX : ** openssl pkeyutl <options> <file_to_sign>
OPTIONS
| OPTION | DESCRIPTION |
| --- | --- |
| -sha1 | use sha1 hash. |
| -sha256 | use sha256 hash. |
| -sign <private_key> | sign using the private key from a file. |
| -verify <public_key> | verify using public key from a file. |
| -out | name of the file to write signature into. |
| -signature | file containing the signature to verify. |
| -sigopt | signing options to use. For example, pss padding. |


#### - Generating sha256withRSA signature and verifying it.
<pre>
hashi@ub-ch1:~$ openssl dgst -sha256 -sign rsa.pri -out earth.txt.sig earth.txt

hashi@ub-ch1:~$ openssl dgst -sha256 -verify rsa.pub -signature earth.txt.sig earth.txt
Verified OK
</pre>

#### - Generating sha256WithRSA-PSS and verifying it.
<pre>
hashi@ub-ch1:~$ openssl dgst -sha256 -sign rsa.pri -sigopt rsa_padding_mode:pss -out earth.txt.sig earth.txt
hashi@ub-ch1:~$ openssl dgst -sha256 -verify rsa.pub -sigopt rsa_padding_mode:pss -signature earth.txt.sig earth.txt
Verified OK
</pre>

#### - Generating sha256WithECDSA signature and verifying it.
<pre>
hashi@ub-ch1:~$ openssl dgst -sha256 -sign ecdsa.pri -out earth.txt.sig earth.txt

hashi@ub-ch1:~$ openssl dgst -sha256 -verify ecdsa.pub -signature earth.txt.sig earth.txt
Verified OK
</pre>

<br>

### DIGITAL SIGNING USING **'openssl pkeyutl'**

**SYNTAX : ** openssl pkeyutl <options>
OPTIONS
| OPTION | DESCRIPTION |
| --- | --- |
| -sign | initiate signing operation. |
| -verify | initiate signature verification. |
| -in | name of the file to sign. |
| -out | name of the file write signatures into. |
| -sigfile | name of the file to read signature from. |
| -rawin | indicates that the data is not hashed. |
| -digest | digest algorithm to use. |
| -pkeyopt | additional public key options. E.g. PSS |


#### - Signing and verifying using sha256WithRSA.
<pre>
hashi@ub-ch1:~$ openssl pkeyutl -sign -rawin -digest sha256 -inkey rsa.pri -in earth.txt -out earth.txt.sig 

hashi@ub-ch1:~$ openssl pkeyutl -verify -rawin -digest sha256 -inkey rsa.pub -pubin -sigfile earth.txt.sig -in earth.txt
Signature Verified Successfully
</pre>

#### - Signing and verifying using sha256WithECDSA.
<pre>
hashi@ub-ch1:~$ openssl pkeyutl -sign -digest sha256 -rawin -inkey ecdsa.pri -in earth.txt -out earth.txt.sig 

hashi@ub-ch1:~$ openssl pkeyutl -verify -digest sha256 -rawin -inkey ecdsa.pub -pubin -sigfile earth.txt.sig -in earth.txt
Signature Verified Successfully
</pre>

#### - Signing a pre-hashed data.
<pre>
hashi@ub-ch1:~$ openssl sha256 -binary -out earth.txt.sha256 earth.txt

hashi@ub-ch1:~$ openssl pkeyutl -sign -inkey rsa.pri -in earth.txt.sha256 -out earth.txt.sig

hashi@ub-ch1:~$ openssl pkeyutl -verify -inkey rsa.pub -pubin -sigfile earth.txt.sig -in earth.txt.sha256
Signature Verified Successfully
</pre>

#### - Signing a pre-hashed data using RSA-PSS padding scheme.
<pre>
hashi@ub-ch1:~$ openssl sha256 -binary -out earth.txt.sha256 earth.txt

hashi@ub-ch1:~$ openssl pkeyutl -sign -inkey rsa.pri -pkeyopt rsa_padding_mode:pss -pkeyopt digest:sha256 -in earth.txt.sha256 -out earth.txt.sig

hashi@ub-ch1:~$ openssl pkeyutl -verify -inkey rsa.pub -pubin -pkeyopt rsa_padding_mode:pss -pkeyopt digest:sha256 -in earth.txt.sha256 -sigfile earth.txt.sig 
Signature Verified Successfully
</pre>

<br>

[Back to main](README.md)
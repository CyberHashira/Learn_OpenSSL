## CMS (Cryptographic Message Syntax) using OpenSSL

### Useful links :-
https://docs.openssl.org/3.5/man1/openssl-cms/
https://www.rfc-editor.org/rfc/rfc5652

<br>

### CMS OPTIONS

| OPTION | DESCRIPTION |
| --- | --- |
| -in | input file to be signed, verified, encrypted or decrypted. |
| -out | output file. (signed or encrypted) |
| -inkey | private key used to signing or decrypt a message. |
| -signer | certificate of the message signer. |
| -certfile	| additional certificates (intermediate CAs.) |
| -CAfile	| trusted certificates file to use for message verification. |
| -recip | certificate of the message recipient. |
| -md	| digest algorithm to use. |
| -encrypt | encrypts a message |
| -decrypt | decrypts an encrypted message. |
| -sign	| signs a message. |
| -verify	| verifies a signed message. |
| -resign	| additional signature to a signed message. |
| -cmsout	|	outputs the CMS structure. |
| -print	| used with -cmsout; prints the cms structure as text.|
| -nodetach	|	embed message into CMS structure. |
| -nocerts	| omit certificates from CMS output. |
| -noattr	|	omit attributes from CMS output |
| -nosmimecap	| omit S/MIME capability from CMS output. |
| -noverify	|	skips certificate verification. |
| -certsout	|	outputs certificates from a message. |
| -secretkey	|	use hex-encoded secret key for encryption. |
| -secretkeyid	| identifier to the secret key. |

<br>

### GENERATING SIGNING KEY AND CERTIFICATE.

#### - Generating private key.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm rsa -quiet -out cms.key
</pre>

#### - Generate self-signed certificate
<pre>
hashi@ub-ch1:~$ openssl req -x509 -days 365 -sha256 -subj '/CN=CMS' -key cms.key -addext keyUsage=digitalSignature -addext basicConstraints=CA:false -out cms.cer
</pre>

#### - Here's the message I'll be signing.
<pre>
hashi@ub-ch1:~$ echo "Climate change is real. It is happening right now, it is the most urgent threat facing our entire species." > message.txt
</pre>

<br>

### Signing and Verifying a message using self-signed certificates

This section demonstrates how to sign a mesage using a self-signed certificate.

#### - Signing a message
<pre>
hashi@ub-ch1:~$ openssl cms -sign -in message.txt -out message.msg -inkey cms.key -signer cms.cer
</pre>

#### - Verify a message
*A message signed using a self-signed certificate will always return an error during verification.*
<pre>
hashi@ub-ch1:~$ openssl cms -verify -in message.msg 
CMS Verification failure
801BD72EC1790000:error:17000064:CMS routines:cms_signerinfo_verify_cert:certificate verify error:crypto/cms/cms_smime.c:291:Verify error: self-signed certificate
</pre>

#### - Instruct OpenSSL to ignore the self-signed certificate by using the -noverify option.
<pre>
hashi@ub-ch1:~$ openssl cms -verify -in message.msg -noverify
Climate change is real. It is happening right now, it is the most urgent threat facing our entire species.
CMS Verification successful
</pre>

#### - Inspecting a signed message.
<pre>
hashi@ub-ch1:~$ openssl cms -cmsout -print -in message.msg 
CMS_ContentInfo: 
  contentType: pkcs7-signedData (1.2.840.113549.1.7.2)
  d.signedData: 
    version: 1
    digestAlgorithms:
        algorithm: sha256 (2.16.840.1.101.3.4.2.1)
</pre>

<br>

### SIGNING AND VERIFYING MESSAGE USING A CA-SIGNED CERTIFICATE.

This section describes how to sign a message using a CA signed certificate.

#### - Key and certificates I'll be using.
- cacert.cer : contains the certificate.
- cms.cer : is a CA signed certificate.
- cms.key : is a private key.

<pre>
hashi@ub-ch1:~$ ls -l
-r--r--r-- 1 hashi hashi 13284 May 10 13:59 cacert.cer
-rw-rw-r-- 1 hashi hashi  1493 May 10 13:59 cms.cer
-rw------- 1 hashi hashi  1704 May 10 13:59 cms.key
</pre>

#### - Simple Sign/Verify using ca-signed certificate
>[!NOTE]
> When a CA-signed certificate is used to sign a message, CMS expects the entire certificate chain to be provided during verification.
> Without a cacerts.cer file contains all relevant CA certificates, the verification will fail.
> Alternatively, the -noverify option can be used to disable certificate chain verification.

<pre>
hashi@ub-ch1:~$ openssl cms -sign -in message.txt -out message.msg -inkey cms.key -signer cms.cer

hashi@ub-ch1:~$ openssl cms -verify -in message.msg
CMS Verification failure
801B22C700760000:error:17000064:CMS routines:cms_signerinfo_verify_cert:certificate verify error:crypto/cms/cms_smime.c:291:Verify error: unable to get local issuer certificate

hashi@ub-ch1:~$ openssl cms -verify -in message.msg -CAfile cacert.cer 
Climate change is real. It is happening right now, it is the most urgent threat facing our entire species.
CMS Verification successful

hashi@ub-ch1:~$ openssl cms -verify -in message.msg -noverify
Climate change is real. It is happening right now, it is the most urgent threat facing our entire species.
CMS Verification successful
</pre>

#### - Signing a message with the signer and CA certificates included.
>[!NOTE]
> Including the CA chain within a message will also increase its overall size.
> Verification still fails in the example below because the certificate was signed by a private CA that is not publicly trusted.
> -noverify option disable certificate verification.

<pre>
hashi@ub-ch1:~$ openssl cms -sign -in message.txt -out message.msg -inkey cms.key -signer cms.cer -certfile cacert.cer

hashi@ub-ch1:~$ openssl cms -verify -in message.msg 
CMS Verification failure
80EBFC1F77790000:error:17000064:CMS routines:cms_signerinfo_verify_cert:certificate verify error:crypto/cms/cms_smime.c:291:Verify error: self-signed certificate in certificate chain

hashi@ub-ch1:~$ openssl cms -verify -in message.msg -noverify
Climate change is real. It is happening right now, it is the most urgent threat facing our entire species.
CMS Verification successful
</pre>

#### - Here is how you can extract all certificates embedded within a message.
<pre>
hashi@ub-ch1:~$ openssl cms -verify -noverify -in message.msg -certsout all_certs.cer
Climate change is real. It is happening right now, it is the most urgent threat facing our entire species.
CMS Verification successful

hashi@ub-ch1:~$ ls all_certs.cer -lh
-rw-rw-r-- 1 hashi hashi 15K May 10 14:54 all_certs.cer
</pre>

#### - Sign a message without embedding the signing certificate.
*Signing a message without embedding the certificate, reduces the size of the signed message.
<pre>
hashi@ub-ch1:~$ openssl cms -sign -in message.txt -out message.msg -inkey cms.key -signer cms.cer -nocerts

hashi@ub-ch1:~$ ls -l message.msg 
-rw-rw-r-- 1 hashi hashi 1472 May 10 19:29 message.msg
</pre>

#### - Verifying a message without an embedded certificate.
*signer certificate is required to verify a message file without a certificate. Without it, the verification would fail.*
<pre>
hashi@ub-ch1:~$ openssl cms -verify -in message.msg 
CMS Verification failure
80ABE64FA6730000:error:1700008A:CMS routines:CMS_verify:signer certificate not found:crypto/cms/cms_smime.c:350:

hashi@ub-ch1:~$ openssl cms -verify -in message.msg -certfile cms.cer -CAfile cacert.cer 
Climate change is real. It is happening right now, it is the most urgent threat facing our entire species.
CMS Verification successful
</pre>

#### - Producing a signed output without headers or the message in clear text.
*The size of the output can be further reduced by excluding headers, certificate and the message in clear*
<pre>
hashi@ub-ch1:~$ openssl cms -sign -in message.txt -out message.msg -inkey cms.key -signer cms.cer -nocerts -nodetach
hashi@ub-ch1:~$ ls -l message.msg 
-rw-rw-r-- 1 hashi hashi 1244 May 10 19:49 message.msg

hashi@ub-ch1:~$ openssl cms -verify -in message.msg -certfile cms.cer -CAfile cacert.cer 
Climate change is real. It is happening right now, it is the most urgent threat facing our entire species.
CMS Verification successful
</pre>

#### - Generating a signed message with the smallest possible footprint.
*You can produce the smallest possible signed message by:*
*- omiting signer certificate as an attachment*
*- omiting CA certificates as an attachment*
*- omiting message in clear*
*- omiting headers*
*- omiting attributes*
*- omiting smime capabilities.*
*= producting DER encoded output.*.

<pre>
hashi@ub-ch1:~$ openssl cms -sign -in message.txt -outform der -out message.msg -inkey cms.key -signer cms.cer -nocerts -nodetach -noattr -nosmimecap

hashi@ub-ch1:~$ ls -l message.msg 
-rw-rw-r-- 1 hashi hashi 547 May 11 09:58 message.msg

hashi@ub-ch1:~$ openssl cms -verify -in message.msg -inform der -certfile cms.cer -CAfile cacert.cer 
Climate change is real. It is happening right now, it is the most urgent threat facing our entire species.
CMS Verification successful
</pre>

<br>

### ENCRYPTING A MESSAGE USING CMS

This section describes how to encrypt a message using CMS. For this section I have prepared the following keys, certificates and a message.

<pre>
hashi@ub-ch1:~/messaging$ tree
.
├── Alice
│   ├── Alice.cer
│   └── Alice.key
├── Bob
│   ├── Bob.cer
│   └── Bob.key
├── Manager
│   ├── Manager.cer
│   └── Manager.key
├── Outsider
│   ├── Outsider.cer
│   └── Outsider.key
├── cacert.cer
└── message.txt
</pre>

#### - A simple message encryption example.
> [!NOTE]
> An encrypted envelope contains the encrypted message, the encrypted secret key, and the algorithm used for encryption.
> By default, OpenSSL uses AES-256-CBC for message encryption.
> In the example below, the message is being encrypt for Alice, Bob and the Manager.

<pre>
hashi@ub-ch1:~/messaging$ openssl cms -encrypt -in message.txt -out message.enc Alice/Alice.cer Bob/Bob.cer Manager/Manager.cer
</pre>

#### - Decryping the encrypted message.
>[!NOTE]
> The -recip option is useful when decrypting a message intended for multiple recipients.
> Without -recip, openssl cms tries to decrypt each encrypted key block sequentially until it finds one that works.
> An error is thrown only if none of the encrypted keys match the provided private key.

<pre>
hashi@ub-ch1:~/messaging$ openssl cms -decrypt -in message.enc -inkey Alice/Alice.key -recip Alice/Alice.cer 
We are the first generation to feel the sting of climate change, and we are the last generation that can do something about it.

hashi@ub-ch1:~/messaging$ openssl cms -decrypt -in message.enc -inkey Bob/Bob.key -recip Bob/Bob.cer 
We are the first generation to feel the sting of climate change, and we are the last generation that can do something about it.

hashi@ub-ch1:~/messaging$ openssl cms -decrypt -in message.enc -inkey Manager/Manager.key 
We are the first generation to feel the sting of climate change, and we are the last generation that can do something about it.

hashi@ub-ch1:~/messaging$ openssl cms -decrypt -in message.enc -inkey Outsider/Outsider.key -recip Outsider/Outsider.cer 
Error decrypting CMS using private key

hashi@ub-ch1:~/messaging$ openssl cms -decrypt -in message.enc -inkey Outsider/Outsider.key 
Error decrypting CMS structure
803B2E333E720000:error:1C800064:Provider routines:ossl_cipher_unpadblock:bad decrypt:providers/implementations/ciphers/ciphercommon_block.c:107:
</pre>

#### - Inspecting an encrypted message.
<pre>
hashi@ub-ch1:~/messaging$ openssl cms -cmsout -print -in message.enc 
CMS_ContentInfo: 
  contentType: pkcs7-envelopedData (1.2.840.113549.1.7.3)
  d.envelopedData: 
    version: 0
    originatorInfo: <ABSENT>
    recipientInfos:
</pre>

#### - Encrypting a message using a specified algorithm.
<pre>
hashi@ub-ch1:~/messaging$ openssl cms -encrypt -aes-128-ofb -in message.txt -out message.enc Alice/Alice.cer Bob/Bob.cer

hashi@ub-ch1:~/messaging$ openssl cms -cmsout -print -in message.enc | grep algorithm
          algorithm: rsaEncryption (1.2.840.113549.1.1.1)
          algorithm: rsaEncryption (1.2.840.113549.1.1.1)
        algorithm: aes-128-ofb (2.16.840.1.101.3.4.1.3)
</pre>

#### - Encrypt a message with a user specified encryption key
<pre>
hashi@ub-ch1:~/messaging$ openssl rand -out aes.key 32

hashi@ub-ch1:~/messaging$ openssl cms -encrypt -in message.txt -out message.enc -secretkey `xxd -c 64 -p aes.key` -secretkeyid 1234 -aes-256-cbc Outsider/Outsider.cer 

hashi@ub-ch1:~/messaging$ openssl cms -decrypt -in message.enc -inkey Outsider/Outsider.key 
We are the first generation to feel the sting of climate change, and we are the last generation that can do something about it.
</pre>

<BR>

[Back to main](README.md)
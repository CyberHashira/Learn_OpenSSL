## OPENSSL KEY FORMATS

This note explains how to generate keys in various formats using OpenSSL, including PKCS#1, SEC1, PKCS#7, PKCS#8, PKCS#10, and PKCS#12. It will also cover how to convert between these formats.


### KEY FORMATS

#### - PKCS1 FORMATTED RSA PRIVATE KEY.
<pre>
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----
</pre>

#### - SEC1 FORMATTED ECDSA KEY.
<pre>
-----BEGIN EC PRIVATE KEY-----
-----END EC PRIVATE KEY-----
</pre>

#### - PKCS8 FORMATTED PRIVATE.
<pre>
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
</pre>

#### - ENCRYPTED PKCS1 FORMATTED RSA KEY.
<pre>
-----BEGIN RSA PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-128-CBC,C84D0ED32DE7231B31F3322335EF79E9
</pre>

#### - ENCRYPTED SEC1 FORMATTED EC KEY.
<pre>
----BEGIN EC PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-256-CBC,F3CA755127856475EDB10DAC6E6FF8FE
</pre>

#### - ENCRYPTED PKCS8 FORMATTED PRIVATE KEY.
<pre>
-----BEGIN ENCRYPTED PRIVATE KEY-----
-----END ENCRYPTED PRIVATE KEY-----
</pre>

#### - PKCS7 FILE.
<pre>
-----BEGIN PKCS7-----
-----END PKCS7-----
</pre>

<br>

### GENERATING KEYS.
>[!NOTE]
> Since OpenSSL 3.0, private keys are generated using the PKCS#8 standard by default.
> To generate keys in PKCS#1 or SEC1 format, use the -traditional option.
> By default, OpenSSL also uses PEM encoding for all cryptographic objects.

#### - Generating RSA private keys. (Defaults to PKCS8).
<pre>
hashi@ub-ch1:~$ openssl genrsa -out rsa.key
hashi@ub-ch1:~$ cat rsa.key | grep "\-\-\-\-\-"
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
</pre>

#### - Generating RSA private key using the traditional format.
<pre>
hashi@ub-ch1:~$ openssl genrsa -traditional -out rsa.key 
hashi@ub-ch1:~$ cat rsa.key | grep "\-\-\-\-\-"
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----
</pre> 

#### - Generating ECDSA private key.
*OpenSSL 3.x still uses the traditional format SEC1 for ECDSA keys.*
<pre>
hashi@ub-ch1:~$ openssl ecparam -genkey -name prime256v1 -noout -out ec.key

hashi@ub-ch1:~$ cat ec.key | grep "\-\-\-\-\-"
-----BEGIN EC PRIVATE KEY-----
-----END EC PRIVATE KEY-----
</pre>

#### - Generating PKCS8 formatted ECDSA private key.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm ec -pkeyopt ec_paramgen_curve:secp384r1 -out ec.key 
hashi@ub-ch1:~$ cat ec.key | grep '\-\-\-\-\-'
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
</pre>

#### - Generating DER encoded private key.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm rsa -quiet -outform der -out rsa.der

hashi@ub-ch1:~$ openssl genpkey -algorithm ec -pkeyopt ec_paramgen_curve:secp384r1 -out ec.der -outform der
</pre>

<BR>

### SWITCHING BETWEEN PEM AND DER ENCODING.

#### - Converting PEM encoded RSA key to DER.

*Method 1*
<pre>
hashi@ub-ch1:~$ openssl pkey -in rsa.pem -out rsa.der -outform der
</pre>

*Method 2*
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.pem -out rsa2.der -outform der
writing RSA key
</pre>

#### - Converting PEM encoded EC key to DER.

*Method 1*
<pre>
hashi@ub-ch1:~$ openssl pkey -in ec.der -out ec.pem -outform pem
</pre>

*Method 2*
<pre>
hashi@ub-ch1:~$ openssl ec -in ec.der -out ec.pem -outform pem
read EC key
writing EC key
</pre>

#### - Inspecting a DER encoded key.
> [!NOTE]
> -inform der option is optional in the newer version of OpenSSL.

**Method 1**
<pre>
hashi@ub-ch1:~$ openssl pkey -in rsa.der -inform der -noout -text

hashi@ub-ch1:~$ openssl pkey -in ec.der -inform der -noout -text

</pre>

**Method 2**
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.der -inform der -noout -text

hashi@ub-ch1:~$ openssl ec -in ec.der -inform der -noout -text
</pre>

#### - Converting DER encoded RSA key to PEM.

**Method 1**
<pre>
hashi@ub-ch1:~$ openssl pkey -in rsa.der -out rsa.pem -outform pem
</pre>

**Method 2**
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa2.der -inform der -outform pem -out rsa2.pem
writing RSA key
</pre>

<BR>

### ENCRYPTING PRIVATE KEYS.

#### - Encrypting RSA Private Keys.

*Method 1*
<pre>
ashi@ub-ch1:~$ openssl rsa -aes-256-cbc -in rsa.key -out rsa.enc
writing RSA key
Enter pass phrase:
Verifying - Enter pass phrase:

hashi@ub-ch1:~$ cat rsa.enc | grep '\-\-\-\-\-'
-----BEGIN ENCRYPTED PRIVATE KEY-----
-----END ENCRYPTED PRIVATE KEY-----
</pre>

*Method 2*
<pre>
hashi@ub-ch1:~$ openssl pkey -in rsa.key -out rsa.enc -aes-128-cbc
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:

hashi@ub-ch1:~$ cat rsa.enc | grep '\-\-\-\-\-'
-----BEGIN ENCRYPTED PRIVATE KEY-----
-----END ENCRYPTED PRIVATE KEY-----
</pre>

#### - Encrypting RSA private key with in a traditional format.
<pre>
hashi@ub-ch1:~$ openssl pkey -in rsa.key -out rsa.enc -aes-128-cbc -traditional
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:

hashi@ub-ch1:~$ cat rsa.enc 
-----BEGIN RSA PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-128-CBC,C84D0ED32DE7231B31F3322335EF79E9
</pre>


#### - Removing encryption from an RSA private key.
<pre>
hashi@ub-ch1:~$ openssl rsa -in rsa.enc -out rsa.pem 
Enter pass phrase for rsa.enc:
writing RSA key

hashi@ub-ch1:~$ cat rsa.pem | grep '\-\-\-\-\-'
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
</pre>

#### - Encrypting an ECDSA private key.
<pre>
hashi@ub-ch1:~$ openssl ec -in ec.pem -out ec2.pem -aes-256-cbc
read EC key
writing EC key
Enter pass phrase for PEM:
Verifying - Enter pass phrase for PEM:

hashi@ub-ch1:~$ cat ec2.pem 
-----BEGIN EC PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-256-CBC,F3CA755127856475EDB10DAC6E6FF8FE
</pre>

#### - Removing encryption from an ECDSA private key.
<pre>
ashi@ub-ch1:~$ openssl pkey -in ec2.pem -out ec.pem
Enter pass phrase for ec2.pem:
</pre>

<br>


### SWITCHING FROM PKCS #1 to PKCS #8.
*Newer versions of OpenSSL defaults to pkcs8 format.*

#### - Converting PKCS#1 formatted key to PKCS#8 format key without encryption.

*Method 1*
<pre>
hashi@ub-ch1:~$ openssl pkey -in rsa.pem -out rsa2.pem

hashi@ub-ch1:~$ cat rsa.pem | grep '\-\-\-\-\-'
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----
hashi@ub-ch1:~$ cat rsa2.pem | grep '\-\-\-\-\-'
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
</pre>

*Method 2*
<pre>
hashi@ub-ch1:~$ openssl pkcs8 -in rsa.pem -topk8 -nocrypt -out rsa2.pem

hashi@ub-ch1:~$ cat rsa.pem | grep '\-\-\-\-\-'
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----
hashi@ub-ch1:~$ cat rsa2.pem | grep '\-\-\-\-\-'
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
</pre>


#### - Converting PKCS#1 formatted key to PKCS#8 format key with encryption.
<pre>
ashi@ub-ch1:~$ openssl pkcs8 -in rsa.pem -topk8 -out rsa2.pem
Enter Encryption Password:
Verifying - Enter Encryption Password:

hashi@ub-ch1:~$ cat rsa.pem | grep '\-\-\-\-\-'
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----

hashi@ub-ch1:~$ cat rsa2.pem | grep '\-\-\-\-\-'
-----BEGIN ENCRYPTED PRIVATE KEY-----
-----END ENCRYPTED PRIVATE KEY-----
</pre>

#### - Converting PKCS#1 to PKCS#8 formatted key with no encryption in DER format.
<pre>
hashi@ub-ch1:~$ openssl pkcs8 -in rsa.pem -topk8 -out rsa2.pem -nocrypt -outform der
</pre>


<BR>

### CREATING CERTIFICATE BUNDLE (PFB FILE)

#### - Files I'll be using
<pre>
hashi@ub-ch1:~/hashi$ ls -l
total 40
-r--r--r-- 1 hashi hashi 13284 May 11 18:35 cacert.cer (Contains issuer.cer + myroot.cer)
-rw-rw-r-- 1 hashi hashi  1497 May 11 18:35 hashi.cer (signed certificate)
-rw------- 1 hashi hashi  1704 May 11 18:35 hashi.key (private key)
-rw-rw-r-- 1 hashi hashi  5543 May 11 18:37 issuer.cer (Issuing CA certificate)
-rw-rw-r-- 1 hashi hashi  7741 May 11 18:38 myroot.cer (Root CA certificate)
</pre>

#### - Generating a P7B file of a signed certificate with certificate chain.
<pre>
hashi@ub-ch1:~/hashi$ openssl crl2pkcs7 -nocrl -certfile hashi.cer -certfile myroot.cer -certfile issuer.cer -out hashi.pfb

hashi@ub-ch1:~/hashi$ cat hashi.pfb | grep '\-\-\-\-\-'
-----BEGIN PKCS7-----
-----END PKCS7-----
</pre>

#### - You can also use the cacert file to generate P7B.
<pre>
hashi@ub-ch1:~/hashi$ openssl crl2pkcs7 -nocrl -certfile hashi.cer -certfile cacert.cer -out hashi.pfb
</pre>

#### - Inspecting a P7B file
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs7 -in hashi.pfb -print_certs
</pre>

#### - Inspecting a P7B file without outputting the certificates
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs7 -in hashi.pfb -print_certs -noout
subject=C=CA, ST=BC, O=Cyber Hashira, CN=Hashi
issuer=C=CA, ST=BC, O=CyberHashira, CN=myIssuer

subject=C=CA, ST=BC, O=CyberHashira, CN=myIssuer
issuer=C=CA, ST=BC, O=CyberHashira, CN=myRoot

subject=C=CA, ST=BC, O=CyberHashira, CN=myRoot
issuer=C=CA, ST=BC, O=CyberHashira, CN=myRoot
</pre>

<br>

### - GENERATING PKCS12 FILE.

#### - Creating a PKCS12 (PFX/P12) file.
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs12 -export -inkey hashi.key -in hashi.cer -out hashi.pfx
Enter Export Password:
Verifying - Enter Export Password:
</pre>

#### - Creating a PFX/P12 file with a certificate chain included.
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs12 -export -inkey hashi.key -in hashi.cer -certfile cacert.cer -out hashi.pfx
Enter Export Password:
Verifying - Enter Export Password:
</pre>

#### - Inspecting a PKCS12 file without displaying the key.
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs12 -in hashi.pfx -info -nokeys
Enter Import Password:
MAC: sha256, Iteration 2048
MAC length: 32, salt length: 8
PKCS7 Encrypted data: PBES2, PBKDF2, AES-256-CBC, Iteration 2048, PRF hmacWithSHA256
Certificate bag
Bag Attributes
    localKeyID: 2D ED 0F 96 58 50 6D 4E F5 55 DE 76 48 C9 6B C9 51 35 19 C3 
subject=C=CA, ST=BC, O=Cyber Hashira, CN=Hashi
issuer=C=CA, ST=BC, O=CyberHashira, CN=myIssuer
</pre>

#### - Inspecting a PKCS12 file without displaying keys and certs (-nokeys + -nocerts)
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs12 -in hashi.pfx -info -noout
Enter Import Password:
MAC: sha256, Iteration 2048
MAC length: 32, salt length: 8
PKCS7 Encrypted data: PBES2, PBKDF2, AES-256-CBC, Iteration 2048, PRF hmacWithSHA256
Certificate bag
Certificate bag
Certificate bag
PKCS7 Data
Shrouded Keybag: PBES2, PBKDF2, AES-256-CBC, Iteration 2048, PRF hmacWithSHA256
</pre>

#### - Inspecting a PKCS12 file to display only signed/client certificate
<pre>
ashi@ub-ch1:~/hashi$ openssl pkcs12 -in hashi.pfx -info -nokeys -clcerts
Enter Import Password:
MAC: sha256, Iteration 2048
MAC length: 32, salt length: 8
PKCS7 Encrypted data: PBES2, PBKDF2, AES-256-CBC, Iteration 2048, PRF hmacWithSHA256
Certificate bag
Bag Attributes
    localKeyID: 2D ED 0F 96 58 50 6D 4E F5 55 DE 76 48 C9 6B C9 51 35 19 C3 
subject=C=CA, ST=BC, O=Cyber Hashira, CN=Hashi
issuer=C=CA, ST=BC, O=CyberHashira, CN=myIssuer
</pre>

#### - Inspecting a PKCS12 file to display only CA certificates.
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs12 -in hashi.pfx -info -nokeys -cacerts
</pre>

#### - Extract private key from a PKCS12 file without encryption
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs12 -in hashi.pfx -nocerts -noenc | openssl rsa -out private.key
Enter Import Password:
writing RSA key
hashi@ub-ch1:~/hashi$ cat private.key 
-----BEGIN PRIVATE KEY-----
-----END PRIVATE KEY-----
</pre>

#### - Extract private key out of PKCS#12 file with encryption
<pre>
hashi@ub-ch1:~/hashi$ openssl pkcs12 -in hashi.pfx -nocerts -out rsa.pri
Enter Import Password:
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:

hashi@ub-ch1:~/hashi$ cat rsa.pri | grep '\-\-\-\-\-'
-----BEGIN ENCRYPTED PRIVATE KEY-----
-----END ENCRYPTED PRIVATE KEY-----
</pre>

#### Extract private key out of PKCS#12 file with encryption, with password taken as input from a file.
<pre>
hashi@ub-ch1:~/hashi$ openssl rand -hex -out passfile

hashi@ub-ch1:~/hashi$ openssl pkcs12 -in hashi.pfx -nocert -nodes | openssl pkey -aes-256-cbc -passout file:passfile -out private.key
Enter Import Password:
</pre>

#### - Inspecting ASN.1 Structure of a key.
<pre>
hashi@ub-ch1:~/hashi$ openssl asn1parse -in rsa.pri 
    0:d=0  hl=4 l=1333 cons: SEQUENCE          
    4:d=1  hl=2 l=  95 cons: SEQUENCE          
    6:d=2  hl=2 l=   9 prim: OBJECT            :PBES2
   17:d=2  hl=2 l=  82 cons: SEQUENCE          
   19:d=3  hl=2 l=  49 cons: SEQUENCE          
   21:d=4  hl=2 l=   9 prim: OBJECT            :PBKDF2
</pre>

<BR>

[Back to main](README.md)
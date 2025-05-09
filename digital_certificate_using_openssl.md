## GENERATING DIGITAL CERTIFICATES USING OPENSSL.

In this section of my notes, I will demonstrate how to generate digital certificates using OpenSSL.

<br>

**SYNTAX:** openssl req <options>

| OPTION | DESCRIPTION |
| --- | --- |
| -x509 | Output a certificate. |
| -key | private key to use. |
| -sha256 | use sha256 hash. |
| -days | no. of days the certificate will be valid for. |
| -out file-name> | output to a file. |
| -newkey | generate a private key. |
| -noenc | don't encrypt private key. |
| -subj subject-info | Supplies subject information via the command line, bypassing interactive prompts. |

### Generating Self-Signed certificates.

*This section covers how to generate a self-signed certificate. A self-signed certificate is one in which the owner of the private key certifies their own identity.*

#### - Generate a private key and a self-signed certificate.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm rsa -quiet -out rsa.pri

hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -sha256 -days 365 -out rsa.cer
Country Name (2 letter code) [AU]:CA
State or Province Name (full name) [Some-State]:BC
Locality Name (eg, city) []:.
Organization Name (eg, company) [Internet Widgits Pty Ltd]:Cyber Hashira
Organizational Unit Name (eg, section) []:.
Common Name (e.g. server FQDN or YOUR name) []:RSA-TEST
Email Address []:
</pre>

#### - Inspecting a certificate.
<pre>
hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -text
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            15:7f:65:5e:70:7b:f4:10:99:c8:bc:ac:14:c7:03:26:a6:bf:1d:b2
        Signature Algorithm: sha256WithRSAEncryption
</pre>

#### - Generating an RSA private key and certificate using one command.
<pre>
hashi@ub-ch1:~$ openssl req -x509 -newkey rsa:2048 -sha256 -nodes -days 365 -out rsa.cer
Country Name (2 letter code) [AU]:CA
State or Province Name (full name) [Some-State]:BC
Locality Name (eg, city) []:.
Organization Name (eg, company) [Internet Widgits Pty Ltd]:Cyber Hashira 
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []:RSATEST
Email Address []:

hashi@ub-ch1:~$ ls -l
total 12
-rw------- 1 hashi hashi 1704 May  7 17:41 privkey.pem
-rw-rw-r-- 1 hashi hashi 1245 May  7 17:41 rsa.cer
</pre>


#### - Generating an ECDSA private key and a self-signed certificate.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm ec -pkeyopt ec_paramgen_curve:secp384r1 -out ecdsa.pri

hashi@ub-ch1:~$ openssl req -x509 -key ecdsa.pri -days 365 -sha256 -subj '/CN=ECDSATEST' -out ecdsa.cer

hashi@ub-ch1:~$ openssl x509 -in ecdsa.cer -noout -text
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            52:08:a4:98:74:71:e4:5e:3c:e9:32:5f:00:da:92:df:f1:4f:09:84
        Signature Algorithm: ecdsa-with-SHA256
        Issuer: CN=ECDSATEST
</pre>

#### - Generating MLDSA private key and a self-signed certificate.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm mldsa65 -quiet -out mldsa.key

hashi@ub-ch1:~$ openssl req -x509 -key mldsa.key -days 365 -sha256 -subj '/CN=MLDSATEST' -out mldsa.cer
hashi@ub-ch1:~$ openssl x509 -in mldsa.cer -noout -text
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            57:70:91:43:ec:1f:47:1f:0e:4c:d4:2a:88:f7:7f:67:f8:36:54:ed
        Signature Algorithm: ML-DSA-65
        Issuer: CN=MLDSATEST
</pre>

<br>

### ADDING SUBJECT INFORMATION IN COMMAND LINE.
*The 'openssl req' command prompts the user to enter subject information interactively if it is not provided via the command line. This can be problematic in automated scripts. The -subj option allows subject details to be specified directly via the command line.*

#### - Adding COMMON NAME (CN) to the Subject DN.
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -days 365 -out rsa.cer -subj '/CN=RSATest'

hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -subject
subject=CN=RSATest
</pre>

#### - Adding CN, O, OU, C, ST, L and emailAddress to the SubjectDN.
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -days 365 -sha256 -out rsa.cer -subj '/CN=RSATEST/O=CyberHashira/OU=CyberSec/C=CA/ST=BC/L=YVR/emailAddress=pki@acme.com/'

hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -subject
subject=CN=RSATEST, O=CyberHashira, OU=CyberSec, C=CA, ST=BC, L=YVR, emailAddress=pki@acme.com
</pre>

<BR>

### ADDING CERTIFICATE EXTENSIONS.
*Certificate extensions are useful for defining the purpose of a certificate and restricting it to specific use cases.*

#### - Adding BasicConstraint (NON CA Certificate).
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -sha256 -days 365 -out rsa.cer -subj '/CN=RSATEST/O=CyberHashira' -addext "basicConstraints=CA:false"

hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -ext basicConstraints
X509v3 Basic Constraints: 
    CA:FALSE
</pre>

#### - Adding BasicConstraint marked as CRITICAL with a pathlen.
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -sha256 -days 365 -out rsa.cer -subj '/CN=RSATEST/O=CyberHashira' -addext "basicConstraints=critical, CA:true, pathlen:1"

hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -ext basicConstraints
X509v3 Basic Constraints: critical
    CA:TRUE, pathlen:1
</pre>

#### - Adding Subject Alternate Names.
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -sha256 -days 365 -out rsa.cer -subj '/CN=RSATEST' -addext "subjectAltName= DNS:cyberhashira,IP:127.0.0.1"

hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -ext subjectAltName
X509v3 Subject Alternative Name: 
    DNS:cyberhashira, IP Address:127.0.0.1
</pre>

#### - Adding Key Usage.

> [!NOTE]
> Accepted key usage values as per RFC-5280 are as follows:
> URL : https://www.rfc-editor.org/rfc/rfc5280

**Key Usage:**
- digitalSignature
- nonRepudiation
- cRLSign
- keyAgreement
- keyCertSign
- encipherOnly
- decipherOnly
- keyEncipherment
- dataEncipherment

<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -sha256 -days 365 -out rsa.cer -subj '/CN=RSATEST' -addext "keyUsage = digitalSignature"

hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -ext keyUsage
X509v3 Key Usage: 
    Digital Signature
</pre>

#### - Adding multiple Key Usage.
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -sha256 -days 365 -out rsa.cer -subj '/CN=RSATEST' -addext "keyUsage = digitalSignature,nonRepudiation,cRLSign"

hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -ext keyUsage
X509v3 Key Usage: 
    Digital Signature, Non Repudiation, CRL Sign
</pre>

#### - Adding extended Key Usage.
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -sha256 -days 365 -out rsa.cer -subj '/CN=RSATEST' -addext "extendedKeyUsage = serverAuth, clientAuth"

hashi@ub-ch1:~$ openssl x509 -in rsa.cer -noout -ext extendedKeyUsage
X509v3 Extended Key Usage: 
    TLS Web Server Authentication, TLS Web Client Authentication
</pre>

<br>

### USING CONFIGURATION FILE.

Instead of using a lengthy command to pass subject DN details and extensions, OpenSSL allows this information to be placed in a configuration file, which can then be used as input for generating certificates.


#### - A sample configuration file.

#### - EXAMPLE 1 | Generating certificate with just the Subject DN.

<pre>sample1.inf</pre>

```
[req]
distinguished_name = dname
prompt = no

commonName = CyberHashira
countryName = CA
stateOrProvinceName = BC
organizationName = CyberHashira
organizationalUnitName = PKI
emailAddress = pki@cyberhashira.com
```
</pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -config sample1.inf -out sample1.cer

hashi@ub-ch1:~$ openssl x509 -in sample1.cer -noout -subject
subject=CN=CyberHashira, C=CA, ST=BC, O=CyberHashira, OU=PKI, emailAddress=pki@cyberhashira.com
</pre>

#### - EXAMPLE 2 | Generating certificate with extensions
<pre>sample2.inf</pre>

```
[req]
x509_extensions = cert_ext
prompt = no

[ cert_ext ]
basicConstraints = CA:FALSE
keyUsage = digitalSignature, nonRepudiation
extendedKeyUsage = codeSigning
crlDistributionPoints=URI:http://myCA/ca.crl
subjectAltName = @sans
subjectKeyIdentifier=hash
authorityKeyIdentifier=keyid,issuer
authorityInfoAccess = OCSP;URI:http://ocsp.myCA/
authorityInfoAccess = caIssuers;URI:http://myCA/ca.cer
certificatePolicies= 1.2.4.5.6.7

[sans]
IP.1 = 127.0.0.1
DNS.1 = blog.cyberhashira.com
DNS.2 = video.cyberhashira.com
```
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -config sample2.inf -subj "/CN=SAMPLE2" -out sample2.cer

hashi@ub-ch1:~$ openssl x509 -in sample2.cer -noout -ext keyUsage 
X509v3 Key Usage: 
    Digital Signature, Non Repudiation

hashi@ub-ch1:~$ openssl x509 -in sample2.cer -noout -ext extendedKeyUsage 
X509v3 Extended Key Usage: 
    Code Signing

hashi@ub-ch1:~$ openssl x509 -in sample2.cer -noout -ext subjectAltName
X509v3 Subject Alternative Name: 
    IP Address:127.0.0.1, DNS:blog.cyberhashira.com, DNS:video.cyberhashira.com
</pre>

#### - EXAMPLE 3 | You can also selectively specify which section of the configuration file to use.
<pre>sample3.inf</pre>

```
[req]
distinguished_name = dname
x509_extensions = cert_ext
prompt = no

[with_dname]
distinguished_name = dname
prompt = no

[with_ext]
x509_extensions = cert_ext
prompt = no

[dname]
commonName = CyberHashira
countryName = CA
stateOrProvinceName = BC
organizationName = CyberHashira
organizationalUnitName = PKI
emailAddress = pki@cyberhashira.com

[cert_ext]
basicConstraints = CA:FALSE
keyUsage = digitalSignature, nonRepudiation
extendedKeyUsage = codeSigning
crlDistributionPoints=URI:http://myCA/ca.crl
subjectAltName = @sans
subjectKeyIdentifier=hash
authorityKeyIdentifier=keyid,issuer
authorityInfoAccess = OCSP;URI:http://ocsp.myCA/
authorityInfoAccess = caIssuers;URI:http://myCA/ca.cer
certificatePolicies= 1.2.4.5.6.7

[sans]
IP.1 = 127.0.0.1
DNS.1 = blog.cyberhashira.com
DNS.2 = video.cyberhashira.com
```
<pre>
hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -config sample.inf -section with_dname -out sample1.cer

hashi@ub-ch1:~$ openssl x509 -in sample1.cer -noout -subject
subject=CN=CyberHashira, C=CA, ST=BC, O=CyberHashira, OU=PKI, emailAddress=pki@cyberhashira.com

hashi@ub-ch1:~$ openssl req -x509 -key rsa.pri -config sample.inf -section with_ext -subj "/CN=Sample3" -out sample3.cer

ashi@ub-ch1:~$ openssl x509 -in sample3.cer -noout -ext keyUsage
X509v3 Key Usage: 
    Digital Signature, Non Repudiation
hashi@ub-ch1:~$ openssl x509 -in sample3.cer -noout -ext extendedKeyUsage
X509v3 Extended Key Usage: 
    Code Signing
</pre>

<br>

### SIGNING A CERTIFICATE REQUEST.
Another way to generate a certificate is by signing a certificate request file. This process requires a certificate authority (CA) to sign the request. In this section, I will demonstrate how to create a CA certificate and use it to sign a certificate request.

#### - Generating CA key and certificate.

<pre>CA.inf</pre>

```
[req]
distinguished_name = dname
x509_extensions = cert_ext
prompt = no

[ dname ]
CN = RootCA

[ cert_ext ]
basicConstraints = CA:TRUE, pathlen:0
keyUsage = keyCertSign, cRLSign
subjectKeyIdentifier=hash
```
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm rsa -quiet -out CA.pri
hashi@ub-ch1:~$ openssl req -x509 -key CA.pri -config CA.inf -out CA.cer
</pre>

#### - Generating a private key and certificate signing request.
<pre>
hashi@ub-ch1:~$ openssl genpkey -algorithm rsa -quiet -out webserver.pri

hashi@ub-ch1:~$ openssl req -new -key webserver.pri -subj '/CN=WebServer' -addext keyUsage=digitalSignature,keyEncipherment -addext extendedKeyUsage=serverAuth -out webserver.csr
</pre>

#### - Here's how to read certificate signing request file.
<pre>
hashi@ub-ch1:~$ openssl req -in webserver.csr -noout -text
Certificate Request:
    Data:
        Version: 1 (0x0)
        Subject: CN=WebServer
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                Public-Key: (2048 bit)
</pre>

#### - Signing certificate request.
<pre>
hashi@ub-ch1:~$ openssl x509 -req -days 365 -in webserver.csr -CA CA.cer -CAkey CA.pri -CAcreateserial -out webserver.cer
Certificate request self-signature ok
subject=CN=WebServer

hashi@ub-ch1:~$ openssl x509 -in webserver.cer -noout -text
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            24:53:82:9a:99:3f:87:5f:11:55:1c:56:c1:76:9c:67:bf:0d:76:c0
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: CN=RootCA
        Validity
            Not Before: May  9 01:23:37 2025 GMT
            Not After : May  9 01:23:37 2026 GMT
        Subject: CN=WebServer
</pre>

<br>

[Back to main page](README.md)

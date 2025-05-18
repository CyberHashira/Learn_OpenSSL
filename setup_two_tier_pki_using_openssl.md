## Setup Two-Tier PKI Using OpenSSL

In this note, I will demonstrate how to set up a two-tier Certificate Authority (CA) using OpenSSL. The two-tier structure consists of a Root CA and an Issuing CA.

I will also show how to use the Issuing CA to issue certificates for various purposes and revoke them when no longer required.


### STEP 1 : MAKE DIRECTORIES AND FILES FOR THE CA.

The first step is to create directories for the Root CA and the Issuing CA, so that files can be organised appropriately.

Below is a list of essential files and their purposes:
- index.dat : maintains an index of issued certificates and their status.
- crl_number : used as a base for indexing revoked certificates.
- serial.dat : used as a base for generating certificate serial numbers.

#### - Make Root CA directories and files.
<pre>
hashi@ub-ch1:~$ mkdir -p ca/rootca/{private,cert,issued_certs,crl,csr,data}
hashi@ub-ch1:~$ cd ca/rootca/
hashi@ub-ch1:~/ca/rootca$ touch data/index.dat
hashi@ub-ch1:~/ca/rootca$ openssl rand -hex -out data/serial.dat 8
hashi@ub-ch1:~/ca/rootca$ echo "1000" > data/crl_number
hashi@ub-ch1:~/ca/rootca$ chmod 600 data/*
hashi@ub-ch1:~/ca/rootca$ cd ..
hashi@ub-ch1:~/ca$ tree rootca/
rootca/
├── cert
├── crl
├── csr
├── data
│   ├── crl_number
│   ├── index.dat
│   └── serial.dat
├── issued_certs
└── private
</pre>

#### - Make Issuing CA directories and files.
<pre>
hashi@ub-ch1:~/ca$ mkdir -p issuingca/{private,cert,issued_certs,crl,csr,data}
hashi@ub-ch1:~/ca$ cd issuingca/
hashi@ub-ch1:~/ca/issuingca$ chmod 700 private/
hashi@ub-ch1:~/ca/issuingca$ touch data/index.dat
hashi@ub-ch1:~/ca/issuingca$ echo "1000" > data/crl_number
hashi@ub-ch1:~/ca/issuingca$ openssl rand -out data/serial.dat -hex 8
hashi@ub-ch1:~/ca/issuingca$ chmod 600 data/*
hashi@ub-ch1:~/ca/issuingca$ cd ..
hashi@ub-ch1:~/ca$ tree issuingca/
issuingca/
├── cert
├── crl
├── csr
├── data
│   ├── crl_number
│   ├── index.dat
│   └── serial.dat
├── issued_certs
└── private
</pre>

<br>

### Step 2 : WRITE CONFIGURATION FILE FOR THE ROOT CA, GENERATE CA KEY AND CERTIFICATE

We will begin by building the Root CA. To do this, we must first create a configuration file for the Root CA. Once that is in place, we will generate a private key and the Root CA certificate.

#### - Make a new directory for the Root CA.
<pre>
hashi@ub-ch1:~/ca$ cd rootca/
hashi@ub-ch1:~/ca/rootca$ nano rootca.cnf
</pre>

**Sample Configuration file for the ROOTCA (rootca.cnf).**

```
[ ca ]
default_ca = CA_default

[ CA_default ]
dir               = /home/hashi/ca/rootca
certs             = $dir/cert
crl_dir           = $dir/crl
new_certs_dir     = $dir/issued_certs
database          = $dir/data/index.dat
serial            = $dir/data/serial.dat
private_key       = $dir/private/rootca.key
certificate       = $dir/cert/rootca.cer
crlnumber         = $dir/data/crl_number
crl               = $dir/crl/rootca.crl
crl_extensions    = crl_ext
default_crl_days  = 30
default_md        = sha256
name_opt          = ca_default
cert_opt          = ca_default
default_days      = 3650
preserve          = no
policy            = policy_any
email_in_dn	  = no

[ policy_any ]
countryName             = supplied
stateOrProvinceName     = optional
organizationName        = match
organizationalUnitName  = optional
commonName              = supplied
emailAddress            = optional

[ req ]
default_bits        = 2048
distinguished_name  = req_distinguished_name
string_mask         = utf8only
default_md          = sha256
x509_extensions     = v3_ca

[ req_distinguished_name ]
countryName                     = Country Name (2 letter code)
stateOrProvinceName             = State or Province Name
localityName                    = Locality Name
0.organizationName              = Organization Name
organizationalUnitName          = Organizational Unit Name
commonName                      = Common Name
emailAddress                    = Email Address

countryName_default             = 
stateOrProvinceName_default     = 
localityName_default            = 
0.organizationName_default      = CyberHashira
organizationalUnitName_default  = PKI
emailAddress_default            = pki@home.lab

[ v3_ca ]
subjectKeyIdentifier = hash
authorityKeyIdentifier = keyid:always,issuer
keyUsage = critical, digitalSignature, cRLSign, keyCertSign
basicConstraints = critical, CA:true, pathlen:2
crlDistributionPoints = URI:http://ca/rootca.crl

[ v3_intermediate_ca ]
subjectKeyIdentifier = hash
authorityKeyIdentifier = keyid:always,issuer
basicConstraints = critical, CA:true, pathlen:0
keyUsage = critical, digitalSignature, cRLSign, keyCertSign
crlDistributionPoints = URI:http://ca/issuingca.crl
authorityInfoAccess = caIssuers;URI:http://ca/rootca.cer
```

#### - Generate Private Key for the RootCA.
>[!NOTE]
> You do not necessarily have to use an RSA-based key. In this demonstration, an RSA private key is used for illustrative purposes.
> You may choose to use any other supported algorithm, such as Elliptic Curve (EC) or ML-DSA, depending on your requirements.
> As a best practice, always secure your private key using encryption, strict file permissions, and appropriate key management procedures.

<pre>
hashi@ub-ch1:~/ca/rootca$ openssl genpkey -aes-128-cbc -algorithm rsa -quiet -out private/rootca.key
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:
</pre>

#### - Generate a self-signed certificate for the RootCA.
<pre>
hashi@ub-ch1:~/ca/rootca$ openssl req -config rootca.cnf -subj "/CN=rootca/C=CA/O=CyberHashira" -key private/rootca.key -new -x509 -sha256 -extensions v3_ca -days 3650 -out cert/rootca.cer 
Enter pass phrase for private/rootca.key:

hashi@ub-ch1:~/ca/rootca$ chmod 444 cert/rootca.cer
</pre>

<br>

### Step 3 : WRITE CONFIGURATION FILE FOR THE ISSUING CA, GENERATE CA KEY AND CERTIFICATE

Once the Root CA setup is complete, we will proceed to build the Issuing CA. As with the Root CA, we will begin by creating a configuration file for the Issuing CA. We will then generate a private key, create a certificate signing request (CSR), and have that request signed by the Root CA.

#### - Make a new directory for the Issuing CA and write a configuration file.
<pre>
hashi@ub-ch1:~/ca$ cd issuingca/
hashi@ub-ch1:~/ca/issuingca$ nano issuingca.cnf
</pre>

**Sample Configuration file for the ISSUINGCA (issuingca.cnf).**

```
[ ca ]
default_ca = CA_default

[ CA_default ]
dir               = /home/hashi/ca/issuingca
certs             = $dir/cert
crl_dir           = $dir/crl
new_certs_dir     = $dir/issued_certs
database          = $dir/data/index.dat
serial            = $dir/data/serial.dat
private_key       = $dir/private/issuingca.key
certificate       = $dir/cert/issuingca.cer
crlnumber         = $dir/data/crl_number
crl               = $dir/crl/issuingca.crl
crl_extensions    = crl_ext
default_crl_days  = 30
default_md        = sha256
name_opt          = ca_default
cert_opt          = ca_default
default_days      = 1826
preserve          = no
policy            = policy_any

[ policy_any ]
countryName             = optional
stateOrProvinceName     = optional
localityName            = optional
organizationName        = optional
organizationalUnitName  = optional
commonName              = supplied
emailAddress            = optional

[ req ]
default_bits        = 2048
distinguished_name  = req_distinguished_name
string_mask         = utf8only
default_md          = sha256
x509_extensions     = v3_ca

[ req_distinguished_name ]
countryName                     = Country Name (2 letter code)
stateOrProvinceName             = State or Province Name
localityName                    = Locality Name
0.organizationName              = Organization Name
organizationalUnitName          = Organizational Unit Name
commonName                      = Common Name
emailAddress                    = Email Address

countryName_default             = 
stateOrProvinceName_default     = 
localityName_default            = 
0.organizationName_default      = CyberHashira
organizationalUnitName_default  = PKI
emailAddress_default            = pki@home.lab

[ user_cert ]
basicConstraints = CA:FALSE
subjectKeyIdentifier = hash
authorityKeyIdentifier = keyid,issuer
keyUsage = critical, nonRepudiation, digitalSignature, keyEncipherment
extendedKeyUsage = clientAuth, emailProtection
crlDistributionPoints = URI:http://ca/issuingca.crl
authorityInfoAccess = caIssuers;URI:http://ca/issuingca.cer

[ server_cert ]
basicConstraints = CA:FALSE
subjectKeyIdentifier = hash
authorityKeyIdentifier = keyid,issuer:always
keyUsage = critical, digitalSignature, keyEncipherment
extendedKeyUsage = serverAuth
crlDistributionPoints = URI:http://ca/issuingca.crl
authorityInfoAccess = caIssuers;URI:http://ca/issuingca.cer

[ codeSigning_cert ]
basicConstraints = CA:FALSE
subjectKeyIdentifier = hash
authorityKeyIdentifier = keyid,issuer:always
keyUsage = critical, digitalSignature, keyEncipherment
extendedKeyUsage = codeSigning
crlDistributionPoints = URI:http://ca/issuingca.crl
authorityInfoAccess = caIssuers;URI:http://ca/issuingca.cer

[ crl_ext ]
authorityKeyIdentifier=keyid:always

[ ocsp ]
basicConstraints = CA:FALSE
subjectKeyIdentifier = hash
authorityKeyIdentifier = keyid,issuer
keyUsage = critical, digitalSignature
extendedKeyUsage = critical, OCSPSigning
```

#### - Generate Private key for the IssuingCA.
<pre>
hashi@ub-ch1:~/ca/issuingca$ openssl genpkey -aes-128-cbc -algorithm rsa -quiet -out private/issuingca.key
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:
</pre>

#### - Generate Certificate Request for the IssuingCA.
<pre>
hashi@ub-ch1:~/ca/issuingca$ openssl req -config ../rootca/rootca.cnf -subj "/CN=issuingca/C=CA/O=CyberHashira" -key private/issuingca.key -new -sha256 -out csr/issuingca.csr 
Enter pass phrase for private/issuingca.key:
</pre>

#### - Submit the certificate request to the Root CA, to get a signed certificate.
<pre>
hashi@ub-ch1:~/ca/issuingca$ openssl ca -config ../rootca/rootca.cnf -extensions v3_intermediate_ca -days 1826 -notext -md sha256 -in csr/issuingca.csr -out cert/issuingca.cer
Using configuration from ../rootca/rootca.cnf
Enter pass phrase for /home/hashi/ca/rootca/private/rootca.key:
Check that the request matches the signature
Signature ok
Certificate Details:
        Serial Number: 3724201449931423241 (0x33af057caf283e09)
        Validity
            Not Before: May 18 18:46:50 2025 GMT
            Not After : May 18 18:46:50 2030 GMT
        Subject:
            countryName               = CA
            organizationName          = CyberHashira
            commonName                = issuingca
        X509v3 extensions:
            X509v3 Subject Key Identifier: 
                2D:37:6F:0C:5B:38:67:D2:31:43:78:27:2C:5D:3B:8E:CF:A7:84:A1
            X509v3 Authority Key Identifier: 
                85:8A:83:55:81:47:2D:9C:46:6B:27:86:30:81:C1:6B:53:69:AB:4A
            X509v3 Basic Constraints: critical
                CA:TRUE, pathlen:0
            X509v3 Key Usage: critical
                Digital Signature, Certificate Sign, CRL Sign
            X509v3 CRL Distribution Points: 
                Full Name:
                  URI:http://ca/issuingca.crl

            Authority Information Access: 
                CA Issuers - URI:http://ca/rootca.cer
Certificate is to be certified until May 18 18:46:50 2030 GMT (1826 days)
Sign the certificate? [y/n]:y


1 out of 1 certificate requests certified, commit? [y/n]y
Write out database with 1 new entries
Database updated

hashi@ub-ch1:~/ca/issuingca$ chmod 444 cert/issuingca.cer
</pre>


#### - Creating a CACERT file.
<pre>
hashi@ub-ch1:~/ca/issuingca$ cat cert/issuingca.cer ../rootca/cert/rootca.cer > cert/cacerts.cer
hashi@ub-ch1:~/ca/issuingca$ chmod 444 cert/cacerts.cer
</pre>

#### - Verifying the certificate.
<pre>
hashi@ub-ch1:~/ca/issuingca$ openssl verify -CAfile ../rootca/cert/rootca.cer cert/issuingca.cer
</pre>

<br>

### - ISSUING CERTIFICATES USING ISSUING CA.

This section explains how to use the Issuing CA to sign certificate requests. I will demonstrate how to issue certificates for two use cases: a web server and code signing.

#### - Issue certificate for a webserver using the Issuing CA.

*Export an environment variable that points to the path of the Issuing CA configuration file. This is purely for convenience.*
<pre>
hashi@ub-ch1:~$ export ISSUING_CA_CONF=/home/hashi/ca/issuingca/issuingca.cnf
</pre>

*Make a new directory, generate a private key in it.*
<pre>
hashi@ub-ch1:~$ mkdir webserver
hashi@ub-ch1:~$ cd webserver/
hashi@ub-ch1:~/webserver$ openssl genpkey -algorithm rsa -quiet -out webserver.key
</pre>

*Generate a certificate signing request.*
<pre>
hashi@ub-ch1:~/webserver$ openssl req -config $ISSUING_CA_CONF -key webserver.key -new -sha256 -out webserver.csr -subj "/CN=WebServer"
</pre>

*Submit the signing request to the IssuingCA to get a CA signed certificate*
<pre>
hashi@ub-ch1:~/webserver$ openssl ca -config $ISSUING_CA_CONF -extensions server_cert -days 365 -notext -md sha256 -in webserver.csr -out webserver.cer
Using configuration from /home/hashi/ca/issuingca/issuingca.cnf
Enter pass phrase for /home/hashi/ca/issuingca/private/issuingca.key:
Check that the request matches the signature
Signature ok
Certificate Details:
        Serial Number: 4889732568920645289 (0x43dbd1dd512c5ea9)
        Validity
            Not Before: May 18 20:14:23 2025 GMT
            Not After : May 18 20:14:23 2026 GMT
        Subject:
            commonName                = WebServer
        X509v3 extensions:
            X509v3 Basic Constraints: 
                CA:FALSE
            X509v3 Subject Key Identifier: 
                B8:82:5A:63:E1:9C:97:C8:04:26:D7:10:2C:44:9B:00:1B:09:16:F3
            X509v3 Authority Key Identifier: 
                keyid:2D:37:6F:0C:5B:38:67:D2:31:43:78:27:2C:5D:3B:8E:CF:A7:84:A1
                DirName:/CN=rootca/C=CA/O=CyberHashira
                serial:33:AF:05:7C:AF:28:3E:09
            X509v3 Key Usage: critical
                Digital Signature, Key Encipherment
            X509v3 Extended Key Usage: 
                TLS Web Server Authentication
            X509v3 CRL Distribution Points: 
                Full Name:
                  URI:http://ca/issuingca.crl

            Authority Information Access: 
                CA Issuers - URI:http://ca/issuingca.cer
Certificate is to be certified until May 18 20:14:23 2026 GMT (365 days)
Sign the certificate? [y/n]:y

1 out of 1 certificate requests certified, commit? [y/n]y
Write out database with 1 new entries
Database updated
</pre>

*Verify the issued certificate using the CA cert file.*
<pre>
hashi@ub-ch1:~/webserver$ openssl verify -CAfile ../ca/issuingca/cert/cacerts.cer webserver.cer 
webserver.cer: OK
</pre>

#### - Here's how to issue a code signing certificate using the Issuing CA.

*Make a new directory, generate a private key in it.*
<pre>
hashi@ub-ch1:~$ mkdir codesigning
hashi@ub-ch1:~$ cd codesigning/
hashi@ub-ch1:~/codesigning$ openssl genpkey -algorithm ec -pkeyopt ec_paramgen_curve:secp384r1 -out codesigning.key
</pre>

*Generate a certificate signing request.*
<pre>
hashi@ub-ch1:~/codesigning$ openssl req -config $ISSUING_CA_CONF -key codesigning.key -new -sha256 -out codesigning.csr -subj "/CN=codesigning"
</pre>

*Submit the signing request to the IssuingCA to get a CA signed certificate*
<pre>
hashi@ub-ch1:~/codesigning$ openssl ca -config $ISSUING_CA_CONF -extensions codeSigning_cert -days 365 -notext -md sha256 -in codesigning.csr -out codesigning.cer
Using configuration from /home/hashi/ca/issuingca/issuingca.cnf
Enter pass phrase for /home/hashi/ca/issuingca/private/issuingca.key:
Check that the request matches the signature
Signature ok
Certificate Details:
        Serial Number: 4889732568920645290 (0x43dbd1dd512c5eaa)
        Validity
            Not Before: May 18 20:36:43 2025 GMT
            Not After : May 18 20:36:43 2026 GMT
        Subject:
            commonName                = codesigning
        X509v3 extensions:
            X509v3 Basic Constraints: 
                CA:FALSE
            X509v3 Subject Key Identifier: 
                4D:AD:94:14:37:C7:CD:22:CB:FF:E9:44:C9:3E:25:13:BE:CE:AE:D7
            X509v3 Authority Key Identifier: 
                keyid:2D:37:6F:0C:5B:38:67:D2:31:43:78:27:2C:5D:3B:8E:CF:A7:84:A1
                DirName:/CN=rootca/C=CA/O=CyberHashira
                serial:33:AF:05:7C:AF:28:3E:09
            X509v3 Key Usage: critical
                Digital Signature, Key Encipherment
            X509v3 Extended Key Usage: 
                Code Signing
            X509v3 CRL Distribution Points: 
                Full Name:
                  URI:http://ca/issuingca.crl

            Authority Information Access: 
                CA Issuers - URI:http://ca/issuingca.cer
Certificate is to be certified until May 18 20:36:43 2026 GMT (365 days)
Sign the certificate? [y/n]:y

1 out of 1 certificate requests certified, commit? [y/n]y
Write out database with 1 new entries
Database updated
</pre>

#### - Listing issued certificates.
<pre>
hashi@ub-ch1:~$ cat ca/issuingca/data/index.dat
V	260518201423Z		43DBD1DD512C5EA9	unknown	/CN=WebServer
V	260518203643Z		43DBD1DD512C5EAA	unknown	/CN=codesigning
</pre>

<br>


### REVOKING AN ISSUED CERTIFICATE

This section covers how to revoke an issued certificates.

#### - Generate a blank CRL database file.
<pre>
hashi@ub-ch1:~$ cd ca/issuingca/
hashi@ub-ch1:~/ca/issuingca$ openssl ca -config issuingca.cnf -gencrl -out crl/issuingca.crl
Using configuration from issuingca.cnf
Enter pass phrase for /home/hashi/ca/issuingca/private/issuingca.key:
</pre>

#### - Here's how to inspect a crl file.
<pre>
hashi@ub-ch1:~/ca/issuingca$ openssl crl -in crl/issuingca.crl -noout -text
Certificate Revocation List (CRL):
        Version 2 (0x1)
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: C=CA, O=CyberHashira, CN=issuingca
        Last Update: May 18 20:52:52 2025 GMT
        Next Update: Jun 17 20:52:52 2025 GMT
        CRL extensions:
            X509v3 Authority Key Identifier: 
                2D:37:6F:0C:5B:38:67:D2:31:43:78:27:2C:5D:3B:8E:CF:A7:84:A1
            X509v3 CRL Number: 
                4096
No Revoked Certificates.
    Signature Algorithm: sha256WithRSAEncryption
    Signature Value:
        25:f4:64:cc:0f:d5:7b:a1:13:ab:bb:5e:35:74:4a:b1:bf:b8:
        d9:f6:19:5d:74:f8:f4:f4:f9:2b:c9:6d:53:d4:43:1f:61:e4:
        a3:ff:1c:5e:2e:2a:71:fb:e5:11:2c:58:d4:ea:d3:ac:c1:44:
        00:f0:70:75:2d:fe:b0:bc:dd:0a:90:cc:b0:ac:36:b1:e3:8e:
        65:4f:9d:f7:01:8c:d4:b1:32:ff:ec:0f:2e:4d:bf:db:99:41:
        4c:6e:5e:f1:5c:c8:90:0c:80:dd:45:3f:35:4c:80:4d:cb:43:
        b5:6b:ee:7c:18:15:d1:f5:8e:dc:1f:d7:88:7d:a2:ec:c8:5e:
        db:0b:51:e9:5d:f0:61:7e:01:ed:ce:c5:1d:e9:f7:52:d2:b8:
        86:30:c6:f3:4a:1e:c2:1d:59:23:3f:51:ee:df:4b:7c:3f:91:
        f9:b5:c2:ea:0e:82:da:48:80:41:5c:6c:1f:f1:40:7a:5b:09:
        c7:6e:8d:b1:da:1c:14:00:c7:ed:5d:f0:40:cb:4a:05:c5:46:
        00:e8:1d:22:0d:d8:98:04:cf:7b:a2:ef:b7:c0:6e:48:62:b0:
        55:0d:6d:8d:9f:49:22:92:0f:c1:1d:d7:43:58:d4:4f:89:b3:
        2f:10:0b:47:81:44:24:58:b3:52:cb:9a:8c:8f:f5:a3:1c:82:
        27:13:0d:5e
</pre>

#### - Revoking a certificate.
<pre>
hashi@ub-ch1:~/ca/issuingca$ openssl ca -config issuingca.cnf -revoke ~/codesigning/codesigning.cer 
Using configuration from issuingca.cnf
Enter pass phrase for /home/hashi/ca/issuingca/private/issuingca.key:
Revoking Certificate 43DBD1DD512C5EAA.
Database updated
</pre>

#### - Update the crl database
<pre>
hashi@ub-ch1:~/ca/issuingca$ openssl ca -config issuingca.cnf -gencrl -out crl/issuingca.crl 
Using configuration from issuingca.cnf
Enter pass phrase for /home/hashi/ca/issuingca/private/issuingca.key:
</pre>

#### - Inspect the CRL database to check for update.
<pre>
hashi@ub-ch1:~/ca/issuingca$ openssl crl -in crl/issuingca.crl -noout -text
Certificate Revocation List (CRL):
        Version 2 (0x1)
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: C=CA, O=CyberHashira, CN=issuingca
        Last Update: May 18 20:56:25 2025 GMT
        Next Update: Jun 17 20:56:25 2025 GMT
        CRL extensions:
            X509v3 Authority Key Identifier: 
                2D:37:6F:0C:5B:38:67:D2:31:43:78:27:2C:5D:3B:8E:CF:A7:84:A1
            X509v3 CRL Number: 
                4097
Revoked Certificates:
    Serial Number: 43DBD1DD512C5EAA
        Revocation Date: May 18 20:54:41 2025 GMT
    Signature Algorithm: sha256WithRSAEncryption
    Signature Value:
        4f:0a:e6:47:6f:dc:4b:39:5d:d3:6a:d1:ec:46:c7:13:1b:0a:
        f4:3a:9e:ff:6d:9c:d9:18:fb:b2:00:25:0b:03:39:6c:05:64:
        c9:5f:a4:f5:5b:d4:9f:a6:5e:33:37:c0:3d:32:71:b2:53:76:
        ac:55:a2:9d:ec:30:bb:81:43:d9:a2:17:d8:c4:0c:b8:0a:cf:
        f0:71:44:90:9e:f3:1f:54:f8:cf:78:a6:3f:10:8e:48:7e:ca:
        18:36:58:95:10:d3:64:10:99:63:50:f5:af:44:5b:61:1d:a6:
        e5:23:8e:e1:ec:9d:e4:d6:b5:77:ef:9f:27:dd:c0:38:09:ec:
        74:70:17:78:fa:d2:a5:fb:3f:9f:54:24:00:ea:ab:62:5e:e9:
        e6:10:3f:5f:09:20:9e:44:b5:46:62:50:c3:ea:50:34:d5:da:
        f9:a8:7f:24:7c:3a:89:55:84:54:3a:8c:12:49:9d:4c:72:97:
        84:11:37:0c:52:31:19:d7:e1:ce:1f:c8:c2:d9:88:8c:74:97:
        8f:f7:87:15:3f:df:a2:0e:a0:58:ba:89:60:9b:2b:6e:b9:88:
        1d:55:42:93:57:14:b2:ea:30:30:80:74:7f:ff:5e:21:0f:2f:
        04:28:eb:f3:02:8a:b4:cd:d3:c4:af:fe:59:71:06:c5:49:d6:
        7d:a5:3c:e6
</pre>

#### - Here's what the content of index.dat would look like.
*V denotes an active (verified) certificate, whereas R denotes a revoked certificate.*

<pre>
hashi@ub-ch1:~/ca/issuingca$ cat data/index.dat
V	260518201423Z		43DBD1DD512C5EA9	unknown	/CN=WebServer
R	260518203643Z	250518205441Z	43DBD1DD512C5EAA	unknown	/CN=codesigning
</pre>

<BR>

[Back to main](README.md)
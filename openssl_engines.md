## OPENSSL ENGINES

OpenSSL engines are used to support capabilities beyond those offered by the OpenSSL library itself. For example, engines can extend functionality to include hardware security modules (HSMs). You can generate private keys within an HSM and then use those keys through OpenSSL.

In this note, I will demonstrate exactly that: using a real HSM with an OpenSSL engine, including how to configure and set up the engine. Specifically, I will use the open-source **pkcs11-engine** with Thales Cloud HSM also known as **Data Protection on Demand (DPoD)**.

<BR>

### INSTALLING ENGINE USING PACKAGE MANAGER.

*The easiest way to install openssl pkcs11-engine is by installing it through package managers such as APT, YUM or PACMAN.*

#### - Installing pkcs11-engine using APT.
<pre>
hashi@ub-ch1:~$ sudo apt install libengine-pkcs11-openssl
</pre>

#### - Once finished, you should be able to use pkcs11-engine immediately.
<pre>
hashi@ub-ch1:~$ openssl engine pkcs11 -v
(pkcs11) pkcs11 engine
     SO_PATH, MODULE_PATH, PIN, VERBOSE, QUIET, INIT_ARGS, FORCE_LOGIN, 
     RE_ENUMERATE
</pre>

<br>

### INSTALLING PKCS11-ENGINE USING SOURCE.

*If you are using a machine that is not connected to the internet, or if for any reason you prefer not to use a package manager to install pkcs11-engine, you can also build it from source.*

#### - Autoreconf package is required to build pkcs11-engine.
<pre>
hashi@rengoku:~$ sudo apt install dh-autoreconf pkgconf libssl-dev
</pre>

#### - Download the latest tarball for libp11.
<pre>
hashi@ub-ch1:~$ wget https://github.com/OpenSC/libp11/archive/refs/tags/libp11-0.4.13.tar.gz
</pre>

#### - Extract the downloaded tarball tarball.
<pre>
hashi@ub-ch1:~$ tar xf libp11-0.4.13.tar.gz
hashi@ub-ch1:~/libp11-libp11-0.4.13$
</pre>

#### - Execute bootstrap to prepare necessary files.
<pre>
hashi@ub-ch1:~/libp11-libp11-0.4.13$ ./bootstrap 
</pre>

#### - Configure Makefile, Build and install.
<pre>
hashi@ub-ch1:~/libp11-libp11-0.4.13$ PKG_CONFIG_PATH=/opt/openssl/lib64/pkgconfig ./configure --prefix=/opt/libp11
hashi@ub-ch1:~/libp11-libp11-0.4.13$ make
hashi@ub-ch1:~/libp11-libp11-0.4.13$ sudo make install
</pre>

#### - Check if OpenSSL can use pkcs11-engine.
<pre>
hashi@ub-ch1:~/libp11-libp11-0.4.13$ openssl engine pkcs11 -v
(pkcs11) pkcs11 engine
     SO_PATH, MODULE_PATH, PIN, VERBOSE, QUIET, INIT_ARGS, FORCE_LOGIN, 
     RE_ENUMERATE
</pre>

<BR>

### USING THE PKCS11-ENGINE.

#### - Install GNUTLS utilities.
*The pkcs11-engine uses PKCS#11 URLs to access keys from an HSM slot. Therefore, the p11tool utility is particularly useful for generating these URLs.*
<pre>
hashi@ub-ch1:~$ sudo apt install gnutls-bin
</pre>

#### - Read URLs using p11tool
<pre>
hashi@ub-ch1:~$ p11tool --list-all --provider /opt/dpod/libs/64/libCryptoki2.so --login
Token 'SP_SIGN' with URL 'pkcs11:model=Cryptovisor7;manufacturer=SafeNet;serial=1546793797723;token=SP_SIGN' requires user PIN
Enter PIN: 
Object 0:
	URL: pkcs11:model=Cryptovisor7;manufacturer=SafeNet;serial=1546793797723;token=SP_SIGN;object=ch-signer;type=private
	Type: Private key (RSA-2048)
	Label: ch-signer
	Flags: CKA_PRIVATE; CKA_NEVER_EXTRACTABLE; CKA_SENSITIVE; 
	ID: 

Object 1:
	URL: pkcs11:model=Cryptovisor7;manufacturer=SafeNet;serial=1546793797723;token=SP_SIGN;object=ch-signer;type=public
	Type: Public key (RSA-2048)
	Label: ch-signer
	Flags: CKA_PRIVATE; 
	ID: 
</pre>

#### - Create a configuration file for pkcs11 engine.
- dynmatic_path : Path to pkcs11 engine library.
- MODULE_PATH : Path to pkcs11 library.

<pre>
hashi@ub-ch1:~$ cat pkcs11.cnf 
openssl_conf = openssl_init

[openssl_init]
engines=engine_section

[engine_section]
pkcs11 = pkcs11_section

[pkcs11_section]
engine_id = pkcs11
dynamic_path = /opt/libp11/lib/libpkcs11.so
MODULE_PATH = /opt/dpod/libs/64/libCryptoki2.so
init = 0
</pre>

#### - SIGNING A FILE.
<pre>
hashi@ub-ch1:~$ OPENSSL_CONF=pkcs11.cnf openssl dgst -engine pkcs11 -keyform engine -sign "pkcs11:model=Cryptovisor7;manufacturer=SafeNet;serial=1546793797723;token=SP_SIGN;object=ch-signer;type=private" -sha256 -out hello.txt.sig hello.txt
Engine "pkcs11" set.
Enter PKCS#11 token PIN for SP_SIGN:
</pre>


#### - Verifying the signature.
<pre>
hashi@ub-ch1:~$ OPENSSL_CONF=pkcs11.cnf openssl dgst -engine pkcs11 -keyform engine -verify "pkcs11:model=Cryptovisor7;manufacturer=SafeNet;serial=1546793797723;token=SP_SIGN;object=ch-signer;type=public" -sha256 -signature hello.txt.sig hello.txt
Engine "pkcs11" set.
Enter PKCS#11 token PIN for SP_SIGN:
Verified OK
</pre>

#### - Generating a self-signed certificate.
<pre>
hashi@ub-ch1:~$ OPENSSL_CONF=pkcs11.cnf openssl req -x509 -engine pkcs11 -keyform engine -key "pkcs11:model=Cryptovisor7;manufacturer=SafeNet;serial=1546793797723;token=SP_SIGN;object=ch-signer;type=private" -subj '/CN=Test' -days 365 -out test.cer
Engine "pkcs11" set.
Enter PKCS#11 token PIN for SP_SIGN:
</pre>

<br>

[Back to main](README.md)
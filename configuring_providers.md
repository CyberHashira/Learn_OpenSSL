## CONFIGURING PROVIDERS

Providers are a new feature introduced in OpenSSL 3, intended to eventually replace the legacy engine framework. Like engines, OpenSSL providers offer additional features or capabilities to the user. From version 3 onwards, OpenSSL uses providers to categorise and modularise functionality—for example, separating FIPS-approved algorithms from legacy algorithms, or isolating encoders and decoders from core cryptographic operations.

<br>

### Providers in OpenSSL.

OpenSSL has implementation for these these fives providers

- **DEFAULT Provider** : his is the provider OpenSSL uses by default. It includes implementations for nearly all modern cryptographic algorithms such as RSA, AES, ECC, SHA, and others. Applications using the OpenSSL API will use this provider by default unless another is explicitly specified.

- **LEGACY Provider** : This provider contains implementations of legacy algorithms such as MD4, RC4, and DES. It is not loaded automatically and must be explicitly enabled in order to use these algorithms.

- **BASE Provider** : This provider offers functionality that is not cryptographic in nature. It includes components such as encoders and decoders for Base64 or hexadecimal conversions, as well as general-purpose mathematical operations.

- **NULL Provider** : This is a special provider that does not implement any algorithms. It must be enabled explicitly. It is typically used in scenarios where a user wishes to disable all OpenSSL-provided algorithms—such as for testing their own custom implementations.

- **FIPS provider** : This provider contains implementations of FIPS-approved algorithms—that is, cryptographic algorithms certified by NIST as secure. Examples include RSA-2048, AES, SHA-256, ML-DSA, and SLH-DSA. Like the Legacy provider, it is not enabled by default and must be manually activated.

<br>

### Enabling LEGACY provider in OpenSSL.

In this section, I will demonstrate how to enable legacy provider in OpenSSL. To enable a provider, specific entries must be added to the openssl.cnf configuration file.

#### - List all available providers.
*Default provider is always enabled by default.*
<pre>
hashi@ub-ch1:~$ openssl list -providers
Providers:
  default
    name: OpenSSL Default Provider
    version: 3.5.0
    status: active
</pre>

#### - Testing a legacy algorithm.
*If legacy provider is not enabled, using a legacy algorithm should result in an error.*
<pre>
hashi@ub-ch1:~$ openssl dgst -md4 test.txt 
Error setting digest
80BB6E5D8F770000:error:0308010C:digital envelope routines:inner_evp_generic_fetch:unsupported:crypto/evp/evp_fetch.c:375:Global default library context, Algorithm (MD4 : 99), Properties ()
80BB6E5D8F770000:error:03000086:digital envelope routines:evp_md_init_internal:initialization error:crypto/evp/digest.c:271:
</pre>

#### - Locating openssl.cnf file.
*openssl.cnf file is found in the OPENSSLDIR path*
<pre>
hashi@ub-ch1:~$ openssl version -d
OPENSSLDIR: "/opt/openssl"
</pre>

#### - Edit the openssl.cnf file to include the necessary entries for enabling a provider.
<pre>
hashi@ub-ch1:~$ sudo nano /opt/openssl/openssl.cnf
</pre>

#### - Locate [provider_sect] section.
>[!NOTE]
> Entries for enabling providers are added under the provider_sect.
> An entry for the default provider is usually present by default.
> The Default provider is implicitly activated if no other provider is specified.
> If any other provider is activated, the default provider must be explicitly enabled as well.

<pre>
[openssl_init]
providers = provider_sect

# List of providers to load
[provider_sect]
default = default_sect

[default_sect]
# activate = 1
</pre>

#### - To enable legacy provider, add legacy provider entry under provider_sect
<pre>
[provider_sect]
default = default_sect
legacy = legacy_sect
</pre>

#### - Add a new section for legacy provider to activate it, and also activate default provider.
<pre>
[provider_sect]
default = default_sect
legacy = legacy_sect

[legacy_sect]
activate=1

[default_sect]
activate = 1
</pre>

#### - Save the changes, and list providers again to verify.
<pre>
hashi@ub-ch1:~$ openssl list -providers
Providers:
  default
    name: OpenSSL Default Provider
    version: 3.5.0
    status: active
  legacy
    name: OpenSSL Legacy Provider
    version: 3.5.0
    status: active
</pre>

#### - Repeating legacy provider test.
<pre>
hashi@ub-ch1:~$ openssl dgst -md4 test.txt 
MD4(test.txt)= 36d729ab4ff7260da6fb010ef5747bb3
</pre>

<br>

### Enabling BASE and NULL providers.

To enable the BASE and NULL providers, we follow the same steps as for enabling the Legacy provider.

#### - Edit openssl.cnf file.
<pre>
hashi@ub-ch1:~$ sudo nano /opt/openssl/openssl.cnf
</pre>

#### - Add entries for BASE and NULL provider under provider_sect.
<pre>
[openssl_init]
providers = provider_sect

# List of providers to load
[provider_sect]
default = default_sect
legacy = legacy_sect
base = base_sect
null = null_sect
</pre>

#### - Add a section for BASE and NULL provider to activate them.
<pre>
[legacy_sect]
activate=1

[base_sect]
activate=1

[null_sect]
activate=1

[default_sect]
activate=1
</pre>

#### - Save the changes, and list providers again.
<pre>
hashi@ub-ch1:~$ openssl list -providers
Providers:
  base
    name: OpenSSL Base Provider
    version: 3.5.0
    status: active
  default
    name: OpenSSL Default Provider
    version: 3.5.0
    status: active
  legacy
    name: OpenSSL Legacy Provider
    version: 3.5.0
    status: active
  null
    name: OpenSSL Null Provider
    version: 3.5.0
    status: active
</pre>

<br>

### Configuring FIPS Provider 

>[!NOTE]
> FIPS provider requires that the fips modules is installed.
> To enable the FIPS provider, OpenSSL must be built with the enable-fips configuration option.
> The FIPS module is typically a shared object named fips.so, which must be located in the ossl-modules directory.
> The fipsmodule configuration file include Known Answer Test (KAT) values.
> KAT values are required to initialise FIPS module securely.

#### - Location of fipsmodule.cnf.
<pre>
hashi@ub-ch1:~$ openssl version -d
OPENSSLDIR: "/opt/openssl"
hashi@ub-ch1:~$ ls -l /opt/openssl/fipsmodule.cnf 
-rw-r--r-- 1 root root 827 Apr 28 14:20 /opt/openssl/fipsmodule.cnf
</pre>

#### - fipsmodule file can be generated/regenerated as follows:
<pre>
hashi@ub-ch1:~$ openssl fipsinstall -out fipsmodule.cnf -module /opt/openssl/lib64/ossl-modules/fips.so -pedantic
HMAC : (Module_Integrity) : Pass
SHA2 : (KAT_Digest) : Pass
SHA3 : (KAT_Digest) : Pass
AES_GCM : (KAT_Cipher) : Pass
ML-DSA : (KAT_Signature) : Pass
SLH-DSA : (KAT_Signature) : Pass
X963KDF : (KAT_KDF) : Pass
X942KDF : (KAT_KDF) : 
ML-KEM : (KAT_AsymmetricKeyGeneration) : Pass
ML-DSA : (KAT_AsymmetricKeyGeneration) : Pass
SLH-DSA : (KAT_AsymmetricKeyGeneration) : Pass
KEM_Decap : (KAT_KEM) : Pass
KEM_Decap_Reject : (KAT_KEM) : Pass
	name:     	OpenSSL FIPS Provider
	version:  	3.5.0
	build:    	3.5.0
INSTALL PASSED

hashi@ub-ch1:~$ sudo cp fipsmodule.cnf /opt/openssl/
[sudo] password for hashi:
</pre>

#### - Verify fips module as follows:
<pre>
hashi@ub-ch1:~$ openssl fipsinstall -in /opt/openssl/fipsmodule.cnf -module /opt/openssl/lib64/ossl-modules/fips.so -verify
VERIFY PASSED
</pre>

<br>

### Enabling FIPS Provider.

#### - Open openssl.cnf file for editing.
<pre>
hashi@ub-ch1:~$ openssl version -d
OPENSSLDIR: "/opt/openssl"

hashi@ub-ch1:~$ sudo nano /opt/openssl/openssl.cnf
</pre>


#### - Uncomment '.include fipsmodule.cnf' and type the complete path to fipsmodule.cnf
<pre>
.include /opt/openssl/fipsmodule.cnf
</pre>

#### - Add entry for FIPS provider.
<pre>
[provider_sect]
default = default_sect
legacy = legacy_sect
base = base_sect
null = null_sect
fips = fips_sect
</pre>

#### - Save the changes, and list providers to verify.
<pre>
hashi@ub-ch1:~$ openssl list -providers
Providers:
  base
    name: OpenSSL Base Provider
    version: 3.5.0
    status: active
  default
    name: OpenSSL Default Provider
    version: 3.5.0
    status: active
  fips
    name: OpenSSL FIPS Provider
    version: 3.5.0
    status: active
  legacy
    name: OpenSSL Legacy Provider
    version: 3.5.0
    status: active
  null
    name: OpenSSL Null Provider
    version: 3.5.0
    status: active
</pre>

<br>

### CONFIGURE OPENSSL TO STRICTLY USE ONLY FIPS MODULE.

In this section, I will demonstrate how to configure OpenSSL to strictly use only the FIPS provider for all cryptographic operations. The FIPS module permits only those algorithms and operations that are approved as secure by NIST.

#### - Execute operations that are disallowed in FIPS.
*default provider allows these operations*
<pre>
hashi@ub-ch1:~$ openssl genrsa -out private.key 1024

hashi@ub-ch1:~$ openssl dgst -md5 test.txt 
MD5(test.txt)= d8e8fca2dc0f896fd7cb4cb0031ba249
</pre>

#### - Open openssl.cnf file for editing.
<pre>
hashi@ub-ch1:~$ sudo nano /opt/openssl/openssl.cnf
</pre>

#### - Make changes to provider sect as follows:
<pre>
# List of providers to load
[provider_sect]
base = base_sect
fips = fips_sect

[base_sect]
activate=1
</pre>

#### - Save the changes and list providers.
<pre>
hashi@ub-ch1:~$ openssl list -providers
Providers:
  base
    name: OpenSSL Base Provider
    version: 3.5.0
    status: active
  fips
    name: OpenSSL FIPS Provider
    version: 3.5.0
    status: active
</pre>

#### - Execute operations that are disallowed in FIPS.
*FIPS module disallows these operations.*
<pre>
hashi@ub-ch1:~$ openssl dgst -md5 test.txt 
Error setting digest
80DB4FF3627D0000:error:0308010C:digital envelope routines:inner_evp_generic_fetch:unsupported:crypto/evp/evp_fetch.c:375:Global default library context, Algorithm (MD5 : 100), Properties ()
80DB4FF3627D0000:error:03000086:digital envelope routines:evp_md_init_internal:initialization error:crypto/evp/digest.c:271:

hashi@ub-ch1:~$ openssl genrsa -out private.key 1024
genrsa: Error generating RSA key
800B227D797B0000:error:020000AE:rsa routines:ossl_rsa_sp800_56b_validate_strength:invalid modulus:crypto/rsa/rsa_sp800_56b_gen.c:180:
</pre>

#### - Operations allowed in FIPS mode will execute as expected.
<pre>
hashi@ub-ch1:~$ openssl genrsa -out private.key 2048

hashi@ub-ch1:~$ openssl dgst -sha256 test.txt 
SHA2-256(test.txt)= f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2
</pre>

<br>

[Back to main](README.md)

## COMPUTE MESSAGE AUTHENTICATION CODE (MAC)

In this section of my notes, I will demonstrate how to compute HMAC and CMAC using OpenSSL.

#### - A conceptual demonstration of MAC.
<pre>
hashi@ub-ch1:~$ echo -n "Earth is the third planet of our Solar System." > earth.txt
hashi@ub-ch1:~$ openssl rand -hex -out key 16
hashi@ub-ch1:~$ cat earth.txt key | openssl sha256
SHA2-256(stdin)= 057b396af54a19e891edd26ba95134adeb9e43e190148781e8447fe96e0fdfe1
</pre>

### Computing HMAC (Hash based Message Authentication Code)

#### - Generating a secret key for HMAC.
<pre>
hashi@ub-ch1:~$ openssl rand -hex -out secret.key 16

hashi@ub-ch1:~$ cat secret.key 
26fb35bb2f7fa34a0ffc37552a1edc63
</pre>

#### - Computing SHA1_HMAC
<pre>
hashi@ub-ch1:~$ openssl sha1 -hmac `cat secret.key` -out earth.txt.sha1hmac earth.txt

hashi@ub-ch1:~$ cat earth.txt.sha1hmac 
HMAC-SHA1(earth.txt)= e513a64360d8b6c36451f2f7b6ea0d7c26fe102b
</pre>

#### - Computing SHA256_HMAC
<pre>
hashi@ub-ch1:~$ openssl sha256 -hmac `cat secret.key` -out earth.txt.sha256hmac earth.txt

hashi@ub-ch1:~$ cat earth.txt.sha256hmac 
HMAC-SHA2-256(earth.txt)= 5081905137f771cf48d615bbbd56178860b64566267725130ef52323e24c43bf
</pre>

#### - Generating SHA1_HMAC using 'openssl dgst'
<pre>
hashi@ub-ch1:~$ openssl dgst -sha1 -hmac `cat secret.key` -out earth.txt.sha1hmac earth.txt

hashi@ub-ch1:~$ cat earth.txt.sha1hmac 
HMAC-SHA1(earth.txt)= e513a64360d8b6c36451f2f7b6ea0d7c26fe102b
</pre>

#### - Generating SHA256_HMAC using 'openssl dgst'
<pre>
hashi@ub-ch1:~$ openssl dgst -sha256 -hmac `cat secret.key` -out earth.txt.sha256hmac earth.txt

hashi@ub-ch1:~$ cat earth.txt.sha256hmac 
HMAC-SHA2-256(earth.txt)= 5081905137f771cf48d615bbbd56178860b64566267725130ef52323e24c43bf
</pre>

> [!NOTE]  
> So far, I have used a secret key in string format to compute a HMAC.
> Now I will demonstrate how to compute an HMAC using a binary secret key.

#### - Generating 32 bytes of binary secret key.
<pre>
hashi@ub-ch1:~$ openssl rand -out secret.bin 32
</pre>

#### - Computing HMAC using a binary secret key.
<pre>
hashi@ub-ch1:~$ openssl dgst -mac hmac -sha256 -macopt hexkey:`xxd -c 32 -p secret.bin` -out earth.txt.sha256hmac earth.txt

hashi@ub-ch1:~$ cat earth.txt.sha256hmac 
HMAC-SHA2-256(earth.txt)= 1bb334c4d871089c204da6a7afd390595929b28dc2673de5f5f1bbc8a16e154a
</pre>

<br>

### COMPUTING CMAC

#### - Computing aes-cmac.
<pre>
hashi@ub-ch1:~$ openssl dgst -mac cmac -macopt cipher:aes-256-cbc -macopt hexkey:`xxd -c 32 -p secret.bin` -out earth.txt.cmacaes earth.txt

hashi@ub-ch1:~$ cat earth.txt.cmacaes 
CMAC(earth.txt)= ee257ff49e76d1c7acf5c13634366cba
</pre>

#### - Computing des3-cmac
<pre>
hashi@ub-ch1:~$ openssl rand -out des3.bin 24

hashi@ub-ch1:~$ openssl dgst -mac cmac -macopt cipher:des-ede3-cbc -macopt hexkey:`xxd -p des3.bin` -out earth.txt.cmacdes3 earth.txt

hashi@ub-ch1:~$ cat earth.txt.cmacdes3 
CMAC(earth.txt)= 554783d1a42953f9
</pre>

<br>

### COMPUTING HMAC, CMAC and GMAC.
*OpenSSL 3.x introduces a new command specifically for computing MACs. Similar to openssl pkeyutl, which supports all public key operations within a single command, the openssl mac command supports HMAC, CMAC, and GMAC. In this section, I'll demonstrate each of these.*

#### - Computing HMAC.
<pre>
hashi@ub-ch1:~$ openssl rand -out hmac.key 16

hashi@ub-ch1:~$ openssl mac -digest sha256 -macopt hexkey:$(xxd -c 16 -p hmac.key) -in earth.txt -out earth.txt.hmacsha256 hmac
</pre>

#### - Computing CMAC
<pre>
hashi@ub-ch1:~$ openssl mac -cipher aes-128-cbc -macopt hexkey:$(xxd -c 16 -p aes.key) -in earth.txt -out earth.txt.cmacaes cmac

hashi@ub-ch1:~$ cat earth.txt.cmacaes 
811C66D01A1DF7C20F1570F4843B15DC
</pre>

#### - Computing GMAC
<pre>
hashi@ub-ch1:~$ openssl rand -out iv.bin 12

hashi@ub-ch1:~$ openssl mac -cipher aes-128-gcm -macopt hexkey:$(xxd -p -c 16 aes.key) -macopt hexiv:$(xxd -p -c 12 iv.bin) -in earth.txt -out earth.txt.gmac GMAC

hashi@ub-ch1:~$ cat earth.txt.gmac 
C5545287B4C5D1EEC4B2E1AE5E448C51
</pre>

<br>

[Back to main page](README.md)
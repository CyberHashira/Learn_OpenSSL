## BUILDING OPENSSL FROM SOURCE

In this section, I will demonstrate how to build OpenSSL from source on both Windows and Linux operating systems. It is worth noting that most Unix/Linux distributions come with OpenSSL preinstalled as part of the system dependencies. Therefore, building or installing OpenSSL from source is generally unnecessary unless you have a specific requirement such as enabling custom features, using a newer version than the system default, or performing a test.

<br>

### Building OpenSSL from source on Linux

The steps provided below should work with any Linux distribution, provided that the prerequisites mentioned are met. So far, these steps have consistently worked for me on all well-known distributions such as Ubuntu, Arch, Rocky, Alma, RHEL, and Debian. For this document, I'll be using Ubuntu Server 24.04.

#### - Prerequisites:
- GCC compiler.
- Perl
- Make

#### - Step 1: Download OpenSSL's source tar ball from www.openssl.org
<pre>
hashi@ub-ch1:~$ wget https://github.com/openssl/openssl/releases/download/openssl-3.5.0/openssl-3.5.0.tar.gz
</pre>

#### - Step 2: Extract the contents of tarball.
<pre>
hashi@ub-ch1:~$ tar xf openssl-3.5.0.tar.gz 
hashi@ub-ch1:~$ cd openssl-3.5.0
</pre>

#### - Step 3: Configure the Makefile with options as described below
- --prefix: Location where the OpenSSL binaries will be installed.
- --openssldir: Location of the openssl.cnf file, private keys and cert directory.
- -static : build static binaries.
- enable-fips: Enables fips provider support.

<pre>
hashi@ub-ch1:~/openssl-3.5.0$ ./config --prefix=/opt/openssl --openssldir=/opt/openssl enable-fips -static
Configuring OpenSSL version 3.5.0 for target linux-x86_64
Using os-specific seed configuration
Created configdata.pm
Running configdata.pm
Created Makefile.in
Created Makefile
Created include/openssl/configuration.h
</pre>

#### - Step 4) Execute "make" to start the build process. Optionally execute "make -j" to enable parallel build (faster).
*Depending on the power of your system, this process may take between 1 and 5 minutes to complete.*

<pre>
hashi@ub-ch1:~/openssl-3.5.0$ make
</pre>

#### - Step 5) Install binaries. The options are as described below:
- **install** : install everything (runtime + documentation + development files + engines + modules + ssl directories).
- **install_sw** : installs runtime, development files, engines and modules.
- **install_docs** : installs documentation.
- **install_ssldirs** : install ssl directories.
- **install_engines** : installs engines.
- **install_fips** : installs fips provider.
- **install_programs** : installs runtime and library.

<pre>
hashi@ub-ch1:~/openssl-3.5.0$ sudo make install_sw && sudo make install_fips && sudo make install_ssldirs
hashi@ub-ch1:~/openssl-3.5.0$ tree /opt/openssl/ -d
/opt/openssl/
├── bin
├── certs
├── include
│   └── openssl
├── lib64
│   ├── cmake
│   │   └── OpenSSL
│   ├── engines-3
│   ├── ossl-modules
│   └── pkgconfig
├── misc
└── private
</pre>

#### - Set LD_LIBRARY_PATH environment variable that points to the directory containing OpenSSL library.
*There's no need to export LD_LIBRARY_PATH if you choose to build OpenSSL statically.*

<pre>
hashi@ub-ch1:~/openssl-3.5.0$ export LD_LIBRARY_PATH=/opt/openssl/lib64/

hashi@ub-ch1:~/openssl-3.5.0$ /opt/openssl/bin/openssl version
OpenSSL 3.5.0 8 Apr 2025 (Library: OpenSSL 3.5.0 8 Apr 2025)
</pre>

<br>

### Building OpenSSL from Source on Windows

Unlike Unix/Linux, Windows does not include OpenSSL as a preinstalled programme. The easiest way to obtain OpenSSL for Windows is to download the installer from the link below. They usually maintain an up-to-date installer for OpenSSL.
URL: https://slproweb.com/products/Win32OpenSSL.html

However, if you are someone who prefers building OpenSSL from source rather than downloading it from elsewhere, here is an easy-to-follow method.

#### - Step 1) Download and install msys2.
*I find MSYS2 the easiest to use, as unlike other programmes such as Visual Studio, it is lightweight, quick to download and install, and the steps are similar to those described for Unix/Linux-based installation.*

URL : https://www.msys2.org/

#### - Step 2) Launch "MSYS2 MING64" from the Start Menu.

#### - Step 3) Install the following prerequisites using this command.
<pre>
hashi@CyberHashira MINGW64 ~
$ pacman -S mingw-w64-x86_64-gcc make
</pre>

![Screenshot from 2025-04-28 15-39-29](https://github.com/user-attachments/assets/1aa26160-99e4-4aa8-994f-7f5524e81349)

#### - Step 4) Download the tarball for OpenSSL.
<pre>
hashi@CyberHashira MINGW64 ~
$ wget https://github.com/openssl/openssl/releases/download/openssl-3.5.0/openssl-3.5.0.tar.gz
</pre>

#### - Step 5) Extract the contents of the tarball.
<pre>
hashi@CyberHashira MINGW64 ~
$ tar xf openssl-3.5.0.tar.gz

hashi@CyberHashira MINGW64 ~
$ cd openssl-3.5.0/
</pre>

#### - Step 6) Configure the makefile.
<pre>
hashi@CyberHashira MINGW64 ~/openssl-3.5.0
$ ./config mingw64 --prefix=C:/OpenSSL --openssldir=C:/OpenSSL enable-fips
</pre>

#### - Step 7) Execute make command to start the build process.
<pre>
hashi@CyberHashira MINGW64 ~/openssl-3.5.0
$ make
</pre>

#### - Step 8) Install the binaries and exit msys2.
<pre>
hashi@CyberHashira MINGW64 ~/openssl-3.5.0
$ make install_sw && make install_ssldirs && make install_fips
$ exit
</pre>

#### - Now you can open a command prompt and simply execute the openssl program.
![Screenshot from 2025-04-28 16-04-47](https://github.com/user-attachments/assets/8e7bbbfe-996b-4944-8465-8eb85bba3512)

[TIP!]
> You can archive the entire OpenSSL directory using tar or zip for use on other computers.
> Set the location of openssl.exe in the system's PATH environment variable for easy access.

<br>

[Back to Main Page](README.md)

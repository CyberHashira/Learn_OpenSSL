CC=gcc
INCLUDES=/opt/openssl/include
LIBS=/opt/openssl/lib64
FLAGS=-lcrypto
OUTDIR=bin
$(shell mkdir -p bin)

# This is the default make option.
default: all

all: using_keys hashing encryption signing misc

using_keys: generate_rsa_keypair_pem generate_rsa_keypair_der generate_ecdsa_keypair_pem generate_ecdsa_keypair_der \
generate_mldsa_keypair_der generate_mldsa_keypair_pem generate_eddsa_keypair_pem load_private_key
	@echo "  - using_keys samples build successfully. Executables are inside bin/using_keys."

hashing: evp_sha1 evp_sha256 evp_sha3_256 evp_shake256
	@echo "  - hashing samples build successfully. Executables are inside bin/hashing."

encryption: encrypting_using_rsa_pkcs1 encrypting_using_rsa_oaep encrypting_using_rsa_oaep2 encrypting_using_aes_cbc
	@echo "  - encryption samples build successfully. Executables are inside bin/encryption."

signing: sign_using_sha256_digest sign_using_sha512_digest verify_sha256_digest_signature \
sign_using_mldsa verify_mldsa_signature sign_using_rsa_pss
	@echo "  - signing samples build successfully. Executables are inside bin/signing."

misc: generate_random_data
	@echo "  - misc samples build successfully. Executables are inside bin/misc."



# SAMPLES FROM USING_KEYS DIRECTORY.
generate_rsa_keypair_pem: using_keys/generate_rsa_keypair_pem.c
	@mkdir -p bin/using_keys
	@$(CC) using_keys/generate_rsa_keypair_pem.c -o bin/using_keys/generate_rsa_keypair_pem -I$(INCLUDES) -L$(LIBS) $(FLAGS)

generate_rsa_keypair_der: using_keys/generate_rsa_keypair_der.c
	@mkdir -p bin/using_keys
	@$(CC) using_keys/generate_rsa_keypair_der.c -o bin/using_keys/generate_rsa_keypair_der -I$(INCLUDES) -L$(LIBS) $(FLAGS)

generate_ecdsa_keypair_pem: using_keys/generate_ecdsa_keypair_pem.c
	@mkdir -p bin/using_keys
	@$(CC) using_keys/generate_ecdsa_keypair_pem.c -o bin/using_keys/generate_ecdsa_keypair_pem -I$(INCLUDES) -L$(LIBS) $(FLAGS)

generate_ecdsa_keypair_der: using_keys/generate_ecdsa_keypair_der.c
	@mkdir -p bin/using_keys
	@$(CC) using_keys/generate_ecdsa_keypair_der.c -o bin/using_keys/generate_ecdsa_keypair_der -I$(INCLUDES) -L$(LIBS) $(FLAGS)

generate_eddsa_keypair_pem: using_keys/generate_eddsa_keypair_pem.c
	@mkdir -p bin/using_keys
	@$(CC) using_keys/generate_eddsa_keypair_pem.c -o bin/using_keys/generate_eddsa_keypair_pem -I$(INCLUDES) -L$(LIBS) $(FLAGS)

generate_mldsa_keypair_der: using_keys/generate_mldsa_keypair_der.c
	@mkdir -p bin/using_keys
	@$(CC) using_keys/generate_mldsa_keypair_der.c -o bin/using_keys/generate_mldsa_keypair_der -I$(INCLUDES) -L$(LIBS) $(FLAGS)

generate_mldsa_keypair_pem: using_keys/generate_mldsa_keypair_pem.c
	@mkdir -p bin/using_keys
	@$(CC) using_keys/generate_mldsa_keypair_pem.c -o bin/using_keys/generate_mldsa_keypair_pem -I$(INCLUDES) -L$(LIBS) $(FLAGS)

load_private_key: using_keys/load_private_key.c
	@mkdir -p bin/using_keys
	@$(CC) using_keys/load_private_key.c -o bin/using_keys/load_private_key -I$(INCLUDES) -L$(LIBS) $(FLAGS)




# SAMPLES FROM HASHING DIRECTORY.
evp_sha1: hashing/evp_sha1.c
	@mkdir -p bin/hashing
	@$(CC) hashing/evp_sha1.c -o bin/hashing/evp_sha1 -I$(INCLUDES) -L$(LIBS) $(FLAGS)

evp_sha256: hashing/evp_sha256.c
	@mkdir -p bin/hashing
	@$(CC) hashing/evp_sha256.c -o bin/hashing/evp_sha256 -I$(INCLUDES) -L$(LIBS) $(FLAGS)

evp_sha3_256: hashing/evp_sha3_256.c
	@mkdir -p bin/hashing
	@$(CC) hashing/evp_sha3_256.c -o bin/hashing/evp_sha3_256 -I$(INCLUDES) -L$(LIBS) $(FLAGS)

evp_shake256: hashing/evp_shake256.c
	@mkdir -p bin/hashing
	@$(CC) hashing/evp_shake256.c -o bin/hashing/evp_shake256 -I$(INCLUDES) -L$(LIBS) $(FLAGS)




# SAMPLES FROM SIGNING DIRECTORY.
sign_using_sha256_digest: signing/sign_using_sha256_digest.c
	@mkdir -p bin/signing
	@$(CC) signing/sign_using_sha256_digest.c -o bin/signing/sign_using_sha256_digest -I$(INCLUDES) -L$(LIBS) $(FLAGS)

sign_using_sha512_digest: signing/sign_using_sha512_digest.c
	@mkdir -p bin/signing
	@$(CC) signing/sign_using_sha512_digest.c -o bin/signing/sign_using_sha512_digest -I$(INCLUDES) -L$(LIBS) $(FLAGS)

verify_sha256_digest_signature: signing/verify_sha256_digest_signature.c
	@mkdir -p bin/signing
	@$(CC) signing/verify_sha256_digest_signature.c -o bin/signing/verify_sha256_digest_signature -I$(INCLUDES) -L$(LIBS) $(FLAGS)

sign_using_mldsa: signing/sign_using_mldsa.c
	@mkdir -p bin/signing
	@$(CC) signing/sign_using_mldsa.c -o bin/signing/sign_using_mldsa -I$(INCLUDES) -L$(LIBS) $(FLAGS)

verify_mldsa_signature: signing/verify_mldsa_signature.c
	@mkdir -p bin/signing
	@$(CC) signing/verify_mldsa_signature.c -o bin/signing/verify_mldsa_signature -I$(INCLUDES) -L$(LIBS) $(FLAGS)

sign_using_rsa_pss: signing/sign_using_rsa_pss.c
	@mkdir -p bin/signing
	@$(CC) signing/sign_using_rsa_pss.c -o bin/signing/sign_using_rsa_pss -I$(INCLUDES) -L$(LIBS) $(FLAGS)



# SAMPLES FROM ENCRYPTION DIRECTORY.
encrypting_using_rsa_pkcs1: encryption/encrypting_using_rsa_pkcs1.c
	@mkdir -p bin/encryption
	@$(CC) encryption/encrypting_using_rsa_pkcs1.c -o bin/encryption/encrypting_using_rsa_pkcs1 -I$(INCLUDES) -L$(LIBS) $(FLAGS)

encrypting_using_rsa_oaep: encryption/encrypting_using_rsa_oaep.c
	@mkdir -p bin/encryption
	@$(CC) encryption/encrypting_using_rsa_oaep.c -o bin/encryption/encrypting_using_rsa_oaep -I$(INCLUDES) -L$(LIBS) $(FLAGS)

encrypting_using_rsa_oaep2: encryption/encrypting_using_rsa_oaep2.c
	@mkdir -p bin/encryption
	@$(CC) encryption/encrypting_using_rsa_oaep2.c -o bin/encryption/encrypting_using_rsa_oaep2 -I$(INCLUDES) -L$(LIBS) $(FLAGS)

encrypting_using_aes_cbc: encryption/encrypting_using_aes_cbc.c
	@mkdir -p bin/encryption
	@$(CC) encryption/encrypting_using_aes_cbc.c -o bin/encryption/encrypting_using_aes_cbc -I$(INCLUDES) -L$(LIBS) $(FLAGS)



# MISCELLANEOUS SAMPLES.
generate_random_data: misc/generate_random_data.c
	@mkdir -p bin/misc
	@$(CC) misc/generate_random_data.c -o bin/misc/generate_random_data -I$(INCLUDES) -L$(LIBS) $(FLAGS)



# DELETES BINARIES.
clean:
	@rm -rf bin
	@echo "All executables removed."
	@echo


# DISPLAYS HELP
help:
	@echo "make            : Builds all sample code."
	@echo "make all        : Builds all sample code."
	@echo "make using_keys : Builds all samples demonstrating key generation."
	@echo "make hashing    : Builds all message digest samples."
	@echo "make encryption : Builds all encryption related samples."
	@echo "make signing    : Builds all digital signing samples."
	@echo "make misc       : Builds all micellaneous samples."
	@echo "make clean      : Deletes all sample binaries."
	@echo
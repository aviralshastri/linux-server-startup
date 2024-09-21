#ifndef STRING_ENCRYPTION_H
#define STRING_ENCRYPTION_H

#include <mbedtls/aes.h>
#include <Arduino.h>

class StringEncryption {
private:
    uint8_t key[16];
    uint8_t iv[16];

public:
    StringEncryption(const char* keyStr);
    String encrypt(const String &plaintext);
    String decrypt(const String &ciphertext);
};

#endif

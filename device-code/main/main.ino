#include <mbedtls/aes.h>
#include <Arduino.h>

class StringEncryption {
private:
    uint8_t key[16];
    uint8_t iv[16];

public:
    StringEncryption(const char* keyStr) {
        memcpy(key, keyStr, 16);
        memset(iv, 0, 16);  // Using a zero IV for simplicity. In practice, use a random IV.
    }

    String encrypt(const String &plaintext) {
        mbedtls_aes_context aes;
        mbedtls_aes_init(&aes);
        mbedtls_aes_setkey_enc(&aes, key, 128);

        size_t paddedLen = ((plaintext.length() + 15) / 16) * 16;
        uint8_t padded[paddedLen];
        memset(padded, 0, paddedLen);
        memcpy(padded, plaintext.c_str(), plaintext.length());

        uint8_t encrypted[paddedLen];
        uint8_t iv_copy[16];
        memcpy(iv_copy, iv, 16);
        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, paddedLen, iv_copy, padded, encrypted);

        String result = "";
        for (size_t i = 0; i < paddedLen; i++) {
            if (encrypted[i] < 0x10) result += "0";
            result += String(encrypted[i], HEX);
        }

        mbedtls_aes_free(&aes);
        return result;
    }

    String decrypt(const String &ciphertext) {
        mbedtls_aes_context aes;
        mbedtls_aes_init(&aes);
        mbedtls_aes_setkey_dec(&aes, key, 128);

        size_t cipherLen = ciphertext.length() / 2;
        uint8_t cipher[cipherLen];
        for (size_t i = 0; i < cipherLen; i++) {
            cipher[i] = (uint8_t) strtol(ciphertext.substring(i*2, i*2+2).c_str(), NULL, 16);
        }

        uint8_t decrypted[cipherLen];
        uint8_t iv_copy[16];
        memcpy(iv_copy, iv, 16);
        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, cipherLen, iv_copy, cipher, decrypted);

        mbedtls_aes_free(&aes);
        return String((char*)decrypted);
    }
};

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    const char* key = "0123456789abcdef";  // 16-byte key
    StringEncryption crypto(key);

    String messages[] = {
        "Hello, World!",
        "Encryption is fun",
        "ESP32 is awesome",
        "AES-128 in action",
        "Secure communication"
    };

    for (int i = 0; i < 5; i++) {
        delay(1000);
        String original = messages[i];
        String encrypted = crypto.encrypt(original);
        String decrypted = crypto.decrypt(encrypted);

        Serial.println("Example " + String(i + 1) + ":");
        Serial.println("Original: " + original);
        Serial.println("Encrypted: " + encrypted);
        Serial.println("Decrypted: " + decrypted);
        Serial.println(); 
    }
}

void loop() {}
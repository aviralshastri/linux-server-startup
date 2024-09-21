#include "AES_ENCRYPTOR.h"

// Constructor
AES_ENCRYPTOR::AES_ENCRYPTOR() {
    aesLib.gen_iv(aes_iv); // Generates the IV
}

// Function to encrypt a string
String AES_ENCRYPTOR::encryptString(String plainText) {
    char encrypted[512]; // Buffer for the encrypted text
    int plainTextLength = plainText.length();
    
    // AES-128: pass 128 for the bit size
    int cipherLength = aesLib.encrypt64(
        (byte*)plainText.c_str(), plainTextLength, encrypted, aes_key, 128, aes_iv
    );

    if (cipherLength == -1) {
        return String("Encryption Error");
    }
    return String(encrypted);
}

// Function to decrypt an encrypted string
String AES_ENCRYPTOR::decryptString(String encryptedText) {
    char decrypted[512]; // Buffer for the decrypted text
    int encryptedLength = encryptedText.length();

    // AES-128: pass 128 for the bit size
    int decryptLength = aesLib.decrypt64(
        (char*)encryptedText.c_str(), encryptedLength, (byte*)decrypted, aes_key, 128, aes_iv
    );

    if (decryptLength == -1) {
        return String("Decryption Error");
    }
    return String(decrypted);
}

from Crypto.Cipher import AES
import binascii
import os

class AESStringEncryption:
    def __init__(self, key=None):
        self.__key = None
        self.__iv = b'\x00' * 16  # Using a zero IV for simplicity. In practice, use a random IV.
        if key is not None:
            self.set_key(key)

    def set_key(self, key):
        if isinstance(key, str):
            key = key.encode('utf-8')
        self.__key = key[:16].ljust(16, b'\0')

    def encrypt(self, plaintext):
        if self.__key is None:
            raise ValueError("Encryption key has not been set. Use set_key() method to set a key.")
        cipher = AES.new(self.__key, AES.MODE_CBC, self.__iv)
        padded_data = self._pad(plaintext.encode('utf-8'))
        ciphertext = cipher.encrypt(padded_data)
        return binascii.hexlify(ciphertext).decode('utf-8')

    def decrypt(self, ciphertext):
        if self.__key is None:
            raise ValueError("Decryption key has not been set. Use set_key() method to set a key.")
        cipher = AES.new(self.__key, AES.MODE_CBC, self.__iv)
        ciphertext_bytes = binascii.unhexlify(ciphertext)
        padded_data = cipher.decrypt(ciphertext_bytes)
        return self._unpad(padded_data).decode('utf-8')

    def _pad(self, data):
        padding_length = AES.block_size - len(data) % AES.block_size
        return data + bytes([padding_length] * padding_length)

    def _unpad(self, padded_data):
        padding_length = padded_data[-1]
        if padding_length == 0 or padding_length > AES.block_size:
            return padded_data.rstrip(b'\x00')
        return padded_data[:-padding_length]


if __name__ == "__main__":
    cipher = AESStringEncryption("0123456789abcdef")
    
    original_text = "Hello, World!"
    encrypted_string = cipher.encrypt(original_text)
    print("Encrypted text:", encrypted_string)

    try:
        decrypted_text = cipher.decrypt("c656b39bb5d74f1ac714a8a2a3d97a6e64c7a6b5bedcfac17f5b7ed3acd99c10")
        print("Decrypted text:", decrypted_text)
    except Exception as e:
        print("Error during decryption:", str(e))

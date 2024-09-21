from Crypto.Cipher import AES
import binascii

def decrypt_string(encrypted_string, key):
    # Convert the hex string to bytes
    ciphertext = binascii.unhexlify(encrypted_string)
    
    # Create a new AES cipher object
    cipher = AES.new(key.encode('utf-8'), AES.MODE_CBC, iv=b'\x00' * 16)
    
    # Decrypt the ciphertext
    decrypted = cipher.decrypt(ciphertext)
    
    return decrypted

# The key used for encryption (must be the same as in Arduino)
key = "0123456789abcdef"

# The encrypted string to decrypt
encrypted = "71dc1ddf22ff896ce45bd46022ddecf5c142851c17fd242de87c01dd28f92020"

try:
    decrypted_bytes = decrypt_string(encrypted, key)
    
    print("Decrypted bytes:")
    print(decrypted_bytes)
    
    print("\nDecrypted as UTF-8 string:")
    print(decrypted_bytes.decode('utf-8', errors='replace'))
    
    print("\nDecrypted with non-printable characters escaped:")
    print(repr(decrypted_bytes.decode('utf-8', errors='replace')))
    
    print("\nDecrypted bytes as hex:")
    print(binascii.hexlify(decrypted_bytes).decode('ascii'))
    
    print("\nDecrypted string after removing null bytes:")
    print(decrypted_bytes.decode('utf-8', errors='replace').rstrip('\x00'))
    
except Exception as e:
    print(f"Decryption failed: {str(e)}")

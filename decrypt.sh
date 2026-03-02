#!/bin/bash

# AES Key and IV (from your STM32 main.c)
KEY="2b7e151628aed2a6abf7158809cf4f3c"
IV="000102030405060708090a0b0c0d0e0f"

# Encrypted data (replace with the actual hex you received on USART)
ENCRYPTED_DATA="$1"

# Decrypt the data
DECRYPTED_DATA=$(echo -n $ENCRYPTED_DATA | xxd -r -p | openssl enc -aes-128-cbc -d -K $KEY -iv $IV -nopad)

# Print the decrypted data
echo "$DECRYPTED_DATA"

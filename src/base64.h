#ifndef BASE64_H
#define BASE64_H

// gcc -o base64 Main.c Base64Encode.c Base64Decode.c -lcrypto -lm
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int calcDecodeLength(const char* b64input);

int base64_decode(char* b64message, char** buffer);

int base64_encode(const char* message, char** buffer);

#endif
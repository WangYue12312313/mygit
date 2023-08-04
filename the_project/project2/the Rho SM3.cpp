#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <openssl/evp.h>
#include<iostream>
#include <map>
#include<chrono>
#include <cmath>
#include<time.h>
using namespace std;


#define MAX_MESSAGE_LENGTH 1024  // 最大消息长度
#define HASH_LENGTH 32  // SM3哈希值长度
#define length 4
void sm3_hash(const unsigned char* message, size_t message_len, unsigned char* digest) {
    EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sm3();
    EVP_DigestInit_ex(md_ctx, md, NULL);
    EVP_DigestUpdate(md_ctx, message, message_len);
    EVP_DigestFinal_ex(md_ctx, digest, NULL);
    EVP_MD_CTX_free(md_ctx);
}

int main() {
    unsigned char message[MAX_MESSAGE_LENGTH] = "1234567890";
    unsigned char message2[MAX_MESSAGE_LENGTH] = "1234567890";
    unsigned char digest[HASH_LENGTH];
    unsigned char tmp_digest[HASH_LENGTH];


    int i = 0;
    int result;
    auto startTime = chrono::system_clock::now();
    auto endTime = chrono::system_clock::now();
    sm3_hash(message, strlen((const char*)message), digest);
    sm3_hash(message2, strlen((const char*)message2), tmp_digest);
    while (++i) {

        if (i % 1024 == 0)
            cout << ".";
        if (i % 16 == 0)
        {
            memcpy(message2, tmp_digest, HASH_LENGTH);

            sm3_hash(tmp_digest, strlen((const char*)tmp_digest), tmp_digest);
           

            result = memcmp(digest, tmp_digest, length);
            if (result == 0)
                break;
        }
        memcpy(message, digest, HASH_LENGTH);
        sm3_hash(digest, strlen((const char*)digest), digest);
     
        result = memcmp(digest, tmp_digest, length);
        if (result == 0)
            break;





    }

    cout << endl << "message1:";
    for (int t = 0; t < HASH_LENGTH; t++)
    {
        printf("%02x  ", message[t]);
    }

    cout << endl << "message2:";
    for (int t = 0; t <HASH_LENGTH; t++)
    {
        printf("%02x  ", message2[t]);
    }

    
    cout << endl << "hash:";
    for (int t = 0; t < length; t++)
    {
        printf("%02x  ", digest[t]);
    }
    cout << endl << "time:" << chrono::duration_cast<chrono::microseconds>(endTime - startTime).count() << endl;
    return 0;
}

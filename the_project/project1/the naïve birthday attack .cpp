#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <openssl/evp.h>
#include<iostream>
#include <map>
#include <cmath>
#include<time.h>
using namespace std;
#define MAX_MESSAGE_LENGTH 1024  // 最大消息长度
#define HASH_LENGTH 32  // SM3哈希值长度
time_t s, e;
void sm3_hash(const unsigned char* message, size_t message_len, unsigned char* digest) {
    EVP_MD_CTX* md_ctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_sm3();
    EVP_DigestInit_ex(md_ctx, md, NULL);
    EVP_DigestUpdate(md_ctx, message, message_len);
    EVP_DigestFinal_ex(md_ctx, digest, NULL);
    EVP_MD_CTX_free(md_ctx);
}

int main() {
    unsigned char message[MAX_MESSAGE_LENGTH];

    unsigned char digest[HASH_LENGTH];
    unsigned long long sign = 0;
    map<unsigned long long, int>table;
    string str;
    int i;
    s = clock();
    for (i = 0; i < 1 << 17; i++) {
        if (i % 1 << 15)
            cout << ".";

        str = to_string(i);
        str.copy(reinterpret_cast<char*>(message), str.length());
        message[str.length()] = '\0';
        sm3_hash(message, strlen((const char*)message), digest);


        sign = ((unsigned long long)digest[0] << 24) + ((unsigned long long)digest[1] << 16) + ((unsigned long long)digest[2] << 8) + ((unsigned long long)digest[3]);
        
        map<unsigned long long, int>::iterator iter = table.find(sign);
        if (iter != table.end())
        {
            
            cout <<endl<< "the message1:" << iter->second << "    the message2:" << i << endl;
            cout << "the hash:";
            for (int t = 0; t < 4; t++)
                printf("%02x", digest[t]);
            break;
        }

        table.insert(map<unsigned long long, int>::value_type(sign, i));
        if (i == (1 << 18))
            cout << "falut";
    }
    e = clock();
    printf("\n");
    cout << e - s << "ms";
    printf("\n");


    return 0;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define SM4_ENCRYPT 1
#define SM4_DECRYPT 0

typedef unsigned char BYTE;
typedef unsigned int MY_WORD;
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define GET_UINT32_BE(n, b, i)                        \
    {                                                 \
        (n) = ((uint32_t)(b)[(i)] << 24)               \
            | ((uint32_t)(b)[(i) + 1] << 16)           \
            | ((uint32_t)(b)[(i) + 2] << 8)            \
            | ((uint32_t)(b)[(i) + 3]);                \
    }

#define PUT_UINT32_BE(n, b, i)                        \
    {                                                 \
        (b)[(i)] = (BYTE)((n) >> 24);                  \
        (b)[(i) + 1] = (BYTE)((n) >> 16);              \
        (b)[(i) + 2] = (BYTE)((n) >> 8);               \
        (b)[(i) + 3] = (BYTE)(n);                      \
    }

static const uint8_t Sbox[16][16] = {
    {0xD6, 0x90, 0xE9, 0xFE, 0xCC, 0xE1, 0x3D, 0xB7, 0x16, 0xB6, 0x14, 0xC2, 0x28, 0xFB, 0x2C, 0x05},
    {0x2B, 0x67, 0x9A, 0x76, 0x2A, 0xBE, 0x04, 0xC3, 0xAA, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99},
    {0x9C, 0x42, 0x50, 0xF4, 0x91, 0xEF, 0x98, 0x7A, 0x33, 0x54, 0x0B, 0x43, 0xED, 0xCF, 0xAC, 0x62},
    {0xE4, 0xB3, 0x1C, 0xA9, 0xC9, 0x08, 0xE8, 0x95, 0x80, 0xDF, 0x94, 0xFA, 0x75, 0x8F, 0x3F, 0xA6},
    {0x47, 0x07, 0xA7, 0xFC, 0xF3, 0x73, 0x17, 0xBA, 0x83, 0x59, 0x3C, 0x19, 0xE6, 0x85, 0x4F, 0xA8},
    {0x68, 0x6B, 0x81, 0xB2, 0x71, 0x64, 0xDA, 0x8B, 0xF8, 0xEB, 0x0F, 0x4B, 0x70, 0x56, 0x9D, 0x35},
    {0x1E, 0x24, 0x0E, 0x5E, 0x63, 0x58, 0xD1, 0xA2, 0x25, 0x22, 0x7C, 0x3B, 0x01, 0x21, 0x78, 0x87},
    {0xD4, 0x00, 0x46, 0x57, 0x9F, 0xD3, 0x27, 0x52, 0x4C, 0x36, 0x02, 0xE7, 0xA0, 0xC4, 0xC8, 0x9E},
    {0xEA, 0xBF, 0x8A, 0xD2, 0x40, 0xC7, 0x38, 0xB5, 0xA3, 0xF7, 0xF2, 0xCE, 0xF9, 0x61, 0x15, 0xA1},
    {0xE0, 0xAE, 0x5D, 0xA4, 0x9B, 0x34, 0x1A, 0x55, 0xAD, 0x93, 0x32, 0x30, 0xF5, 0x8C, 0xB1, 0xE3},
    {0x1D, 0xF6, 0xE2, 0x2E, 0x82, 0x66, 0xCA, 0x60, 0xC0, 0x29, 0x23, 0xAB, 0x0D, 0x53, 0x4E, 0x6F},
    {0xD5, 0xDB, 0x37, 0x45, 0xDE, 0xFD, 0x8E, 0x2F, 0x03, 0xFF, 0x6A, 0x72, 0x6D, 0x6C, 0x5B, 0x51},
    {0x8D, 0x1B, 0xAF, 0x92, 0xBB, 0xDD, 0xBC, 0x7F, 0x11, 0xD9, 0x5C, 0x41, 0x1F, 0x10, 0x5A, 0xD8},
    {0x0A, 0xC1, 0x31, 0x88, 0xA5, 0xCD, 0x7B, 0xBD, 0x2D, 0x74, 0xD0, 0x12, 0xB8, 0xE5, 0xB4, 0xB0},
    {0x89, 0x69, 0x97, 0x4A, 0x0C, 0x96, 0x77, 0x7E, 0x65, 0xB9, 0xF1, 0x09, 0xC5, 0x6E, 0xC6, 0x84},
    {0x18, 0xF0, 0x7D, 0xEC, 0x3A, 0xDC, 0x4D, 0x20, 0x79, 0xEE, 0x5F, 0x3E, 0xD7, 0xCB, 0x39, 0x48}
};

static const uint32_t FK[4] = {
    0xA3B1BAC6, 0x56AA3350, 0x677D9197, 0xB27022DC
};

static const uint32_t CK[32] = {
    0x00070E15, 0x1C232A31, 0x383F464D, 0x545B6269,
    0x70777E85, 0x8C939AA1, 0xA8AFB6BD, 0xC4CBD2D9,
    0xE0E7EEF5, 0xFC030A11, 0x181F262D, 0x343B4249,
    0x50575E65, 0x6C737A81, 0x888F969D, 0xA4ABB2B9,
    0xC0C7CED5, 0xDCE3EAF1, 0xF8FF060D, 0x141B2229,
    0x30373E45, 0x4C535A61, 0x686F767D, 0x848B9299,
    0xA0A7AEB5, 0xBCC3CAD1, 0xD8DFE6ED, 0xF4FB0209,
    0x10171E25, 0x2C333A41, 0x484F565D, 0x646B7279
};

static uint32_t s_box_change(uint32_t x)
{
    uint8_t b[4];
    uint8_t high;
    uint8_t low;
    uint32_t temp = 0;
    b[0] = x >> 24;
    b[1] = x >> 16;
    b[2] = x >> 8;
    b[3] = x >> 0;
    for (int i = 0; i < 4; i++)
    {
        int t = (3 - i) * 8;
        high = b[i] >> 4;
        low = b[i] & 0x0F;

        temp += (uint32_t)Sbox[high][low] << t;

    }
    return temp;
}
static uint32_t sm4_fk(uint32_t x)
{
    x = s_box_change(x);
    return (x ^ ROTATE_LEFT(x, 2) ^ ROTATE_LEFT(x, 10) ^ ROTATE_LEFT(x, 18) ^ ROTATE_LEFT(x, 24));
}

static uint32_t sm4_round(uint32_t x, uint32_t rk)
{
    x = s_box_change(x);
    return (x ^ (ROTATE_LEFT(x, 13)) ^ ROTATE_LEFT(x, 23) ^ rk);
}

void sm4_set_key(const BYTE* key, MY_WORD* rk)
{
    uint32_t K[4];
    uint32_t tmp;
    int i;

    GET_UINT32_BE(K[0], key, 0);
    GET_UINT32_BE(K[1], key, 4);
    GET_UINT32_BE(K[2], key, 8);
    GET_UINT32_BE(K[3], key, 12);

    K[0] ^= FK[0];
    K[1] ^= FK[1];
    K[2] ^= FK[2];
    K[3] ^= FK[3];

    for (i = 0; i < 32; i++)
    {
        tmp = K[1] ^ K[2] ^ K[3] ^ CK[i];
        tmp = sm4_round(tmp, K[0]);
        K[0] = K[1];
        K[1] = K[2];
        K[2] = K[3];
        K[3] = tmp;

        rk[i] = K[3];
    }
}


void sm4_crypt_ecb(BYTE input[16], BYTE output[16], BYTE key[16], int mode)
{
    MY_WORD rk[32];
    sm4_set_key(key, rk);
    uint32_t X[4];
    uint32_t tmp;
    int i;

    if (mode == 1)
    {
        GET_UINT32_BE(X[0], input, 0);
        GET_UINT32_BE(X[1], input, 4);
        GET_UINT32_BE(X[2], input, 8);
        GET_UINT32_BE(X[3], input, 12);

        for (i = 0; i < 32; i++)
        {
            tmp = X[1] ^ X[2] ^ X[3] ^ rk[i];
            tmp = sm4_fk(tmp) ^ X[0];
            X[0] = X[1];
            X[1] = X[2];
            X[2] = X[3];
            X[3] = tmp;
        }

        PUT_UINT32_BE(X[0], output, 12);
        PUT_UINT32_BE(X[1], output, 8);
        PUT_UINT32_BE(X[2], output, 4);
        PUT_UINT32_BE(X[3], output, 0);
    }
    if (mode == 0)
    {
        GET_UINT32_BE(X[0], input, 0);
        GET_UINT32_BE(X[1], input, 4);
        GET_UINT32_BE(X[2], input, 8);
        GET_UINT32_BE(X[3], input, 12);

        for (i = 31; i >= 0; i--)
        {
            tmp = X[1] ^ X[2] ^ X[3] ^ rk[i];
            tmp = sm4_fk(tmp) ^ X[0];
            X[0] = X[1];
            X[1] = X[2];
            X[2] = X[3];
            X[3] = tmp;
        }

        PUT_UINT32_BE(X[0], output, 12);
        PUT_UINT32_BE(X[1], output, 8);
        PUT_UINT32_BE(X[2], output, 4);
        PUT_UINT32_BE(X[3], output, 0);
    }
}

int main()
{

    BYTE key[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };
    BYTE encrypted[16];
    BYTE decrypted[16];
    BYTE input[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };
    LARGE_INTEGER frequency;
    LARGE_INTEGER start_time, end_time;
    double elapsed_time;

    // 获取性能计数器的频率
    QueryPerformanceFrequency(&frequency);

    // 获取开始时间
    QueryPerformanceCounter(&start_time);

    sm4_crypt_ecb(input, encrypted, key, 1);

    sm4_crypt_ecb(encrypted, decrypted, key, 0);
    // 获取结束时间
    QueryPerformanceCounter(&end_time);

    // 计算运行时间（单位：秒）
    elapsed_time = (double)(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;

    printf("Elapsed Time: %f seconds\n", elapsed_time);

    BYTE the_right_cip[16] = { 0x68,0x1E,0xDF,0x34,0xD2,0x06,0x96,0x5E,0x86,0xB3,0xE9,0x4F,0x53,0x6E,0x42,0x46 };
    for (int i = 0; i < 16; i++)
    {
        if (encrypted[i] != the_right_cip[i])
        {
            printf("test failed");
            printf("\n");
            return 1;
        }
    }

    return 0;
}
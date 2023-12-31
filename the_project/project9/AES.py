import string
import numpy as np
import time

# # 将字符n转化为8bit二进制的ascii值
# def er(n):
#     a = list('00000000')
#     i = 7
#     while n > 0:
#         if n % 2 == 1:
#             a[i] = '1'
#         n //= 2
#         i -= 1
#     b = ''.join('%s' % id for id in a)
#     return b


# 将字符串n转化为按ascii表示的128bit.本实验中作为明文与密钥使用。
def text(n):
    a = list(map(lambda x: ord(x), n))
    while len(a) < 16:
        a.append('0')
    a = np.array(a)
    c = a.astype(int)
    d: object = np.fromfunction(lambda a, b: c[a + 4 * b], (4, 4), dtype=int)

    return d


RCon = [0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36]
MIX_C = [[0x2, 0x3, 0x1, 0x1],
         [0x1, 0x2, 0x3, 0x1],
         [0x1, 0x1, 0x2, 0x3],
         [0x3, 0x1, 0x1, 0x2]]
S_BOX = [[0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76],
         [0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0],
         [0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15],
         [0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75],
         [0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84],
         [0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF],
         [0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8],
         [0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2],
         [0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73],
         [0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB],
         [0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79],
         [0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08],
         [0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A],
         [0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E],
         [0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF],
         [0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16]]


# 给定一个明文x和密钥key，将State初始化为x，同时产生11个轮密钥，并进行AddRoundKey操作，即将RoundKey与State异或，然后进行10轮迭代。
# 对前9轮中的每一轮，用S盒对State进行一次代换操作SubBytes；对State做一次行移位ShiftRows；再对State做一次列混合MixColumns；然后进行AddRoundKey操作。
# 对State依次进行操作SubBytes，ShiftRows，AddRoundKey。
# 将State定义为密文y。

# 字节代换操作
def SubBytes(n):
    for x in range(4):
        for y in range(4):
            n[x][y] = S_BOX[n[x][y] // 16][n[x][y] % 16]


# 行移位
def ShiftRows(n):
    w = np.empty((4, 4), dtype=int)
    for x in range(4):
        for y in range(4):
            w[x][y] = n[x][(y + x) % 4]
    n[:, :] = w[:, :]


# 有限域G[8]乘法
def galoismul(x, i):
    if i == 1:
        return x
    if i == 2:
        y = x
        x = (x << 1) % (2 ** 8)
        if (y // 2 ** 7) == 0:
            return x
        else:
            return x ^ 0b00011011
    if i == 3:
        return x ^ galoismul(x, 2)


# 列混合
def MixColumns(n, s):
    w = np.zeros((4, 4), dtype=int)

    for j in range(4):
        for k in range(4):
            for i in range(4):
                w[j][k] ^= galoismul(n[i][k], s[j][i])
    n[:, :] = w[:, :]


# 轮密钥产生
# 对W数组扩充40个新列，构成总共44列的扩展密钥数组。新列以如下的递归方式产生：
# 1.如果i不是4的倍数，那么第i列由如下等式确定：
# W[i]=W[i-4]⨁W[i-1]
# 2.如果i是4的倍数，那么第i列由如下等式确定：
# W[i]=W[i-4]⨁T(W[i-1])
# 其中，T是一个有点复杂的函数。
# 函数T由3部分组成：字循环、字节代换和轮常量异或，这3部分的作用分别如下。
# a.字循环：将1个字中的4个字节循环左移1个字节。即将输入字[b0, b1, b2, b3]变换成[b1,b2,b3,b0]。
# b.字节代换：对字循环的结果使用S盒进行字节代换。
# c.轮常量异或：将前两步的结果同轮常量Rcon[j]进行异或，其中j表示轮数。
# T函数
def T(n, j):
    w = np.array([0, 0, 0, 0])

    for i in range(4):
        w[i] = n[(i + 1) % 4]

    for i in range(4):
        w[i] = S_BOX[w[i] // 16][w[i] % 16]

    w[0] = w[0] ^ RCon[(j//4)-1]

    n = w
    return n


# 轮密钥拓展
def KeyExpansion(n):
    w = np.zeros((4, 44), dtype=int)
    w[:, 0:4] = n
    i = 4
    while i < 44:
        if i % 4 == 0:

            w[:, i] = w[:, i - 4] ^ T(w[:, i - 1], i)
        else:
            w[:, i] = w[:, i - 4] ^ w[:, i - 1]

        i += 1
    return w


# 轮密钥加
def AddRoundKey(a, b, i):
    for x in range(4):
        for y in range(4):
            a[x][y] ^= b[x][y + 4 * i]


# aes-128加密算法
def aes(state):
    s = text(state)

    K = KeyExpansion(s)
    print(K)
    AddRoundKey(s, s, 0)
    for i in range(1, 11):
        SubBytes(s)
        ShiftRows(s)
        if i < 10:
            MixColumns(s, MIX_C)

        AddRoundKey(s, K, i)
    c = []
    for x in np.nditer(s,order='F'):
        e = format(int(hex(x), 16), 'x')
        c.append(e)
        b = ''.join('%s' % id for id in c)
    print("加密结果为：", b)

start_time = time.time()
aes('202100460049000')
end_time = time.time()
elapsed_time = end_time - start_time

print(f"Elapsed Time: {elapsed_time:.6f} seconds")
# m = [[0x3C, 0xA1, 0x0B, 0x21], [0x57, 0xF0, 0x19, 0x16], [0x90, 0x2E, 0x13, 0x80], [0xAC, 0xC1, 0x07, 0xBD]]


# m = [[0x3C, 0x57, 0x90, 0xAC], [0xA1, 0xF0, 0x2E, 0xC1, ], [0x0B, 0x19, 0x13, 0x07], [0x21, 0x16, 0x80, 0xBD]]
#
# m=np.array(m)
# print(KeyExpansion(m))

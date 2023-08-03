分工：
赵正一-202100460049 ： 7 8 9 10 11 12 

Project7: Try to Implement this scheme

实现内容：这个项目我并没有非常清楚地想明白具体要做什么，在查找有关资料后，我仅仅完成了生成最小支配分区（MDP）的算法。
技巧与创新点：我使用了自定义的动态数组结构 struct Vector 来存储返回的向量，因为C语言没有直接的向量类型，同时在 push_back 函数中，当动态数组的大小超出容量时，使用 realloc 进行重新分配，可以减少内存频繁分配和释放的次数，提高性能，并且通过使用 capacity 变量来控制动态数组的大小，以便在必要时动态增加容量。这可以避免频繁的内存重新分配，提高性能。

Project8: AES impl with ARM instruction

运行时间：
由于是arm指令集，与笔记本的intel架构不同，需要交叉编译运行，但是尝试了很多方法依旧无法对代码进行调试。

实现内容：
该代码实现了AES-128加密算法的 ARMv8 架构优化版本。AES（Advanced Encryption Standard）是一种常用的对称加密算法，用于保护敏感数据的机密性。在这个实现中，使用了ARM NEON指令集来加速加密过程。首先，定义了一个固定的Rcon（Round Constant）数组，Sbox（Substitution Box）数组和一些常量。ase128_enc_armv8 函数实现了AES-128的加密过程。该函数使用ARM NEON指令来优化计算。具体来说，它将输入数据分成128位（16字节）的数据块，并根据预生成的轮密钥进行加密。在每一轮中，通过使用AES指令来执行逐位的状态混淆和轮密钥的XOR操作，以实现加密。最终的加密结果存储在输出数组 ou 中。aes128_dec_armv8 函数实现了AES-128的解密过程，类似于加密函数，但使用不同的AES指令和密钥。generate_AES_keys 函数用于生成AES加密过程中所需的轮密钥。它从输入的16字节密钥生成44个32位的轮密钥。该函数使用了密钥扩展算法，通过循环左移、S盒替换和轮常数与轮密钥的异或操作来生成轮密钥。main 函数演示了如何使用上述函数进行AES-128加密。首先，定义了输入数据和AES密钥。然后，生成轮密钥，最后调用 ase128_enc_armv8 函数进行加密，并将结果存储在输出数组中。

创新点和技巧：
代码中的创新点在于使用了ARM NEON指令集进行加速。ARM NEON是一组SIMD（单指令，多数据）指令，可同时处理多个数据元素，提高了加密算法的并行性能。通过使用密钥扩展算法，代码生成了整个AES加密过程中所需的轮密钥，避免了在每一轮中重复计算轮密钥。代码使用了合适的数据类型和数据结构，如 uint8x16_t 类型来表示128位数据块，并利用指针操作进行数据的加载和存储，以提高计算效率。

roject9: AES / SM4 software implementation

SM4：
运行时间：0.000005 seconds

实现内容与技巧：
该代码实现了 SM4 加密算法的 ECB（Electronic Codebook）模式加解密功能，并且通过性能计数器 QueryPerformanceCounter 计算了加解密操作的运行时间。SM4 是一种对称加密算法，是中国国家密码管理局发布的国密算法之一。实现了以下功能：
定义了一些常量、宏和数据结构，用于实现 SM4 加解密算法。
实现了 S-box 变换和轮函数中的 FK 和 CK 常量的定义。
实现了 s_box_change 函数，用于进行 S-box 变换。
实现了 sm4_fk 函数，对一个输入进行 S-box 变换并与位移操作后的结果进行异或。
实现了 sm4_round 函数，对一个输入进行 S-box 变换并与多次位移操作和轮密钥异或操作后的结果进行异或。
实现了 sm4_set_key 函数，用于生成 SM4 加密的轮密钥。
实现了 sm4_crypt_ecb 函数，用于对 16 字节的数据块进行 ECB 模式的加解密。
在 main 函数中，通过定义密钥和输入数据，进行了 SM4 加密和解密操作，并通过 QueryPerformanceCounter 计算了加解密操作的运行时间。
验证加密结果是否正确，如果加密结果与预期的结果不符，输出测试失败。

AES：
运行时间：0.001993 seconds

实现内容与技巧：
该代码实现了AES-128加密算法的功能
导入模块和定义常量：代码开始部分导入了需要的模块，如numpy和time。还定义了S盒、轮常量、列混合矩阵等AES算法中使用的常量。
文本转换函数：text函数将输入的字符串转换为128位的ASCII表示，用作明文和密钥。
字节代换和行移位：SubBytes函数根据S盒进行字节代换操作，ShiftRows函数对状态进行行移位。
有限域Galois乘法：galoismul函数实现了有限域Galois乘法操作，用于列混合中的乘法运算。
列混合：MixColumns函数实现了列混合操作，利用有限域乘法将状态矩阵的每一列进行混合。
轮密钥产生：T函数用于生成轮密钥，KeyExpansion函数根据主密钥生成轮密钥数组。
轮密钥加：AddRoundKey函数实现了轮密钥加操作，将轮密钥与状态矩阵进行异或。
AES加密函数：aes函数实现了AES的加密过程，包括字节代换、行移位、列混合、轮密钥加等步骤。
性能测试：代码中包含了对加密过程的性能测试部分，使用time模块来测量代码执行时间。

Project10: report on the application of this deduce technique in Ethereum with ECDSA

运行时间：0.158470 seconds

实现内容：
实现了判断一个数是否为二次剩余（Quadratic Residue）的函数 is_quadratic_residue。
实现了模逆运算的函数 modular_inverse，用于计算模的逆元。
定义了扩展的欧几里得算法函数 extended_gcd，用于计算模的扩展最大公约数。
实现了解决二次剩余的算法函数 solve_quadratic_residue。
实现了生成密钥对的函数 generate_keypair，其中使用了基于随机数生成的私钥和椭圆曲线点乘来生成公钥。
定义了点加法和点倍乘法的函数，用于椭圆曲线上的运算。
实现了生成数字签名的函数 signature_generation，其中使用私钥、消息和随机数生成签名。
实现了从签名推导公钥的函数 deduce_public_key，基于签名信息反推可能的公钥。
通过哈希函数 hash_function，实现了对消息的哈希操作。
在 if __name__ == '__main__': 块中，生成密钥对、生成数字签名、从签名推导公钥，并计算执行时间。

技巧和创新点：
点运算模块化设计： 代码模块化地实现了椭圆曲线上的点加法、点倍乘和点解密等核心运算，使得代码结构清晰，易于维护。
模块化的随机数生成： 使用 secrets 模块生成安全的随机数，确保密钥和随机数的高度安全性。
快速幂和模逆的优化： 使用快速幂算法和模逆计算来优化指数运算和模逆运算，提高了计算效率。
椭圆曲线数学性质的利用： 代码充分利用椭圆曲线的数学性质，如模平方根的求解和扩展欧几里得算法，用于点运算和签名生成。
对称性利用： 在推导公钥的过程中，利用了椭圆曲线的对称性，优化了公钥的计算。
高效的点运算： 代码通过点加法和点倍乘运算，实现了高效的椭圆曲线上的点操作，提高了整体的执行效率。

Project11: impl sm2 with RFC6979

运行时间：0.004743 seconds

实现内容：
导入了 gmssl 库中的 sm2 模块，以及 Python 内置的 hashlib 和 hmac 模块。
定义了 bits2int 函数，用于将字节数据转换为整数。函数将字节数据转换为一个大整数，并将其位数截断或填充以适应给定的位长度。
定义了 bits2octets 函数，将比特数据（bit string）转换为字节序列（octets）。这在 SM2 算法的随机数生成过程中用到。
定义了 generate_k 函数，用于生成符合要求的随机数 k。这个函数在 SM2 数字签名过程中用于生成随机数。函数中使用了 RFC6979 算法生成随机数。
定义了一些参数，包括椭圆曲线的参数、哈希函数、消息、私钥和公钥。
将私钥从十六进制字符串转换为字节数据。
调用 generate_k 函数生成随机数 k。
创建 sm2_crypt 对象，使用 gmssl 库的 CryptSM2 类，初始化了私钥和公钥。
使用私钥对消息进行签名，生成一个数字签名。
使用公钥进行验证，确保生成的数字签名是有效的。

技巧和创新点：
代码中使用了 gmssl 库提供的 CryptSM2 类来简化 SM2 算法的数字签名和验证过程。
使用了位操作和字节序列的转换来处理比特数据和整数之间的转换。

Project12: verify the above pitfalls with proof-of-concept code

ECDSA：

实现内容：
导入了ECDSA模块，用库函数实现ESDSA。
定义了leading_k函数，用于验证Leaking k leads to leaking of d,d=(s*k-e)*pow(r, -1, n))。
定义了reusing_k函数，用于验证Reusing k leads to leaking of d,d=(s*e1-s1*e)*pow((s1-s)*r, -1, n)%n
定义了two_user_k函数，用于验证Two users, using k leads to leaking of d, that is they can deduce each other’s d,d=((s * k - e) * pow(r, -1, n)) % n
将verify(pk, (r, n-s), m)中的s改为n-s，用于验证Malleability, e.g. （r, s) and （r, -s)  are both valid signatures, lead to blockchain network split
定义了forge_sig函数，用于验证One can forge signature if the verification does not check m,.选随机数a、b，计算K=aG+bP2.计算r=X(K)，s=rb−1，e=arb−1，并且定义了verify1函数，用于验证：s−1∗(eG+rP)=r−1b∗(arb−1G+rP)=aG+bP=K，验证通过

创新点与技巧：
深入学习了ecdsa库函数内容，根据有关数学公式，编写对应的POC代码进行证明

Schnorr：

实现内容、创新点与技巧：
基本同于ecdsa的内容，验证思路也近似，额外对Same d and k with ECDSA, leads to leaking of d进行了验证，因为该缺陷对于ecdsa会退化为前边已经证明的内容，故在ecdsa内容中进行了忽略。d= ((s * s1 - e1) * pow(r1 + e * s1, -1, n)) % n。

sm2：与前两个算法的思路基本一致。

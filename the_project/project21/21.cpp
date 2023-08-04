from Crypto.Hash import SHA256
from Crypto.Random import random

# Schnorr签名算法
def schnorr_sign(private_key, message) :
    hash_obj = SHA256.new(message)
    hash_digest = int.from_bytes(hash_obj.digest(), byteorder = 'big')
    nonce = random.randint(1, private_key - 1)
    R = pow(2, nonce, private_key)
    e = hash_digest ^ R
    s = (nonce - private_key * e) % (private_key - 1)
    return R, s

    # Schnorr签名验证
    def schnorr_verify(public_key, message, R, s) :
    hash_obj = SHA256.new(message)
    hash_digest = int.from_bytes(hash_obj.digest(), byteorder = 'big')
    u = (pow(public_key, R, public_key) * pow(R, s, public_key)) % public_key
    return u == pow(2, hash_digest, public_key)

    # Schnorr批量验证
    def schnorr_batch_verify(public_key, messages, Rs, ss) :
    if len(messages) != len(Rs) or len(Rs) != len(ss) :
        return False

        hash_digests = []
        for message in messages :
hash_obj = SHA256.new(message)
hash_digests.append(int.from_bytes(hash_obj.digest(), byteorder = 'big'))

u = 1
for i in range(len(messages)) :
    u = (u * pow(public_key, Rs[i], public_key) * pow(Rs[i], ss[i], public_key)) % public_key

    return u == pow(2, sum(hash_digests) % (public_key - 1), public_key)

    # 示例用法
    if __name__ == '__main__':
private_key = 1234567890  # 私钥
public_key = pow(2, private_key, 10 * *9 + 7)  # 公钥

# 生成多个签名和消息
num_signatures = 5
messages = [b"Message " + bytes(str(i), 'utf-8') for i in range(num_signatures)]
signatures = [schnorr_sign(private_key, message) for message in messages]
Rs = [signature[0] for signature in signatures]
ss = [signature[1] for signature in signatures]

# 验证批量签名
is_valid = schnorr_batch_verify(public_key, messages, Rs, ss)

print("Batch Signature valid:", is_valid)


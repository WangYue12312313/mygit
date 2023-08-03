from gmssl import sm2
import hashlib
import hmac
import time

def bits2int(data, qlen):
    x = int.from_bytes(data, byteorder='big')
    l = len(data) * 8
    if l > qlen:
        return x >> (l - qlen)
    return x

def bits2octets(data, q):
    z1 = bits2int(data, q.bit_length())
    z2 = z1 - q
    if z2 < 0:
        z2 = z1
    return z2.to_bytes((z2.bit_length() + 7) // 8, byteorder='big')

def generate_k(generator, order, hash_func, private_key_bytes, message):
    q = order
    hsize = hash_func().digest_size
    rolen = (q.bit_length() + 7) // 8
    bx = (
        private_key_bytes,
        bits2octets(message, q),
        b"",
    )

    v = b'\x01' * hsize
    k = b'\x00' * hsize

    k = hmac.new(k, digestmod=hash_func)
    k.update(v + b'\x00')
    for i in bx:
        k.update(i)
    k = k.digest()

    v = hmac.new(k, v, hash_func).digest()
    k = hmac.new(k, digestmod=hash_func)
    k.update(v + b'\x01')
    for i in bx:
        k.update(i)
    k = k.digest()

    v = hmac.new(k, v, hash_func).digest()

    while True:
        t = b''
        while len(t) < rolen:
            v = hmac.new(k, v, hash_func).digest()
            t += v
        secret = bits2int(t, q.bit_length())
        if 1 <= secret < q:
            return secret
        k = hmac.new(k, v + b'\x00', hash_func).digest()
        v = hmac.new(k, v, hash_func).digest()

p = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
n = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
Gx = 0x32c4ae2c1f1981195f9904466a39c9948fe30bbff2660be1715a4589334c74c7
Gy = 0xbc3736a2f4f6779c59bdcee36b692153d0a9877cc62a474002df32e52139f0a0

generator = (Gx, Gy)
order = n
hash_func = hashlib.sha256
message = b"Hello, world!"
# 生成密钥对
private_key = '00B9AB0B828FF68872F21A837FC303668428DEA11DCD1B24429D0C99E24EED83D5'
public_key = 'B9C9A6E04E9C91F7BA880429273747D7EF5DDEB0BB2FF6317EB00BEF331A83081A6994B8993F3F5D6EADDDB81872266C87C018FB4162F5AF347B483E24620207'
# 创建SM2对象
start_time = time.time()
private_key_bytes = bytes.fromhex(private_key)
k = generate_k(generator, order, hash_func, private_key_bytes, message)
sm2_crypt = sm2.CryptSM2(
    public_key=public_key, private_key=private_key)


#k = 74890137352751800706386558303504304731182793799763314201807687680821020571786

# 使用私钥进行签名
signature = sm2_crypt.sign(message, hex(k)[2:])

end_time = time.time()
elapsed_time = end_time - start_time
print(f"Elapsed Time: {elapsed_time:.6f} seconds")
# 使用公钥进行验证
assert sm2_crypt.verify(signature, message)


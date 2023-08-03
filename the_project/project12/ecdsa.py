from ecdsa import SigningKey, SECP256k1, util, VerifyingKey
import ecdsa
import hashlib
from pyasn1.type import univ
from pyasn1.codec.der import encoder, decoder
from cryptography.hazmat.primitives import serialization


def generate_keys():
    sk = SigningKey.generate(curve=SECP256k1)
    vk = sk.get_verifying_key()
    secret_key = sk.to_string().hex()
    verify_key = vk.to_string().hex()
    return secret_key, verify_key

def sign(secret_key, m):
    sk = SigningKey.from_string(bytes.fromhex(secret_key), curve=SECP256k1)
    signature = sk.sign(bytes(m, 'utf-8'),k=1)
    r, s = util.sigdecode_string(signature, sk.curve.order)
    return r, s

def verify(verify_key, signature, m):
    vk = VerifyingKey.from_string(bytes.fromhex(verify_key), curve=SECP256k1)
    signature_bytes = util.sigencode_string(signature[0], signature[1], vk.curve.order)
    return vk.verify(signature_bytes, bytes(m, 'utf-8'))




def leading_k(m,n,r,s,k):
    hash0=hashlib.sha1(bytes(m, 'utf-8')).digest()
    e=int(hash0.hex(),16)
    print(((s * k - e) * pow(r, -1, n)) % n)

def reusing_k(m,n,r,s,m1,s1):
    hash0 = hashlib.sha1(bytes(m, 'utf-8')).digest()
    e = int(hash0.hex(), 16)
    hash1 = hashlib.sha1(bytes(m1, 'utf-8')).digest()
    e1 = int(hash1.hex(), 16)
    print((s*e1-s1*e)*pow((s1-s)*r, -1, n)%n)

def two_user_k(m,n,r,s,m1,s1):
    hash0 = hashlib.sha1(bytes(m, 'utf-8')).digest()
    e = int(hash0.hex(), 16)
    hash1 = hashlib.sha1(bytes(m1, 'utf-8')).digest()
    e1 = int(hash1.hex(), 16)
    print(((s * k - e) * pow(r, -1, n)) % n)
    print(((s1 * k - e1) * pow(r, -1, n)) % n)

def forge_sig(a, b, public_key):
    vk = VerifyingKey.from_string(bytes.fromhex(public_key), curve=SECP256k1)
    aG = a * SECP256k1.generator
    bP = b * vk.pubkey.point
    kk = aG + bP
    return kk.x() % n


a = 453432
b = 438435


def verify1(public_key, signature, e):
    verifying_key = VerifyingKey.from_string(bytes.fromhex(public_key), curve=SECP256k1)
    signature = ecdsa.util.sigencode_string(signature[0], signature[1], verifying_key.curve.order)
    is_valid = verifying_key.verify_digest(signature, e, sigdecode=ecdsa.util.sigdecode_string)
    return is_valid


if __name__ == '__main__':
    curve = ecdsa.curves.SECP256k1
    curve_name = curve.name
    n = curve.order

    sk, pk = generate_keys()
    sk1, pk1 = generate_keys()
    m = 'hello'
    m1 = 'hello1'
    k=1

    r, s = sign(sk, m)
    r1, s1 = sign(sk, m1)
    #print('Signature (r, s):', r, s)
    #print('Verification result:', verify(pk, (r, n-s), m))
    #print(int(sk, 16))
    #print(int(sk1, 16))
    #leading_k(m,n,r,s,k)
    #reusing_k(m,n,r,s,m1,s1)
    #two_user_k(m,n,r,s,m1,s1)

    rr = forge_sig(a, b, pk) % n
    ss = rr * pow(b, -1, n) % n
    e = int(a * rr * pow(b, -1, n) % n)
    e1 = e.to_bytes((e.bit_length() + 7) // 8, 'big')
    signature = (rr, ss)
    is_valid = verify1(pk, signature, e1)
    if is_valid:
        print("forge signature successfully")
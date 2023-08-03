import hashlib
import random
import ecdsa
import ecpy.curves
from ecdsa.curves import SECP256k1
from ecdsa.numbertheory import inverse_mod
from ecpy.curves import Curve, Point
from ecdsa import SigningKey, SECP256k1, util, VerifyingKey
from pyasn1.type import univ
from pyasn1.codec.der import encoder, decoder
from cryptography.hazmat.primitives import serialization

def generate_keys():
    curve = SECP256k1
    sk = random.randint(1, curve.order - 1)
    pk = sk * curve.generator
    return sk, pk


def sign(private_key, message, k1=None):
    curve = SECP256k1
    if k1 is not None:
        k = k1
    else:
        k = random.randint(1, curve.order - 1)
    r = k * curve.generator
    e = int.from_bytes(
        hashlib.sha256(ecdsa.ellipticcurve.AbstractPoint.to_bytes(r) + message).digest(), 'big')
    s = (k + e * private_key) % curve.order
    return r, s


def verify(pk, message, signature):
    curve = SECP256k1
    r, s = signature
    e = int.from_bytes(hashlib.sha256(ecdsa.ellipticcurve.AbstractPoint.to_bytes(r) + message).digest(), 'big')
    sG = ecdsa.ellipticcurve.PointJacobi.__mul__(curve.generator, s)
    return sG == ecdsa.ellipticcurve.PointJacobi.mul_add(r, 1,pk, e)



def ecdsa_sign(secret_key, m):
    sk = SigningKey.from_string(secret_key, curve=SECP256k1)
    signature = sk.sign(m)
    r, s = util.sigdecode_string(signature, sk.curve.order)
    return r, s


def ecdsa_verify(verify_key, signature, m):
    vk = VerifyingKey.from_string(bytes.fromhex(verify_key), curve=SECP256k1)
    signature_bytes = util.sigencode_string(signature[0], signature[1], vk.curve.order)
    return vk.verify(signature_bytes, m)

curve = ecdsa.curves.SECP256k1
n = curve.order
k = 61851351

private_key, public_key = generate_keys()
message = b"Hello, Schnorr!"
signature = sign(private_key, message, k)
print("Verification result:", verify(public_key, message, signature))

# Leaking k leads to leaking of d
r, s = signature
e = int.from_bytes(hashlib.sha256(int(r.x()).to_bytes(32, 'big') + int(r.y()).to_bytes(32, 'big') + message).digest(),
                   'big')
print("d=", ((s - k) * pow(e, -1, n)) % n)


# Reusing k leads to leaking of d
message1= b'Hello, Schnorr!1'
signature1 = sign(private_key, message1, k)
r1, s1 = signature1
e1 = int.from_bytes(hashlib.sha256(int(r1.x()).to_bytes(32, 'big') + int(r1.y()).to_bytes(32, 'big') + message1).digest(), 'big')
print("d=", ((s - s1) * pow(e - e1, -1, n)) % n)


# reusing k by different users
private_key1, public_key1 = generate_keys()
signature2 = sign(private_key1, message1, k)
r2, s2 = signature1
e2 = int.from_bytes(
    hashlib.sha256(int(r2.x()).to_bytes(32, 'big') + int(r2.y()).to_bytes(32, 'big') + message1).digest(), 'big')
print("d=",((s - s2 + e2 * private_key1) * pow(e, -1, n)) % n)


# user1 compute user3's d with two signatures , his own d and messages
print("d=", ((s2 - s + e * private_key) * pow(e2, -1, n)) % n)


# Malleability, e.g. r, s and r, -s, are both valid signatures, lead to blockchain network split

print('Verification result:', verify(public_key, message, (r, s)))

# One can forge signature if the verification does not check m
r3 = ecdsa.ellipticcurve.PointJacobi.__add__(r, r1)
e3 = (e + e1) % n
s3 = (s + s1) % n
signature_forged = (r3, s3)


def verify1(public_key, e, signature):
    curve = SECP256k1
    r, s = signature
    sG = s * curve.generator
    eP = e * public_key
    return sG == r + eP

if verify1(public_key, e3, signature_forged):
    print("forge signature successfully")

# Same d and k with ECDSA, leads to leaking of d
sk = ecdsa.SigningKey.from_secret_exponent(private_key, curve)
pk = sk.get_verifying_key().to_string()
sk1 = private_key.to_bytes(32, 'big')


signature4 = ecdsa_sign(sk1, message)
r3, s3 = signature4

h = hashlib.sha1(message).digest()
e3 = int(h.hex(), 16)
print("d=", ((s * s3 - e3) * pow(r3 + e * s3, -1, n)) % n)



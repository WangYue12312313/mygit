from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ec


def main():
    curve = ec.SECP256R1()
    alice_private_key = ec.generate_private_key(curve)
    alice_public_key = alice_private_key.public_key()

    bob_public_key_bytes = open('bob_public_key.pem', 'rb').read()
    bob_public_key = serialization.load_pem_public_key(bob_public_key_bytes)

    shared_secret = alice_private_key.exchange(ec.ECDH(), bob_public_key)

    print("Shared Secret:", shared_secret.hex())


if __name__ == "__main__":
    main()

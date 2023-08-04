from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ec


def main():
    curve = ec.SECP256R1()
    bob_private_key = ec.generate_private_key(curve)
    bob_public_key = bob_private_key.public_key()

    with open('bob_public_key.pem', 'wb') as f:
        f.write(
            bob_public_key.public_bytes(serialization.Encoding.PEM, serialization.PublicFormat.SubjectPublicKeyInfo))


if __name__ == "__main__":
    main()

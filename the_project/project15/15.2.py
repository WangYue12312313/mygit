import socket

from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import ec


def main():
    client_private_key = ec.generate_private_key(ec.SECP256R1())
    client_public_key = client_private_key.public_key()

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('localhost', 12345))

    client_socket.sendall(client_public_key.public_bytes(ec.EllipticCurvePublicKey.Encoding.X962,
                                                         ec.EllipticCurvePublicKey.PublicFormat.Compressed))

    server_public_key_bytes = client_socket.recv(1024)
    server_public_key = ec.EllipticCurvePublicKey.from_encoded_point(
        ec.SECP256R1(), server_public_key_bytes, ec.EllipticCurvePublicKey.PublicFormat.Compressed
    )

    server_signature = client_socket.recv(1024)

    try:
        server_public_key.verify(
            server_signature, b"Hello from server", ec.ECDSA(hashes.SHA256())
        )
        print("Server signature is valid")
    except Exception as e:
        print("Server signature is not valid:", e)

    client_socket.close()


if __name__ == "__main__":
    main()

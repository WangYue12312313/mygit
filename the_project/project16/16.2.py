import socket

from cryptography.hazmat.primitives.asymmetric import ec


def main():
    client_private_key = ec.generate_private_key(ec.SECP256R1())
    client_public_key = client_private_key.public_key()

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('localhost', 12345))

    client_socket.sendall(client_public_key.public_bytes(ec.EllipticCurvePublicKey.Encoding.X962,
                                                         ec.EllipticCurvePublicKey.PublicFormat.Compressed))

    encrypted_message = client_private_key.encrypt(b"Secret message for Alice", ec.ECIES())

    client_socket.sendall(encrypted_message)

    server_public_key_bytes = client_socket.recv(1024)
    server_public_key = ec.EllipticCurvePublicKey.from_encoded_point(
        ec.SECP256R1(), server_public_key_bytes
    )

    client_socket.close()


if __name__ == "__main__":
    main()

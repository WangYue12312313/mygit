import socket

from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat


def main():
    server_private_key = ec.generate_private_key(ec.SECP256R1())
    server_public_key = server_private_key.public_key()

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('localhost', 12345))
    server_socket.listen(1)

    print("Server listening on localhost:12345")

    while True:
        client_socket, _ = server_socket.accept()
        print("Client connected")

        client_public_key_bytes = client_socket.recv(1024)
        client_public_key = ec.EllipticCurvePublicKey.from_encoded_point(
            ec.SECP256R1(), client_public_key_bytes, Encoding.X962, PublicFormat.Compressed
        )

        server_signature = server_private_key.sign(b"Hello from server")

        client_socket.sendall(server_public_key.public_bytes(Encoding.X962, PublicFormat.Compressed))
        client_socket.sendall(server_signature)

        client_socket.close()


if __name__ == "__main__":
    main()



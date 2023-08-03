import secrets
import time

prime_modulus_p = 115792089237316195423570985008687907853269984665640564039457584007908834671663
prime_modulus_n = 115792089237316195423570985008687907852837564279074904382605163141518161494337
constant_a = 0
base_point_G = (55066263022277343669578718895168534326250603453777594175500187360389116729240, 32670510020758816978083085130507043184471273380659243275938904335757337482424)

def is_quadratic_residue(number, modulus):
    return pow(number, (modulus - 1) // 2, modulus)

def modular_inverse(B, N):
    if B == N:
        return (B, 1, 0)
    else:
        i = 0
        b = [B]
        n = [N]
        q = []
        r = []
        flag = False

        while not flag:
            q.append(n[i] // b[i])
            r.append(n[i] % b[i])
            n.append(b[i])
            b.append(r[i])
            if r[i] == 0:
                flag = True
            i += 1
        tmp = b[i - 1]
        x = [1]
        y = [0]

        i -= 2
        num = i

        while i >= 0:
            y.append(x[num - i])
            x.append(y[num - i] - q[i] * x[num - i])
            i -= 1

        return (tmp, x[-1], y[-1])

def extended_gcd(b, n):
    (gcd, x, y) = modular_inverse(b, n)

    if gcd == 1:
        return x % n
    else:
        return -1

def solve_quadratic_residue(n, p):
    assert is_quadratic_residue(n, p) == 1
    if p % 4 == 3:
        return pow(n, (p + 1) // 4, p)
    q = p - 1
    s = 0
    while q % 2 == 0:
        q = q // 2
        s += 1
    for z in range(2, p):
        if is_quadratic_residue(z, p) == p - 1:
            c = pow(z, q, p)
            break
    r = pow(n, (q + 1) // 2, p)
    t = pow(n, q, p)
    m = s
    if t % p == 1:
        return r
    else:
        i = 0
        while t % p != 1:
            temp = pow(t, 2 ** (i + 1), p)
            i += 1
            if temp % p == 1:
                b = pow(c, 2 ** (m - i - 1), p)
                r = r * b % p
                c = b * b % p
                t = t * c % p
                m = i
                i = 0
        return r

def generate_keypair():
    secret_key = int(secrets.token_hex(32), 16)
    public_key = point_multiplication(secret_key, base_point_G)
    return secret_key, public_key

def point_addition(P, Q):
    if P == 0 and Q == 0:
        return 0
    elif P == 0:
        return Q
    elif Q == 0:
        return P
    else:
        if P[0] > Q[0]:
            tmp = P
            P = Q
            Q = tmp

        result = []
        t = (Q[1] - P[1]) * extended_gcd(Q[0] - P[0], prime_modulus_p) % prime_modulus_p
        result.append((t ** 2 - P[0] - Q[0]) % prime_modulus_p)
        result.append((t * (P[0] - result[0]) - P[1]) % prime_modulus_p)
        return (result[0], result[1])

def point_doubling(P):
    result = []
    tmp = (3 * P[0] ** 2 + constant_a) * extended_gcd(2 * P[1], prime_modulus_p) % prime_modulus_p
    result.append((tmp ** 2 - 2 * P[0]) % prime_modulus_p)
    result.append((tmp * (P[0] - result[0]) - P[1]) % prime_modulus_p)
    return (result[0], result[1])

def point_multiplication(k, g):
    tmp = g
    result = 0
    k_binary = bin(k)[2:]
    k_length = len(k_binary)

    for i in reversed(range(k_length)):
        if k_binary[i] == '1':
            result = point_addition(result, tmp)
        tmp = point_doubling(tmp)

    return result

def signature_generation(secret_key, message):
    e = hash_function(message)
    k = secrets.randbelow(prime_modulus_p)  # Random number in the range (0, p)
    R = point_multiplication(k, base_point_G)  # R = k * G
    r = R[0] % prime_modulus_p
    s = extended_gcd(k, prime_modulus_n) * (e + r * secret_key) % prime_modulus_n

    return (r, s)  # Signature as a tuple

def deduce_public_key(signature, message):
    r = signature[0]
    s = signature[1]
    x = r % prime_modulus_p
    y = solve_quadratic_residue(((x ** 3) + 7), prime_modulus_p)
    e = hash_function(message)

    P1 = (x, y)
    P2 = (x, prime_modulus_p - y)
    sk1 = point_multiplication(s % prime_modulus_n, P1)
    tmp = point_multiplication(e % prime_modulus_n, base_point_G)
    tmp_i = (tmp[0], prime_modulus_p - tmp[1])
    tmp_1 = point_addition(sk1, tmp_i)
    pk1 = point_multiplication(extended_gcd(r, prime_modulus_n), tmp_1)

    sk2 = point_multiplication(s % prime_modulus_n, P2)
    tmp_2 = point_addition(sk2, tmp_i)
    pk2 = point_multiplication(extended_gcd(r, prime_modulus_n), tmp_2)
    return pk1, pk2

def hash_function(message):
    # Placeholder for the actual hash function implementation
    return hash(message)

if __name__ == '__main__':
    start_time = time.time()
    secret_key, public_key = generate_keypair()
    print("Public Key: \n", public_key)
    message = "hello,world!"
    signature = signature_generation(secret_key, message)
    print("\nSignature:\n ", signature)
    public_key1, public_key2 = deduce_public_key(signature, message)
    end_time = time.time()
    elapsed_time = end_time - start_time
    print(f"Elapsed Time: {elapsed_time:.6f} seconds")
    print('\nTry to deduce public key from signature:')
    print('\nPossible Public Key 1:\n', public_key1)
    print('\nPossible Public Key 2:\n', public_key2)

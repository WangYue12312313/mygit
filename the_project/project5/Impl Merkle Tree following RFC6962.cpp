#include <iostream>
#include <openssl/evp.h>
#include <iomanip>
#include <vector>
#include <sstream>
using namespace std;
int leaf = 100000;
string sha256(const string& str) {
    EVP_MD_CTX* mdctx;
    const EVP_MD* md;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, nullptr);
    EVP_DigestUpdate(mdctx, str.c_str(), str.size());
    EVP_DigestFinal_ex(mdctx, hash, &hashLen);
    EVP_MD_CTX_free(mdctx);

    stringstream ss;
    for (unsigned int i = 0; i < hashLen; ++i) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}
string merkleTree(int mount) {
    vector<string> data;
    vector<string> current_layer = data;
    vector<string> next_layer;
    data.resize(mount);

    for (int i = 0; i < mount; i += 1) {
        char tmp[32];
        tmp[0] = 0x00;
        snprintf(&tmp[1], sizeof(tmp) - 1, "%d", i);
        data.push_back(tmp);
        next_layer.push_back(sha256(data[i]));
    }

    current_layer = next_layer;
    next_layer.clear();



    while (current_layer.size() > 1) {
        for (size_t i = 0; i < current_layer.size() - 1; i += 2) {
            char tmp[65];
            tmp[0] = { 0x01 };
            memcpy(&tmp[1], data[i].c_str(), data[i].length());

            memcpy(&tmp[32], data[i + 1].c_str(), data[i + 1].length());
            next_layer.push_back(sha256(tmp));

        }
        current_layer = next_layer;
        next_layer.clear();
    }

    return current_layer[0];
}
int main() {
    time_t e, s;
    s = clock();
    string input = "Hello, World!";
    char a[2] = { 0x00,0x01 };
    string hash = sha256(a);

    string merkleRoot = merkleTree(leaf);
    cout << merkleRoot;
    e = clock();
    cout << endl <<"time:" << e - s << "ms";
    return 0;
}

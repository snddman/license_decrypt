#include <iostream>
#include <fstream>
#include <string>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/err.h>

using namespace std;

int main() {
    // read license file
    ifstream ifs("./vspdpro.act", ifstream::binary | ifstream::ate);
    if (!ifs)
    {
        cerr << "Failed to open license file vspdrpo.act." << endl;
        return 1;
    }

    streampos size = ifs.tellg();
    char* buffer = new char[size];
    ifs.seekg(0, ifstream::beg);
    ifs.read(buffer, size);
    ifs.close();

    // load public key
    string public_key = "-----BEGIN PUBLIC KEY-----\r\n" \
        "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwknN5/81qIoM+k1WEc/5\r\n" \
        "A+u3YzWeDNE5i5rpayiNWv4TvxX9udMz3uKF4tmDJfjuSCVUHQWAjJefwSv4ZBQ7\r\n" \
        "egXHtGzh/HVY/Geei7A1TwOroHxxgUKVkXRoPGYIVNBhrqX0CsCgYCZv7Ij6CSy4\r\n" \
        "Iamw0Gb/BnZ4Uw1AkOs6DfbGaVspeQGg7oTjSj+TKOWdK1dJ7ZoGyoVQLqxpaq41\r\n" \
        "r893XqSK5mXlbE3pFSjS6Rn8unyt57EuIcFy+x2VqjOf3KiwiGCeIaQtAtF5Pqii\r\n" \
        "KoEMoGgcDedO4C6h/LKy1oXJR7QeHQijUAn7JXE/i0I57BXyKylR6/YIV6hV+Ovm\r\n" \
        "bQIDAQAB\r\n" \
        "-----END PUBLIC KEY-----";

    BIO* bio = BIO_new_mem_buf(public_key.c_str(), public_key.size());
    if (!bio) {
        cerr << "Failed to create BIO for public key." << endl;
        delete[] buffer;
        return 1;
    }

    // read public key from BIO
    RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
    if (!rsa)
    {
        cerr << "Failed to read RSA public key." << endl;
        BIO_free(bio);
        delete[] buffer;
        return 1;
    }

    // print rsa key parameters
    cout << "RSA Key Parameters:" << endl;
    const BIGNUM* n, *e;
    RSA_get0_key(rsa, &n, &e, nullptr);
    cout << "Modulus (n): '" << BN_bn2hex(n) << "';" << endl;
    cout << "Exponent (e): '" << BN_bn2hex(e) << "';" << endl;
    cout << endl;

    // try to verify the license file
    unsigned char* from = (unsigned char*)buffer;
    unsigned char to[256];
    string txt = "";
    bool decrypt_sucess = true;

    cout << "Start encryption license file..." << endl;
    if (size % 256 != 0) {
        cerr << "License file size is not a multiple of 256 bytes." << endl;
        RSA_free(rsa);
        BIO_free(bio);
        delete[] buffer;
        return 1;
    } else {
        int total_decrypted = 0;
        while (total_decrypted < size) {
            int len = RSA_public_decrypt(256, from, to, rsa, RSA_PKCS1_PADDING);
            if (len < 0) {
                cerr << "RSA_public_decrypt failed: " << ERR_error_string(ERR_get_error(), nullptr) << endl;
                decrypt_sucess = false;
                break;
            }

            txt.append(string((char*)to, len));
            from += 256;
            total_decrypted += 256;
        }
    }
    if (!decrypt_sucess) {
        cout << "Failed to decrypt the license file." << endl;
        RSA_free(rsa);
        BIO_free(bio);
        delete[] buffer;
        return 1;
    }

    cout << "Decrypted license file content:" << endl;
    cout << "==========================================" << endl;
    cout << txt << endl;
    cout << "==========================================" << endl;

    // Parse license content
    cout << "Parsing license content..." << endl;
    size_t pos = 0;
    while (pos < txt.length()) {
        size_t eq_pos = txt.find('=', pos);
        if (eq_pos == string::npos) break;

        size_t next_pos = txt.find(' ', eq_pos);
        if (next_pos == string::npos) next_pos = txt.length();

        string key = txt.substr(pos, eq_pos - pos);
        string value = txt.substr(eq_pos + 1, next_pos - (eq_pos + 1));

        cout << key << " = " << value << endl;
        pos = next_pos;
        while (pos < txt.length() && txt[pos] == ' ') pos++;
    }

    // Clean resources
    delete[] buffer;
    RSA_free(rsa);
    BIO_free(bio);
    return 0;
}
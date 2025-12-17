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


    return 0;
}
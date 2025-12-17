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

    return 0;
}
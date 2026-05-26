#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

using namespace std;



vector<unsigned char> init_sbox(string key) {
    vector<unsigned char> s(256);
    iota(s.begin(), s.end(), 0);
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + s[i] + (unsigned char)key[i % key.length()]) % 256;
        swap(s[i], s[j]);
    }
    return s;
}


extern "C" {

    void rc4_crypt(string& data, string key) {
        if (key.empty()) return;

        vector<unsigned char> s = init_sbox(key);
        int i = 0, j = 0;

        for (char& byte : data) {
            i = (i + 1) % 256;
            j = (j + s[i]) % 256;
            swap(s[i], s[j]);
            unsigned char k = s[(s[i] + s[j]) % 256];
            byte ^= k;
        }
    }
}

#include <cstdint>
#include <cstddef>

uint8_t ModInverse(uint8_t a) {
    a = a % 256;
    for (int x = 1; x < 256; x++) {
        if ((a * x) % 256 == 1) {
            return static_cast<uint8_t>(x);
        }
    }
    return 1;
}

extern "C" {
    void ProcessData(uint8_t* data, size_t len, int key_a, int key_b, bool encrypt) {
        if (key_a % 2 == 0) {
            key_a += 1; 
        }

        if (encrypt) {
            for (size_t i = 0; i < len; i++) {
                data[i] = static_cast<uint8_t>((key_a * data[i] + key_b) % 256);
            }
        } else {
            uint8_t a_inv = ModInverse(static_cast<uint8_t>(key_a));
            for (size_t i = 0; i < len; i++) {
                int delta = data[i] - key_b;
                while (delta < 0) {
                    delta += 256;
                }
                data[i] = static_cast<uint8_t>((a_inv * delta) % 256);
            }
        }
    }
}
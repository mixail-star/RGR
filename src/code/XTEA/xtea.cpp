#include <cstdint>
#include <cstddef>

extern "C" {
    // Единая функция для шифрования и дешифрования
    void ProcessData(uint8_t* data, size_t len, const char* key_str, bool encrypt) {
        uint32_t k[4] = {0};
        // Превращаем строку ключа в 4 числа (uint32)
        for(int i = 0; i < 16 && key_str[i] != '\0'; i++) {
            ((char*)k)[i] = key_str[i];
        }

        uint32_t delta = 0x9E3779B9;
        
        for (size_t i = 0; i + 8 <= len; i += 8) {
            uint32_t* v = reinterpret_cast<uint32_t*>(&data[i]);
            uint32_t v0 = v[0], v1 = v[1], sum = 0;

            if (encrypt) {
                for (int j = 0; j < 32; j++) {
                    v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
                    sum += delta;
                    v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
                }
            } else {
                sum = delta * 32;
                for (int j = 0; j < 32; j++) {
                    v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
                    sum -= delta;
                    v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
                }
            }
            v[0] = v0; v[1] = v1;
        }
    }
}
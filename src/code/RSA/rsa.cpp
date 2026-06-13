#include <cstdint>
#include <cstddef>

uint64_t power_mod(uint64_t base, uint64_t power, uint64_t mod) {
    uint64_t res = 1;
    base %= mod;
    power %= (mod - 1);

    for (uint64_t i = 0; i < power; ++i) {
        res *= base;
        res %= mod;
    }
    return res;
}

extern "C" {
    size_t ProcessData(const uint8_t* in_data, size_t in_len, uint8_t* out_data, uint64_t key_e_or_d, uint64_t key_n, bool encrypt) {
        if (encrypt) {
            size_t out_idx = 0;
            for (size_t i = 0; i < in_len; i++) {
                uint64_t m = in_data[i];
                uint64_t c = power_mod(m, key_e_or_d, key_n);
                
                for (int b = 0; b < 8; ++b) {
                    out_data[out_idx++] = (c >> (b * 8)) & 0xFF;
                }
            }
            return out_idx;
        } else {
            size_t out_idx = 0;
            // Читаем по 8 байт, восстанавливаем исходный 1 байт
            for (size_t i = 0; i < in_len; i += 8) {
                uint64_t c = 0;
                for (int b = 0; b < 8; ++b) {
                    c |= (static_cast<uint64_t>(in_data[i + b]) << (b * 8));
                }
                
                uint64_t m = power_mod(c, key_e_or_d, key_n);
                out_data[out_idx++] = static_cast<uint8_t>(m & 0xFF);
            }
            return out_idx;
        }
    }
}




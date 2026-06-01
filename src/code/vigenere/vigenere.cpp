#include <cstdint>
#include <cstddef>

extern "C" {
    void vigenerShifr(uint8_t* data, size_t len, const char* key_str, bool encrypt) {
        
        if (data == nullptr || len == 0 || key_str == nullptr || *key_str == '\0') {
            return;
        }

        size_t key_len = 0;
        while (key_str[key_len] != '\0') {
            key_len++;
        }

        for (size_t i = 0; i < len; i++) {
            uint8_t shift = static_cast<uint8_t>(key_str[i % key_len]);
            if (encrypt) {
                data[i] = static_cast<uint8_t>(data[i] + shift);
            } else {
                data[i] = static_cast<uint8_t>(data[i] - shift);
            }
        }
    }
}

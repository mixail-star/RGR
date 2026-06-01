#include <cstdint>
#include <cstddef>

void cezar_process(const uint8_t* in_data, size_t in_len, uint8_t* out_data, int shift) {
    for (size_t i = 0; i < in_len; ++i) {
        out_data[i] = static_cast<uint8_t>((in_data[i] + shift + 256) % 256);
    }
}



extern "C" {
    size_t ProcessData(const uint8_t* in_data, size_t in_len, uint8_t* out_data, int key_shift, int cipher_type, bool encrypt) {
        if (cipher_type == 1) {
            
            int actual_shift = encrypt ? key_shift : -key_shift;
            cezar_process(in_data, in_len, out_data, actual_shift);
        
        return in_len;
    }
}
}




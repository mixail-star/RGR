#ifndef CIPHERS_H
#define CIPHERS_H

#include <cstdint>
#include <cstddef>
typedef size_t (*ProcessData_caesar_t)(const uint8_t* in_data, size_t in_len,
                                        uint8_t* out_data, int key_shift,
                                        int cipher_type, bool encrypt);

#endif // CIPHERS_H

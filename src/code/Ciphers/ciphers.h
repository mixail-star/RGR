#ifndef CIPHERS_H
#define CIPHERS_H

#include <cstdint>
#include <cstddef>

// Соответствует функции из ciphers.cpp:
// extern "C" size_t ProcessData(const uint8_t* in_data, size_t in_len,
//                                uint8_t* out_data, int key_shift,
//                                int cipher_type, bool encrypt);
//
// cipher_type = 1 -> шифр Цезаря (со сдвигом key_shift по модулю 256)
//
// out_data должен быть заранее выделен размером не меньше in_len.
typedef size_t (*ProcessData_caesar_t)(const uint8_t* in_data, size_t in_len,
                                        uint8_t* out_data, int key_shift,
                                        int cipher_type, bool encrypt);

#endif // CIPHERS_H

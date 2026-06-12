#ifndef VIGENERE_H
#define VIGENERE_H

#include <cstdint>
#include <cstddef>

// Соответствует функции из vigenere.cpp:
// extern "C" void vigenerShifr(uint8_t* data, size_t len,
//                               const char* key_str, bool encrypt);
//
// Шифрование/дешифрование выполняется НА МЕСТЕ (in-place) в буфере data.
// key_str - ключ (непустая C-строка).
typedef void (*vigenerShifr_t)(uint8_t* data, size_t len,
                                const char* key_str, bool encrypt);

#endif // VIGENERE_H

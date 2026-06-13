#ifndef VIGENERE_H
#define VIGENERE_H

#include <cstdint>
#include <cstddef>
typedef void (*vigenerShifr_t)(uint8_t* data, size_t len,
                                const char* key_str, bool encrypt);

#endif // VIGENERE_H

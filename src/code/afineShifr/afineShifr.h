#ifndef AFFINE_H
#define AFFINE_H

#include <cstdint>
#include <cstddef>

// Соответствует функции из afineShifr.cpp:
// extern "C" void ProcessData(uint8_t* data, size_t len,
//                              int key_a, int key_b, bool encrypt);
//
// Шифрование/дешифрование выполняется НА МЕСТЕ (in-place) в буфере data.
// key_a - должен быть взаимно прост с 256 (нечётным, библиотека сама
//         прибавляет 1 к чётному key_a).
typedef void (*ProcessData_affine_t)(uint8_t* data, size_t len,
                                      int key_a, int key_b, bool encrypt);

#endif // AFFINE_H

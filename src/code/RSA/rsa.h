#ifndef RSA_H
#define RSA_H

#include <cstdint>
#include <cstddef>

// Соответствует функции из rsa.cpp:
// extern "C" size_t ProcessData(const uint8_t* in_data, size_t in_len,
//                                uint8_t* out_data, uint64_t key_e_or_d,
//                                uint64_t key_n, bool encrypt);
//
// Шифрование: каждый входной байт превращается в 8 байт выходного буфера
//             (out_data должен быть размером >= in_len * 8).
// Дешифрование: каждые 8 байт входа превращаются в 1 байт выхода
//             (in_len должен быть кратен 8, out_data >= in_len / 8).
//
// key_e_or_d - открытая (e) или закрытая (d) экспонента
// key_n      - модуль n = p * q
typedef size_t (*ProcessData_rsa_t)(const uint8_t* in_data, size_t in_len,
                                     uint8_t* out_data, uint64_t key_e_or_d,
                                     uint64_t key_n, bool encrypt);

#endif // RSA_H

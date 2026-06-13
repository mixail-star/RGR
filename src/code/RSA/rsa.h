#ifndef RSA_H
#define RSA_H

#include <cstdint>
#include <cstddef>

typedef size_t (*ProcessData_rsa_t)(const uint8_t* in_data, size_t in_len,
                                     uint8_t* out_data, uint64_t key_e_or_d,
                                     uint64_t key_n, bool encrypt);

#endif // RSA_H

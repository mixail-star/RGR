#ifndef XTEA_H
#define XTEA_H

#include <cstdint>
#include <cstddef>
typedef void (*ProcessData_xtea_t)(uint8_t* data, size_t len,
                                    const char* key_str, bool encrypt);

#endif // XTEA_H

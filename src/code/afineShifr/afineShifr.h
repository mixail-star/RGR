#ifndef AFFINE_H
#define AFFINE_H

#include <cstdint>
#include <cstddef>
typedef void (*ProcessData_affine_t)(uint8_t* data, size_t len,
                                      int key_a, int key_b, bool encrypt);

#endif // AFFINE_H

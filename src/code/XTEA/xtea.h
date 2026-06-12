#ifndef XTEA_H
#define XTEA_H

#include <cstdint>
#include <cstddef>

// Соответствует функции из xtea.cpp:
// extern "C" void ProcessData(uint8_t* data, size_t len,
//                              const char* key_str, bool encrypt);
//
// Шифрование/дешифрование выполняется НА МЕСТЕ (in-place) в буфере data.
// ВНИМАНИЕ: функция обрабатывает данные блоками по 8 байт и обрабатывает
// только полные блоки (i + 8 <= len). Если len не кратен 8, "хвост"
// останется необработанным. Поэтому перед вызовом данные нужно дополнить
// (padding) до длины, кратной 8.
// key_str - ключ длиной до 16 байт (остальное игнорируется).
typedef void (*ProcessData_xtea_t)(uint8_t* data, size_t len,
                                    const char* key_str, bool encrypt);

#endif // XTEA_H

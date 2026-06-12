#ifndef RS4_H
#define RS4_H

#include <string>

// Соответствует функции из RS4.cpp:
// extern "C" void rc4_crypt(string& data, string key);
//
// Шифрование и дешифрование выполняются одной и той же функцией
// (RC4 - потоковый шифр, XOR с гамма-последовательностью), результат
// записывается на место исходной строки data (по ссылке).
// key - не должен быть пустым.
typedef void (*rc4_crypt_t)(std::string& data, std::string key);

#endif // RS4_H

#ifndef ATBASH_H
#define ATBASH_H

#include <cstddef>
#include <cwchar>

// Соответствует функции из Atbash.cpp:
// extern "C" void atbash_process(wchar_t* data, size_t length);
//
// Шифр Атбаш является симметричным (encrypt == decrypt - одна и та же
// операция). Обрабатывает печатные ASCII-символы (32-126) и русский
// алфавит (А-Я, а-я, Ё, ё), отражая их в пределах своего алфавита.
// Символы, не входящие в эти диапазоны, остаются без изменений.
//
// Работает с массивом wchar_t (кодовые точки Unicode), поэтому текст
// в кодировке UTF-8 нужно предварительно преобразовать в массив
// "широких" символов, а после обработки - обратно в UTF-8.
typedef void (*atbash_process_t)(wchar_t* data, size_t length);

#endif // ATBASH_H

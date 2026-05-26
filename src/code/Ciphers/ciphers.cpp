#include <cstdint>
#include <cstddef>

// Шифр Цезаря по всей таблице ASCII (индексы от 0 до 255)
void cezar_process(const uint8_t* in_data, size_t in_len, uint8_t* out_data, int shift) {
    for (size_t i = 0; i < in_len; ++i) {
        // Берем индекс символа (0-255), добавляем сдвиг, делаем +256 на случай отрицательного сдвига
        // и берем остаток от деления на 256, чтобы не выйти за пределы таблицы
        out_data[i] = static_cast<uint8_t>((in_data[i] + shift + 256) % 256);
    }
}

// Шифр Атбаш по всей таблице ASCII (зеркальное отражение индексов: 0 переходит в 255, 1 в 254 и т.д.)
void atbash_process(const uint8_t* in_data, size_t in_len, uint8_t* out_data) {
    for (size_t i = 0; i < in_len; ++i) {
        // Формула зеркала для диапазона 0-255: (255 - текущий_индекс)
        out_data[i] = static_cast<uint8_t>(255 - in_data[i]);
    }
}

extern "C" {
    size_t ProcessData(const uint8_t* in_data, size_t in_len, uint8_t* out_data, int key_shift, int cipher_type, bool encrypt) {
        if (cipher_type == 1) {
            // При расшифровке Цезаря просто инвертируем сдвиг
            int actual_shift = encrypt ? key_shift : -key_shift;
            cezar_process(in_data, in_len, out_data, actual_shift);
        } else if (cipher_type == 2) {
            atbash_process(in_data, in_len, out_data);
        }
        return in_len;
    }
}




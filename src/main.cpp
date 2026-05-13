#include <iostream>
#include <vector>
#include <fstream>
#include <dlfcn.h>
#include <cstdint>
#include <string>
#include <filesystem> // Для создания папок (C++17)

namespace fs = std::filesystem;

typedef void (*CipherFunc)(uint8_t*, size_t, const char*, bool);

// Помощник для сохранения файла в нужную папку
void save_to_result(const std::string& filename, const std::vector<uint8_t>& data, bool encrypt) {
    std::string dir_path = "result/XTEA";
    
    // Создаем папку, если её нет
    if (!fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }

    std::string prefix = encrypt ? "enc_" : "dec_";
    std::string ext = encrypt ? ".bin" : ".txt";
    std::string final_path = dir_path + "/" + prefix + filename + ext;

    std::ofstream outFile(final_path, std::ios::binary);
    if (outFile) {
        outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
        std::cout << "Файл сохранен: " << final_path << std::endl;
    } else {
        std::cerr << "Ошибка записи в " << final_path << std::endl;
    }
}

int main() {
    // 1. Загрузка библиотеки (путь согласно твоей структуре)
    void* handle = dlopen("./code/XTEA/libxtea.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Ошибка: не найдена библиотека по пути ./code/XTEA/libxtea.so\n" << dlerror() << std::endl;
        return 1;
    }

    CipherFunc xtea = (CipherFunc)dlsym(handle, "ProcessData");
    
    int choice, mode;
    std::string key, input_data, filename;

    std::cout << "--- XTEA Manager ---\n1. Текст\n2. Файл\nВыбор: ";
    std::cin >> choice;
    std::cout << "1. Зашифровать\n2. Расшифровать\nВыбор: ";
    std::cin >> mode;
    bool encrypt = (mode == 1);

    std::cout << "Ключ: ";
    std::cin >> key;
    if(key.length() < 16) key.resize(16, '0');

    std::vector<uint8_t> buffer;

    if (choice == 1) {
        std::cout << "Введите текст: ";
        std::cin.ignore();
        std::getline(std::cin, input_data);
        buffer.assign(input_data.begin(), input_data.end());
        filename = "manual_text";
    } else {
        std::cout << "Путь к файлу: ";
        std::cin >> filename;
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) { std::cerr << "Файл не найден!"; return 1; }
        buffer.assign(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());
        // Оставим только имя файла для сохранения
        filename = fs::path(filename).filename().string();
    }

    // Обработка Padding (выравнивание по 8 байт)
    if (encrypt) {
        while (buffer.size() % 8 != 0) buffer.push_back(0);
    }

    // Вызов шифра
    xtea(buffer.data(), buffer.size(), key.c_str(), encrypt);

    // Удаление нулей при расшифровке (чистим результат)
    if (!encrypt) {
        while (!buffer.empty() && buffer.back() == 0) buffer.pop_back();
    }

    // Сохранение в result/XTEA
    save_to_result(filename, buffer, encrypt);

    dlclose(handle);
    return 0;
}
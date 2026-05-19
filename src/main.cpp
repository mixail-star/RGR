#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <cstddef>

using namespace std;

extern "C" void vigenerShifr(uint8_t* data, size_t len, const char* key_str, bool encrypt);

uint8_t* readFile(const string& path, size_t& fileSize) {
    
    ifstream file(path, ios::binary | ios::ate);
    if (!file.is_open()) {
        cout << "Ошибка: Не удалось открыть файл для чтения: " << path << endl;
        fileSize = 0;
        return nullptr;
    }

    fileSize = file.tellg();
    file.seekg(0, ios::beg);

    uint8_t* buffer = new uint8_t[fileSize];
    file.read(reinterpret_cast<char*>(buffer), fileSize);
    file.close();

    return buffer;
}

bool writeFile(const string& path, uint8_t* data, size_t fileSize) {
    ofstream file(path, ios::binary);
    if (!file.is_open()) {
        cout << "Ошибка: Не удалось открыть файл для записи: " << path << endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(data), fileSize);
    file.close();
    return true;
}

int main() {
    string key;
    int sourceChoice, actionChoice;
    uint8_t* data = nullptr;
    size_t len = 0;

    cout << "1. Ввести текст вручную\n2. Прочитать из файла\nВыберите источник: ";
    cin >> sourceChoice;
    cin.ignore(); // Очистка буфера после ввода числа

    if (sourceChoice == 2) {
        string inputPath;
        cout << "Введите путь к исходному файлу: ";
        getline(cin, inputPath);
        
        data = readFile(inputPath, len);
        if (data == nullptr || len == 0) {
            return 1; // Ошибка открытия или файл пуст
        }
    } else {
        string inputText;
        cout << "Введите строку: ";
        getline(cin, inputText);

        if (inputText.empty()) {
            cout << "Ошибка: пустая строка!" << endl;
            return 1;
        }

        len = inputText.length();
        data = new uint8_t[len];
        for (size_t i = 0; i < len; i++) {
            data[i] = static_cast<uint8_t>(inputText[i]);
        }
    }

    // 2. Ввод ключа
    cout << "Введите ключ: ";
    getline(cin, key);

    if (key.empty()) {
        cout << "Ошибка: пустой ключ!" << endl;
        delete[] data;
        return 1;
    }

    // 3. Выбор действия
    cout << "1. Зашифровать\n2. Расшифровать\nВыберите действие: ";
    cin >> actionChoice;
    
    bool encrypt = (actionChoice == 1);

    // 4. Вызов функции обработки данных из .so библиотеки
    vigenerShifr(data, len, key.c_str(), encrypt);

    // 5. Сохранение или вывод результата
    int saveChoice;
    cout << "Сохранить результат в файл? (1 - Да, 0 - Вывести в консоль): ";
    cin >> saveChoice;
    cin.ignore();

    if (saveChoice == 1) {
        string outputPath;
        cout << "Введите имя для сохранения файла (например, end.txt): ";
        getline(cin, outputPath);
        outputPath = "result/vigener/" + outputPath;
        if (writeFile(outputPath, data, len)) {
            cout << "Успешно сохранено в: " << outputPath << endl;
        }
    } else {
        cout << "\nРезультат обработки: ";
        if (encrypt) {
            cout << "\n[Текст]: ";
            for (size_t i = 0; i < len; i++) cout << static_cast<char>(data[i]);
            
            cout << "\n[Байты (HEX)]: ";
            for (size_t i = 0; i < len; i++) printf("%02X ", data[i]);
            cout << endl;
        } else {
            for (size_t i = 0; i < len; i++) cout << static_cast<char>(data[i]);
            cout << endl;
        }
    }

    // Освобождаем динамическую память
    delete[] data;
    return 0;
}

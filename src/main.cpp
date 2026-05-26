#include <iostream>
#include <vector>
#include <fstream>
#include <dlfcn.h>
#include <cstdint>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Сигнатура функции из библиотеки (теперь без типа шифра, так как он один)
typedef size_t (*CezarFunc)(const uint8_t*, size_t, uint8_t*, int, bool);

void save_to_result(const string& filename, const vector<uint8_t>& data, bool encrypt) {
    string dir_path = "result/ Cezar";
    if (!fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }

    string prefix = encrypt ? "enc_" : "dec_";
    string ext = encrypt ? ".bin" : ".txt"; // Зашифрованное сохраняем в бинарник, расшифрованное в текст
    string final_path = dir_path + "/" + prefix + filename + ext;

    ofstream outFile(final_path, std::ios::binary);
    if (outFile) {
        outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
        cout << "\n[+] Файл успешно сохранен: " << final_path << endl;
    } else {
        cerr << "[!] Ошибка записи файла!" << endl;
    }
}

int main() {
    int source_choice, mode;
    string filename, input_data;
    vector<uint8_t> in_buffer;

    cout << "=== Шифр Цезаря (По таблице ASCII) ===\n";
    cout << "--- Источник ---\n1. Ввод с клавиатуры\n2. Чтение из файла\nВыбор: ";
    cin >> source_choice;

    cout << "--- Режим ---\n1. Зашифровать\n2. Расшифровать\nВыбор: ";
    cin >> mode;
    bool encrypt = (mode == 1);

    if (source_choice == 1) {
        cout << "Введите текст: ";
        cin.ignore();
        getline(cin, input_data);
        in_buffer.assign(input_data.begin(), input_data.end());
        filename = "manual_input";
    } else {
        cout << "Путь к файлу: ";
        cin >> filename;
        ifstream inFile(filename, std::ios::binary);
        if (!inFile) { 
            cerr << "Файл не найден!" << endl; 
            return 1; 
        }
        in_buffer.assign(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());
        filename = fs::path(filename).filename().string();
    }

    
    void* handle = dlopen("./src/code/Ciphers/libcezar.so", RTLD_LAZY);
    if (!handle) { 
        cerr << "Библиотека cezar не найдена! Проверьте путь ./src/code/Ciphers/libcezar.so" << endl; 
        return 1; 
    }
    CezarFunc process_cezar = (CezarFunc)dlsym(handle, "ProcessData");

    int key_shift = 0;
    cout << "Введите шаг (сдвиг 0-255) для шифра Цезаря: ";
    cin >> key_shift;
    key_shift %= 256; // Ограничиваем сдвиг размером таблицы ASCII

    vector<uint8_t> out_buffer(in_buffer.size());

    // Вызываем функцию шифрования
    process_cezar(in_buffer.data(), in_buffer.size(), out_buffer.data(), key_shift, encrypt);

    // Сохраняем в папку result/Cezar
    save_to_result(filename, out_buffer, encrypt);
    
    dlclose(handle);
    return 0;
}
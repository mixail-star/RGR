#include <iostream>
#include <vector>
#include <fstream>
#include <dlfcn.h>
#include <cstdint>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

typedef void (*AffineFunc)(uint8_t*, size_t, int, int, bool);

void save_to_result(const string& filename, const vector<uint8_t>& data, bool encrypt) {
    string dir_path = "result/Affine";
    if (!fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }
    //собираем имя файла
    string prefix = encrypt ? "enc_" : "dec_";
    string ext = encrypt ? ".bin" : ".txt";
    string final_path = dir_path + "/" + prefix + filename + ext;

    ofstream outFile(final_path, std::ios::binary);
    if (outFile) {
        outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
        cout << "Файл сохранен: " << final_path << endl;
    } else {
        cerr << "Ошибка записи!" << endl;
    }
}

int main() {
    int source_choice, mode;
    string filename, input_data;
    vector<uint8_t> buffer;

    cout << "--- Источник ---\n1. Ввод с клавиатуры\n2. Чтение из файла\nВыбор: ";
    cin >> source_choice;

    cout << "--- Режим ---\n1. Зашифровать\n2. Расшифровать\nВыбор: ";
    cin >> mode;
    bool encrypt = (mode == 1);

    if (source_choice == 1) {
        cout << "Введите данные: ";
        cin.ignore();
        getline(cin, input_data);
        buffer.assign(input_data.begin(), input_data.end());
        filename = "manual_input";
    } else {
        cout << "Путь к файлу: ";
        cin >> filename;
        ifstream inFile(filename, std::ios::binary);
        if (!inFile) { 
            cerr << "Файл не найден!" << endl; 
            return 1; 
        }
        buffer.assign(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());
        filename = fs::path(filename).filename().string();
    }

    void* handle = dlopen("./code/afineShifr/libaffine.so", RTLD_LAZY);
    if (!handle) { 
        cerr << "Либа Affine не найдена!" << endl; 
        return 1; 
    }
    AffineFunc affine = (AffineFunc)dlsym(handle, "ProcessData");

    int key_a, key_b;
    cout << "Ключ А (нечетное число): ";
    cin >> key_a;
    cout << "Ключ B: ";
    cin >> key_b;

    affine(buffer.data(), buffer.size(), key_a, key_b, encrypt);

    save_to_result(filename, buffer, encrypt);
    dlclose(handle);

    return 0;
}
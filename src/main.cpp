#include <iostream>
#include <vector>
#include <fstream>
#include <dlfcn.h>
#include <cstdint>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

const uint64_t KEY_N = 9173503; 

typedef size_t (*RsaFunc)(const uint8_t*, size_t, uint8_t*, uint64_t, uint64_t, bool);

void save_to_result(const string& filename, const vector<uint8_t>& data, bool encrypt) {
    string dir_path = "./result/RSA";
    if (!fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }

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
    vector<uint8_t> in_buffer;

    cout << "--- Источник ---\n1. Ввод с клавиатуры\n2. Чтение из файла\nВыбор: ";
    cin >> source_choice;

    cout << "--- Режим ---\n1. Зашифровать\n2. Расшифровать\nВыбор: ";
    cin >> mode;
    bool encrypt = (mode == 1);

    if (source_choice == 1) {
        cout << "Введите данные: ";
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

    void* handle = dlopen("./code/RSA/librsa.so", RTLD_LAZY);
    if (!handle) { 
        cerr << "Либа RSA не найдена!" << endl; 
        return 1; 
    }
    RsaFunc rsa = (RsaFunc)dlsym(handle, "ProcessData");

    uint64_t key_e = 0;
    uint64_t key_d = 0;
    uint64_t active_key = 0;

    if (encrypt) {
        cout << "Введите публичный ключ E: ";
        cin >> key_e;
        active_key = key_e;
    } else {
        cout << "Введите приватный ключ D: ";
        cin >> key_d;
        active_key = key_d;
    }
    
    
    cout << "Используется константный модуль N: " << KEY_N << endl;

    vector<uint8_t> out_buffer;
    if (encrypt) {
        out_buffer.resize(in_buffer.size() * 8);
    } else {
        out_buffer.resize(in_buffer.size() / 8);
    }


    size_t actual_out_len = rsa(in_buffer.data(), in_buffer.size(), out_buffer.data(), active_key, KEY_N, encrypt);
    out_buffer.resize(actual_out_len);

    save_to_result(filename, out_buffer, encrypt);
    dlclose(handle);

    return 0;
}
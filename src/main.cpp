#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <codecvt> 
#include <locale>
#include <dlfcn.h>

using namespace std;

typedef void (*atbash_func_t)(wchar_t*, size_t);

int main() {
    // 1. Загрузка библиотеки
    void* handle = dlopen("./code/Atbash/libatbash.so", RTLD_LAZY);
    if (!handle) {
        cerr << "Ошибка загрузки библиотеки: " << dlerror() << endl;
        return 1;
    }

    atbash_func_t atbash_process = (atbash_func_t)dlsym(handle, "atbash_process");
    if (!atbash_process) {
        cerr << "Ошибка поиска функции: " << dlerror() << endl;
        dlclose(handle);
        return 1;
    }

    // 2. Интерфейс
    int mode, input_source, output_dest;
    string file_content = "";

    cout << "=== Сквозной шифр Атбаш (Безопасный UTF-8) ===" << endl;
    cout << "Выберите действие:\n1. Зашифровать\n2. Расшифровать\nВаш выбор: ";
    cin >> mode;

    cout << "\nОткуда считать текст?\n1. Из консоли\n2. Из файла\nВаш выбор: ";
    cin >> input_source;
    cin.ignore();

    // 3. Считывание UTF-8 текста
    if (input_source == 1) {
        cout << "Введите строку: ";
        getline(cin, file_content);
    } else {
        string filename;
        cout << "Введите путь к файлу для чтения: ";
        getline(cin, filename);

        ifstream infile(filename);
        if (!infile.is_open()) {
            cerr << "Ошибка открытия файла!" << endl;
            dlclose(handle);
            return 1;
        }
        string line;
        while (getline(infile, line)) {
            file_content += line + "\n";
        }
        if (!file_content.empty()) file_content.pop_back();
        infile.close();
    }

    // 4. Конвертация UTF-8 в wstring
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    wstring wide_buffer = converter.from_bytes(file_content);

    // ИСПРАВЛЕНО: Передаем прямой указатель на массив символов строки
    if (!wide_buffer.empty()) {
        atbash_process(&wide_buffer[0], wide_buffer.size());
    }

    // Конвертация обратно в UTF-8
    string result_content = converter.to_bytes(wide_buffer);

    // 5. Сохранение / Вывод
    cout << "\nКуда вывести результат?\n1. В консоль\n2. В файл\nВаш выбор: ";
    cin >> output_dest;
    cin.ignore();

    if (output_dest == 1) {
        cout << "\nРезультат обработки:\n" << result_content << endl;
    } else {
        string filename;
        cout << "Введите путь к файлу для записи: ";
        getline(cin, filename);

        ofstream outfile(filename);
        if (!outfile.is_open()) {
            cerr << "Ошибка открытия файла для записи!" << endl;
            dlclose(handle);
            return 1;
        }
        outfile << result_content;
        outfile.close();
        cout << "Результат успешно сохранен." << endl;
    }

    dlclose(handle);
    return 0;
}

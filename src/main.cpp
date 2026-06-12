#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <dlfcn.h>

#include "code/Ciphers/ciphers.h"
#include "code/afineShifr/afineShifr.h"
#include "code/vigenere/vigenere.h"
#include "code/XTEA/xtea.h"
#include "code/RSA/rsa.h"
#include "code/RS4/RS4.h"
#include "code/Atbash/Atbash.h"
#include "code/Litoreya/litoreya.h"

using namespace std;

// Вспомогательные функции работы с буферами / файлами / консолью

static vector<uint8_t> readFile(const string& path)
{
    ifstream f(path, ios::binary);
    if (!f)
    {
        throw runtime_error("Не удалось открыть файл: " + path);
    }
    return vector<uint8_t>((istreambuf_iterator<char>(f)),
                            istreambuf_iterator<char>());
}


static void writeFile(const string& path, const vector<uint8_t>& data)
{
    ofstream f("result/" + path, ios::binary);
    if (!f)
    {
        throw runtime_error("Не удалось создать файл: " + path);
    }
    f.write(reinterpret_cast<const char*>(data.data()), static_cast<streamsize>(data.size()));
}


static vector<uint8_t> readConsole()
{
    cout << "Введите текст (одна строка):\n> ";
    string line;
    getline(cin, line);
    return vector<uint8_t>(line.begin(), line.end());
}

// Кусок кода для XTEA и RSA

static string toHex(const vector<uint8_t>& data)
{
    static const char* hexDigits = "0123456789ABCDEF";
    string out;
    out.reserve(data.size() * 2);
    for (uint8_t b : data)
    {
        out.push_back(hexDigits[b >> 4]);
        out.push_back(hexDigits[b & 0x0F]);
    }
    return out;
}

//Кусок кода для АТбаш
static vector<wchar_t> utf8ToCodepoints(const vector<uint8_t>& data)
{
    vector<wchar_t> out;
    size_t i = 0;
    while (i < data.size())
    {
        uint8_t c = data[i];
        uint32_t cp = 0;
        int extra = 0;
        if ((c & 0x80) == 0)        { cp = c;            extra = 0; }
        else if ((c & 0xE0) == 0xC0){ cp = c & 0x1F;     extra = 1; }
        else if ((c & 0xF0) == 0xE0){ cp = c & 0x0F;     extra = 2; }
        else if ((c & 0xF8) == 0xF0){ cp = c & 0x07;     extra = 3; }
        else { cp = c; extra = 0; } 

        if (i + extra >= data.size())
        {
            
            out.push_back((wchar_t)c);
            i++;
            continue;
        }

        for (int k = 1; k <= extra; ++k)
        {
            cp = (cp << 6) | (data[i + k] & 0x3F);
        }
        out.push_back((wchar_t)cp);
        i += (extra + 1);
    }
    return out;
}

static vector<uint8_t> codepointsToUtf8(const vector<wchar_t>& cps)
{
    vector<uint8_t> out;
    for (wchar_t wc : cps)
    {
        uint32_t cp = (uint32_t)wc;
        if (cp <= 0x7F)
        {
            out.push_back((uint8_t)cp);
        }
        else if (cp <= 0x7FF)
        {
            out.push_back((uint8_t)(0xC0 | (cp >> 6)));
            out.push_back((uint8_t)(0x80 | (cp & 0x3F)));
        }
        else if (cp <= 0xFFFF)
        {
            out.push_back((uint8_t)(0xE0 | (cp >> 12)));
            out.push_back((uint8_t)(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back((uint8_t)(0x80 | (cp & 0x3F)));
        }
        else
        {
            out.push_back((uint8_t)(0xF0 | (cp >> 18)));
            out.push_back((uint8_t)(0x80 | ((cp >> 12) & 0x3F)));
            out.push_back((uint8_t)(0x80 | ((cp >> 6) & 0x3F)));
            out.push_back((uint8_t)(0x80 | (cp & 0x3F)));
        }
    }
    return out;
}

// ---------------------------------------------------------------------
// Загрузка динамических библиотек
// ---------------------------------------------------------------------

//Перегрузка функций
template <typename T>
T loadSymbol(void* handle, const char* symbolName, const string& libPath)
{
    dlerror();
    void* sym = dlsym(handle, symbolName);
    const char* err = dlerror();
    if (err)
    {
        throw runtime_error("Не удалось найти символ '" + string(symbolName) +
                             "' в " + libPath + ": " + err);
    }
    return reinterpret_cast<T>(sym);
}

//загрузка библиотеки по заданному пути
void* openLib(const string& path)
{
    void* h = dlopen(path.c_str(), RTLD_NOW);
    if (!h)
    {
        throw runtime_error("Не удалось загрузить библиотеку " + path + ": " + dlerror());
    }
    return h;
}

// ---------------------------------------------------------------------
// Ввод / вывод данных пользователем
// ---------------------------------------------------------------------

static vector<uint8_t> getInputData()
{
    cout << "\nОткуда взять данные?\n"
            "  1 - ввести с консоли (текст)\n"
            "  2 - прочитать из файла\n> ";
    int choice;
    cin >> choice;
    cin.ignore(10000, '\n');

    if (choice == 2)
    {
        cout << "Введите путь к файлу: ";
        string path;
        getline(cin, path);
        return readFile(path);
    }
    return readConsole();
}


static void putOutputData(const vector<uint8_t>& data)
{
    cout << "\nКуда записать результат?\n"
            "  1 - вывести в консоль\n"
            "  2 - записать в файл\n> ";
    int choice;
    cin >> choice;
    cin.ignore(10000, '\n');

    if (choice == 2)
    {
        cout << "Введите путь к файлу: ";
        string path;
        getline(cin, path);
        writeFile(path, data);
        cout << "Результат (" << data.size() << " байт) записан в файл: " << path << "\n";
    }
    else
    {
        cout << "\n--- Результат (как текст, если возможно) ---\n";
        string text(data.begin(), data.end());
        cout << text << "\n";
        cout << "--- Результат (в HEX) ---\n";
        cout << toHex(data) << "\n";
    }
}


static bool askEncrypt()
{
    cout << "\nВыберите операцию:\n  1 - зашифровать\n  2 - расшифровать\n> ";
    int c;
    cin >> c;
    cin.ignore(10000, '\n');
    return c == 1;
}


// Реализации обработчиков для каждого шифра
static void runCaesar()
{
    void* h = openLib("./code/Ciphers/libciphers.so");
    auto ProcessData = loadSymbol<ProcessData_caesar_t>(h, "ProcessData", "libciphers.so");

    vector<uint8_t> in = getInputData();
    bool enc = askEncrypt();

    cout << "Введите сдвиг (ключ, целое число): ";
    int shift;
    cin >> shift;
    cin.ignore(10000, '\n');

    vector<uint8_t> out(in.size());
    size_t written = ProcessData(in.data(), in.size(), out.data(), shift, /*cipher_type=*/1, enc);
    out.resize(written);

    putOutputData(out);
    dlclose(h);
}

static void runAffine()
{
    void* h = openLib("./code/afineShifr/libaffine.so");
    auto ProcessData = loadSymbol<ProcessData_affine_t>(h, "ProcessData", "libaffine.so");

    vector<uint8_t> in = getInputData();
    bool enc = askEncrypt();

    cout << "Введите ключ a (целое число, рекомендуется нечётное, "
            "взаимно простое с 256): ";
    int a;
    cin >> a;
    cout << "Введите ключ b (целое число): ";
    int b;
    cin >> b;
    cin.ignore(10000, '\n');

    
    ProcessData(in.data(), in.size(), a, b, enc);

    putOutputData(in);
    dlclose(h);
}

static void runVigenere()
{
    void* h = openLib("./code/vigenere/libvigenere.so");
    auto vigenerShifr = loadSymbol<vigenerShifr_t>(h, "vigenerShifr", "libvigenere.so");

    vector<uint8_t> in = getInputData();
    bool enc = askEncrypt();

    cout << "Введите ключ (непустая строка): ";
    string key;
    getline(cin, key);

    vigenerShifr(in.data(), in.size(), key.c_str(), enc);

    putOutputData(in);
    dlclose(h);
}

static void runXtea()
{
    void* h = openLib("./code/XTEA/libxtea.so");
    auto ProcessData = loadSymbol<ProcessData_xtea_t>(h, "ProcessData", "libxtea.so");

    vector<uint8_t> in = getInputData();
    bool enc = askEncrypt();

    cout << "Введите ключ (до 16 символов): ";
    string key;
    getline(cin, key);

    
    size_t origLen = in.size();
    size_t padded = ((origLen + 7) / 8) * 8;
    if (padded == 0) padded = 8;
    in.resize(padded, 0);

    ProcessData(in.data(), in.size(), key.c_str(), enc);

    if (enc)
    {
        cout << "Внимание: исходные данные были дополнены нулевыми байтами до "
             << padded << " (с " << origLen << "), чтобы длина была кратна 8. "
             << "При расшифровке результат может содержать "
             << (padded - origLen) << " лишних нулевых байт в конце.\n";
    }

    putOutputData(in);
    dlclose(h);
}

static void runRsa()
{
    void* h = openLib("./code/RSA/librsa.so");
    auto ProcessData = loadSymbol<ProcessData_rsa_t>(h, "ProcessData", "librsa.so");

    vector<uint8_t> in = getInputData();
    bool enc = askEncrypt();

    cout << "Введите экспоненту (e для шифрования / d для дешифрования): ";
    uint64_t key_ed;
    cin >> key_ed;
    cout << "Введите модуль n: ";
    uint64_t key_n;
    cin >> key_n;
    cin.ignore(10000, '\n');

    vector<uint8_t> out;
    if (enc)
    {
        out.resize(in.size() * 8);
    }
    else
    {
        if (in.size() % 8 != 0)
        {
            cout << "Ошибка: для расшифровки RSA длина входных данных должна быть кратна 8 байтам. "
                 << "Текущая длина: " << in.size() << "\n";
            dlclose(h);
            return;
        }
        out.resize(in.size() / 8);
    }

    size_t written = ProcessData(in.data(), in.size(), out.data(), key_ed, key_n, enc);
    out.resize(written);

    putOutputData(out);
    dlclose(h);
}

static void runRS4()
{
    void* h = openLib("./code/RS4/libRS4.so");
    auto rc4_crypt = loadSymbol<rc4_crypt_t>(h, "rc4_crypt", "libRS4.so");

    vector<uint8_t> in = getInputData();
    
    askEncrypt();

    cout << "Введите ключ (непустая строка): ";
    string key;
    getline(cin, key);

    string data(in.begin(), in.end());
    rc4_crypt(data, key);

    vector<uint8_t> out(data.begin(), data.end());
    putOutputData(out);
    dlclose(h);
}


static void runAtbash()
{
    void* h = openLib("./code/Atbash/libatbash.so");
    auto atbash_process = loadSymbol<atbash_process_t>(h, "atbash_process", "libatbash.so");

    vector<uint8_t> in = getInputData();
    
    askEncrypt();

    vector<wchar_t> wide = utf8ToCodepoints(in);
    atbash_process(wide.data(), wide.size());
    vector<uint8_t> out = codepointsToUtf8(wide);

    putOutputData(out);
    dlclose(h);
}

static void runLitoreya()
{
    void* h = openLib("./code/Litoreya/liblitoreya.so");
    auto process_litoreya = loadSymbol<process_litoreya_t>(h, "process_litoreya", "liblitoreya.so");

    vector<uint8_t> in = getInputData();
    
    
    askEncrypt();

    string text(in.begin(), in.end());
    string result = process_litoreya(text);

    vector<uint8_t> out(result.begin(), result.end());
    putOutputData(out);
    dlclose(h);
}

// ---------------------------------------------------------------------
// main
// ---------------------------------------------------------------------

int main()
{
    while (true)
    {
        cout << "\n==================== Меню шифров ====================\n"
                " 1 - Шифр Цезаря\n"
                " 2 - Аффинный шифр\n"
                " 3 - Шифр Виженера\n"
                " 4 - XTEA\n"
                " 5 - RSA\n"
                " 6 - RC4 (RS4)\n"
                " 7 - Атбаш\n"
                " 8 - Литорея\n"
                " 0 - Выход\n"
                "> ";
        int choice;
        if (!(cin >> choice))
            break;
        cin.ignore(10000, '\n');

        try
        {
            switch (choice)
            {
                case 1: runCaesar();   break;
                case 2: runAffine();   break;
                case 3: runVigenere(); break;
                case 4: runXtea();     break;
                case 5: runRsa();      break;
                case 6: runRS4();      break;
                case 7: runAtbash();   break;
                case 8: runLitoreya(); break;
                case 0: return 0;
                default:
                    cout << "Неизвестный пункт меню.\n";
            }
        }
        catch (const exception& e)
        {
            cout << "Ошибка: " << e.what() << "\n";
        }
    }
    return 0;
}

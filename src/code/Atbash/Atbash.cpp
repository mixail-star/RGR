#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

bool is_initialized = false;
unordered_map<wchar_t, wchar_t> atbash_map;

void initialize_alphabet() {
    vector<wchar_t> alphabet;

    // Печатный ASCII (32 - 126)
    for (wchar_t c = 32; c <= 126; ++c) {
        alphabet.push_back(c);
    }

    // Русский алфавит
    alphabet.push_back(L'Ё');
    for (wchar_t c = L'А'; c <= L'Я'; ++c) {
        alphabet.push_back(c);
    }
    alphabet.push_back(L'ё');
    for (wchar_t c = L'а'; c <= L'я'; ++c) {
        alphabet.push_back(c);
    }

    size_t size = alphabet.size();
    for (size_t i = 0; i < size; ++i) {
        atbash_map[alphabet[i]] = alphabet[size - 1 - i];
    }

    is_initialized = true;
}

extern "C" {
    void atbash_process(wchar_t* data, size_t length) {
        if (!data) return;
        
        if (!is_initialized) {
            initialize_alphabet();
        }

        for (size_t i = 0; i < length; ++i) {
            auto it = atbash_map.find(data[i]);
            if (it != atbash_map.end()) {
                data[i] = it->second;
            }
        }
    }
}

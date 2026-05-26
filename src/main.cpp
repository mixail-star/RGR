#include <iostream>
#include <fstream>
#include <string>
#include "./code/RS4/RS4.cpp" 

using namespace std;

int main() {
    

    int choice;
    string key, text, inputPath, outputPath;

    cout << " RC4   Шифратор \n";
    cout << "1. Зашифровать / Расшифровать\n";
    cout << "Выберите действие: ";
    cin >> choice;
    

    cout << "Введите ключ  ";
    cin >> key;   
    

    cout << "Откуда взять текст? (1 - Ввести вручную, 2 - Считать из файла): ";
    int source;
    cin >> source;
    cin.ignore();

    if (source == 2) {
        cout << "Введите имя входного файла (например, ./result/RS4/end.txt): ";
        cin >> inputPath;
        cin.ignore();
        
        ifstream inFile(inputPath, ios::binary);
        if (!inFile) {
            cout << "Ошибка открытия файла!\n";
            return 1;
        }
        
        text.assign((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();
    } else {
        cout << "Введите сообщение: ";
        cin >> text;
        cin.ignore();
    }

    
    rc4_crypt(text, key);

    cout << "Куда вывести результат? (1 - В консоль, 2 - В файл): ";
    int outputChoice;
    cin >> outputChoice;
    cin.ignore();

    if (outputChoice == 2) {
        cout << "Введите имя файла для сохранения: ";
        cin >> outputPath;
        cin.ignore();
        
        ofstream outFile(outputPath, ios::binary);
        outFile << text;
        outFile.close();
        cout << "Готово! Результат сохранен в " << outputPath << "\n";
    } else {
        cout << "Результат: " << text << "\n";
    }

    return 0;
}

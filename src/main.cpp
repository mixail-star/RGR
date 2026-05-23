#include <iostream>
#include <string>
#include <fstream>

using namespace std;

extern "C" string process_litoreya(const string &text);

int main()
{
    int actionChoice = 0;
    int inputChoice = 0;
    string text = "";

    cout << "=== Программа шифрования Литорея (UTF-8) ===" << endl;
    cout << "Выберите действие:" << endl;
    cout << "1. Зашифровать" << endl;
    cout << "2. Расшифровать" << endl;
    cout << "Ваш выбор: ";
    cin >> actionChoice;
    cin.ignore();

    if (actionChoice != 1 && actionChoice != 2)
    {
        cout << "Ошибка: неверный выбор действия!" << endl;
        return 1;
    }

    cout << "\nВыберите способ ввода текста:" << endl;
    cout << "1. Ввести текст вручную в консоли" << endl;
    cout << "2. Прочитать из файла" << endl;
    cout << "Ваш выбор: ";
    cin >> inputChoice;
    cin.ignore();

    if (inputChoice == 1)
    {
        cout << "\nВведите текст: ";
        getline(cin, text);
    }
    else if (inputChoice == 2)
    {
        string inFilename;
        cout << "\nВведите имя файла для чтения (например, input.txt): ";
        getline(cin, inFilename);

        ifstream inFile(inFilename);
        if (!inFile.is_open())
        {
            cout << "Ошибка: невозможно открыть файл " << inFilename << endl;
            return 1;
        }

        string line;
        while (getline(inFile, line))
        {
            text += line + "\n";
        }
        if (!text.empty())
        {
            text.pop_back();
        }
        inFile.close();
        cout << "Текст успешно прочитан из файла!" << endl;
    }
    else
    {
        cout << "Ошибка: неверный способ ввода!" << endl;
        return 1;
    }

    string resultText = process_litoreya(text);

    if (inputChoice == 1)
    {
        cout << "\nРезультат обработки:" << endl;
        cout << resultText << endl;
    }

    int saveChoice = 1;
    if (inputChoice == 1)
    {
        cout << "\nХотите сохранить результат в файл?" << endl;
        cout << "1. Да\n2. Нет" << endl;
        cout << "Ваш выбор: ";
        cin >> saveChoice;
        cin.ignore();
    }

    if (saveChoice == 1)
    {
        string userOutFilename;
        cout << "\nВведите имя файла для сохранения (например, secret.txt): ";
        getline(cin, userOutFilename);

        string fullPath = "result/Litoreya/" + userOutFilename;
        ofstream outFile(fullPath);

        if (!outFile.is_open())
        {
            cout << "Ошибка при создании файла в папке result/Litoreya/. Проверьте, существует ли папка." << endl;
            return 1;
        }

        outFile << resultText;
        outFile.close();
        cout << "Результат успешно сохранен в: " << fullPath << endl;
    }

    return 0;
}

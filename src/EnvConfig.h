#ifndef ENV_CONFIG_H
#define ENV_CONFIG_H

#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>

using namespace std;

inline unordered_map<string, string> loadEnv(const string& path)
{
    unordered_map<string, string> result;
    ifstream f(path);
    if (!f)
    {
        return result; 
    }

    string line;
    while (getline(f, line))
    {
        size_t eq = line.find('=');
        
        string key = line.substr(0, eq);
        string value = line.substr(eq + 1);

        result[key] = value;
    }
    return result;
}

inline bool checkPassword(const string& envPath = ".env")
{
    auto env = loadEnv(envPath);

    auto it = env.find("APP_PASSWORD");
    if (it == env.end())
    {
        cout << "Внимание: файл " << envPath
                  << " не найден";
        return true;
    }

    const string& correct = it->second;

    for (int attempt = 1; attempt <= 3; ++attempt)
    {
        cout << "Введите пароль для доступа к программе: ";
        string entered;
        getline(cin, entered);

        if (entered == correct)
        {
            return true;
        }

        cout << "Неверный пароль. Осталось попыток: " << (3 - attempt) << "\n";
    }
    return false;
}

#endif 

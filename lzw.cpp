#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

// Функция для кодирования строки с использованием алгоритма LZW
vector<int> LZWEncode(const string& input) {
    // Инициализация словаря
    unordered_map<string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[string(1, char(i))] = i; // Заполнение словаря одиночными символами
    }

    string current = "";
    vector<int> result;
    int dictSize = 256; // Начальный размер словаря

    for (char c : input) {
        string combined = current + c;
        // Если комбинация существует в словаре
        if (dictionary.find(combined) != dictionary.end()) {
            current = combined; // Продолжаем строить текущую последовательность
        } else {
            // Добавляем код текущей последовательности в результат
            result.push_back(dictionary[current]);
            // Добавляем новую комбинацию в словарь
            dictionary[combined] = dictSize++;
            current = string(1, c); // Начинаем новую последовательность
        }
    }

    // Добавляем последний код в результат, если он не пустой
    if (!current.empty()) {
        result.push_back(dictionary[current]);
    }

    return result;
}

// Функция для декодирования LZW
string LZWDecode(const vector<int>& input) {
    unordered_map<int, string> dictionary;
    for (int i = 0; i < 256; ++i) {
        dictionary[i] = string(1, char(i)); // Заполнение словаря одиночными символами
    }

    string current = "";
    string result = "";
    int dictSize = 256;

    for (int code : input) {
        string entry;
        // Если код существует в словаре
        if (dictionary.find(code) != dictionary.end()) {
            entry = dictionary[code];
        } else if (code == dictSize) {
            entry = current + current[0]; // Специальный случай
        } else {
            throw invalid_argument("Invalid LZW code.");
        }

        result += entry; // Добавляем текущую строку к результату

        // Добавляем новую комбинацию в словарь
        if (!current.empty()) {
            dictionary[dictSize++] = current + entry[0];
        }

        current = entry; // Обновляем текущую строку
    }

    return result;
}

int main() {
    string inputData;
    cout << "Введите строку для сжатия: ";
    getline(cin, inputData);

    // Кодирование
    vector<int> encodedData = LZWEncode(inputData);
    
    cout << "Закодированные данные: ";
    for (int code : encodedData) {
        cout << code << " ";
    }
    cout << endl;

    // Декодирование
    string decodedData = LZWDecode(encodedData);
    
    cout << "Декодированные данные: " << decodedData << endl;

    return 0;
}
#include "modAlphaCipher.h"
#include <stdexcept>
#include <algorithm>
using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    // Проверка на пустой ключ
    if (skey.empty()) {
        throw invalid_argument("Ключ не может быть пустым");
    }
    
    // Заполняем ассоциативный массив alphaNum
    for (size_t i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    
    try {
        key = convert(skey);
    } catch (const exception& e) {
        throw invalid_argument("Неверный формат ключа: " + string(e.what()));
    }
    
    // Проверка, что ключ содержит только допустимые символы
    for (wchar_t c : skey) {
        if (alphaNum.find(c) == alphaNum.end()) {
            throw invalid_argument("Ключ содержит недопустимые символы");
        }
    }
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{
    // Проверка на пустой текст
    if (open_text.empty()) {
        throw invalid_argument("Текст для шифрования не может быть пустым");
    }
    
    try {
        vector<int> work = convert(open_text);
        for (size_t i = 0; i < work.size(); i++) {
            work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
        }
        return convert(work);
    } catch (const exception& e) {
        throw runtime_error("Ошибка при шифровании: " + string(e.what()));
    }
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    // Проверка на пустой текст
    if (cipher_text.empty()) {
        throw invalid_argument("Текст для расшифровки не может быть пустым");
    }
    
    try {
        vector<int> work = convert(cipher_text);
        for (size_t i = 0; i < work.size(); i++) {
            work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
        }
        return convert(work);
    } catch (const exception& e) {
        throw runtime_error("Ошибка при расшифровании: " + string(e.what()));
    }
}

vector<int> modAlphaCipher::convert(const wstring& s)
{
    vector<int> result;
    for (auto c : s) {
        if (alphaNum.find(c) == alphaNum.end()) {
            throw invalid_argument("Обнаружен недопустимый символ: '" + string(1, static_cast<char>(c)) + "'");
        }
        result.push_back(alphaNum[c]);
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{
    wstring result;
    for (auto i : v) {
        if (i < 0 || i >= static_cast<int>(numAlpha.size())) {
            throw out_of_range("Индекс символа вне допустимого диапазона: " + to_string(i));
        }
        result.push_back(numAlpha[i]);
    }
    return result;
}
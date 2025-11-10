#include "module.h"
#include <stdexcept>
#include <locale>

RCipher::RCipher(int key) {
    if (key <= 0) throw std::invalid_argument("Ключ должен быть > 0");
    columns = key;
}

std::wstring removeSpaces(const std::wstring& text) {
    std::wstring result;
    for (wchar_t c : text) {
        if (c != L' ') result += c;
    }
    return result;
}

std::wstring RCipher::encrypt(const std::wstring& text) {
    if (text.empty()) return L"";
    
    std::wstring clean_text = removeSpaces(text);
    if (clean_text.empty()) return L"";
    
    int rows = (clean_text.length() + columns - 1) / columns;
    std::wstring result;
    
    for (int col = columns - 1; col >= 0; col--) {
        for (int row = 0; row < rows; row++) {
            int pos = row * columns + col;
            if (pos < clean_text.length()) {
                result += clean_text[pos];
            }
        }
    }
    return result;
}

std::wstring RCipher::decrypt(const std::wstring& text) {
    if (text.empty()) return L"";
    
    std::wstring clean_text = removeSpaces(text);
    if (clean_text.empty()) return L"";
    
    int rows = (clean_text.length() + columns - 1) / columns;
    std::wstring result(clean_text.length(), L' ');
    int index = 0;

    for (int col = columns - 1; col >= 0; col--) {
        for (int row = 0; row < rows; row++) {
            int pos = row * columns + col;
            if (pos < clean_text.length() && index < clean_text.length()) {
                result[pos] = clean_text[index++];
            }
        }
    }
    return result;
}
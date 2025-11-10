#include <iostream>
#include "modAlphaCipher.h"
using namespace std;

void check(const wstring& Text, const wstring& key)
{ 
    wstring cipherText;
    wstring decryptedText;
    modAlphaCipher cipher(key);
    cipherText = cipher.encrypt(Text);
    decryptedText = cipher.decrypt(cipherText);
    wcout << L"key=" << key << endl;
    wcout << Text << endl;
    wcout << cipherText << endl;
    wcout << decryptedText << endl;
    if (Text == decryptedText)
        wcout << L"Ok\n";
    else
        wcout << L"Err\n";
}

int main(int argc, char **argv)
{ 
    setlocale(LC_ALL, "ru_RU.UTF-8");
    check(L"САМОЕСЭТИМСАМЫМ", L"ОДИН");
    check(L"САМОЕСЭТИМСАМЫМ", L"ДВА");
    check(L"САМОЕСЭТИМСАМЫМ", L"ТРИ");
    return 0;
}
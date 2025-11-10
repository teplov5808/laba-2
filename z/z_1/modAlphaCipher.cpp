#include "modAlphaCipher.h"
#include <iostream>
#include <algorithm>
using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{ 
    for (unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    
    if (skey.empty()) {
        throw invalid_argument("Пустой ключ");
    }
    
    key = convert(skey);
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{ 
    if (open_text.empty()) {
        throw invalid_argument("Пустой открытый текст");
    }
    
    vector<int> work = convert(open_text);
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{ 
    if (cipher_text.empty()) {
        throw invalid_argument("Пустой зашифрованный текст");
    }
    
    vector<int> work = convert(cipher_text);
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

vector<int> modAlphaCipher::convert(const wstring& s)
{ 
    vector<int> result;
    for(auto c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{ 
    wstring result;
    for(auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}
#ifndef TABLE_ROUTE_CIPHER_H
#define TABLE_ROUTE_CIPHER_H

#include <string>
#include <vector>
#include <stdexcept>

// Базовый класс для исключений шифра
class CipherException : public std::exception {
private:
    std::string message;
public:
    explicit CipherException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Исключение для неверного ключа
class InvalidKeyException : public CipherException {
public:
    explicit InvalidKeyException(const std::string& msg) : CipherException(msg) {}
};

// Исключение для неверного текста
class InvalidTextException : public CipherException {
public:
    explicit InvalidTextException(const std::string& msg) : CipherException(msg) {}
};

// Исключение для неанглийских символов
class NonEnglishTextException : public CipherException {
public:
    explicit NonEnglishTextException(const std::string& msg) : CipherException(msg) {}
};

// Исключение для ошибок шифрования/дешифрования
class CipherOperationException : public CipherException {
public:
    explicit CipherOperationException(const std::string& msg) : CipherException(msg) {}
};

class TableRouteCipher {
private:
    int columns;
    
    // Вспомогательные методы с проверками
    void validateKey() const;
    void validateText(const std::string& text, const std::string& operation) const;
    void validateCipherTextLength(int length) const;
    void validateEnglishText(const std::string& text) const;
    bool isEnglishLetter(char c) const;
    
public:
    TableRouteCipher(int key);
    std::string encrypt(const std::string& text);
    std::string decrypt(const std::string& cipher_text);
    
    // Геттер для ключа
    int getColumns() const { return columns; }
};

#endif
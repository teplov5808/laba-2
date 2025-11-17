#include "table_route_cipher.h"
#include <algorithm>
#include <cctype>

// Валидация ключа
void TableRouteCipher::validateKey() const {
    if (columns <= 0) {
        throw InvalidKeyException("Количество столбцов должно быть положительным числом");
    }
    if (columns > 1000) {
        throw InvalidKeyException("Количество столбцов слишком велико (максимум 1000)");
    }
}

// Проверка, является ли символ английской буквой
bool TableRouteCipher::isEnglishLetter(char c) const {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Проверка текста на наличие только английских букв
void TableRouteCipher::validateEnglishText(const std::string& text) const {
    for (char c : text) {
        if (c != ' ' && !isEnglishLetter(c)) {
            throw NonEnglishTextException(
                "Текст должен содержать только английские буквы и пробелы. "
                "Найден недопустимый символ: '" + std::string(1, c) + "'"
            );
        }
    }
}

// Валидация текста
void TableRouteCipher::validateText(const std::string& text, const std::string& operation) const {
    if (text.empty()) {
        throw InvalidTextException("Текст не может быть пустым для операции: " + operation);
    }
    
    // Проверяем, содержит ли текст хотя бы одну букву после удаления пробелов
    bool hasValidChars = false;
    for (char c : text) {
        if (c != ' ' && isEnglishLetter(c)) {
            hasValidChars = true;
            break;
        }
    }
    
    if (!hasValidChars) {
        throw InvalidTextException("Текст должен содержать английские буквы");
    }
    
    // Проверяем, что текст содержит только английские буквы и пробелы
    validateEnglishText(text);
}

// Валидация длины шифртекста
void TableRouteCipher::validateCipherTextLength(int length) const {
    if (length % columns != 0) {
        throw CipherOperationException(
            "Длина шифртекста (" + std::to_string(length) + 
            ") должна быть кратна количеству столбцов (" + std::to_string(columns) + ")"
        );
    }
}

TableRouteCipher::TableRouteCipher(int key) : columns(key) {
    validateKey();
}

std::string TableRouteCipher::encrypt(const std::string& text) {
    try {
        validateKey();
        validateText(text, "шифрование");
        
        // Обрабатываем текст: удаляем пробелы и преобразуем к верхнему регистру
        std::string processed_text;
        for (char c : text) {
            if (c != ' ') {
                if (isEnglishLetter(c)) {
                    processed_text += std::toupper(static_cast<unsigned char>(c));
                }
                // Не-буквенные символы уже отсеяны в validateEnglishText
            }
        }
        
        if (processed_text.empty()) {
            throw InvalidTextException("После удаления пробелов текст стал пустым");
        }
        
        int text_length = processed_text.length();
        int rows = (text_length + columns - 1) / columns;
        
        // Проверка на слишком большую таблицу
        if (rows > 10000) {
            throw CipherOperationException(
                "Слишком большая таблица для шифрования: " + 
                std::to_string(rows) + " строк × " + std::to_string(columns) + " столбцов"
            );
        }
        
        // Создаем таблицу и заполняем ее по маршруту записи
        std::vector<std::vector<char>> table(rows, std::vector<char>(columns, ' '));
        int index = 0;
        
        // Запись: слева направо, сверху вниз
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (index < text_length) {
                    table[i][j] = processed_text[index++];
                } else {
                    table[i][j] = 'X'; // Заполнитель
                }
            }
        }
        
        // Чтение: сверху вниз, справа налево
        std::string result;
        for (int j = columns - 1; j >= 0; j--) {
            for (int i = 0; i < rows; i++) {
                result += table[i][j];
            }
        }
        
        return result;
        
    } catch (const std::bad_alloc&) {
        throw CipherOperationException("Недостаточно памяти для создания таблицы шифрования");
    } catch (const std::exception& e) {
        // Перебрасываем уже известные исключения
        throw;
    }
}

std::string TableRouteCipher::decrypt(const std::string& cipher_text) {
    try {
        validateKey();
        validateText(cipher_text, "расшифрование");
        validateCipherTextLength(cipher_text.length());
        
        // Преобразуем шифртекст к верхнему регистру для consistency
        std::string processed_cipher_text;
        for (char c : cipher_text) {
            if (c != ' ') {
                processed_cipher_text += std::toupper(static_cast<unsigned char>(c));
            }
        }
        
        int cipher_length = processed_cipher_text.length();
        int rows = cipher_length / columns;
        
        // Проверка на слишком большую таблицу
        if (rows > 10000) {
            throw CipherOperationException(
                "Слишком большая таблица для расшифрования: " + 
                std::to_string(rows) + " строк × " + std::to_string(columns) + " столбцов"
            );
        }
        
        // Создаем таблицу для расшифровки
        std::vector<std::vector<char>> table(rows, std::vector<char>(columns, ' '));
        int index = 0;
        
        // Заполняем таблицу по маршруту считывания шифра
        for (int j = columns - 1; j >= 0; j--) {
            for (int i = 0; i < rows; i++) {
                if (index < cipher_length) {
                    table[i][j] = processed_cipher_text[index++];
                }
            }
        }
        
        // Читаем по маршруту записи: слева направо, сверху вниз
        std::string result;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                result += table[i][j];
            }
        }
        
        // Удаляем добавленные символы 'X' в конце
        while (!result.empty() && result.back() == 'X') {
            result.pop_back();
        }
        
        return result;
        
    } catch (const std::bad_alloc&) {
        throw CipherOperationException("Недостаточно памяти для создания таблицы расшифрования");
    } catch (const std::exception& e) {
        // Перебрасываем уже известные исключения
        throw;
    }
}
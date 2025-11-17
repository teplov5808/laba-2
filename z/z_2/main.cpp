#include <iostream>
#include <string>
#include <limits>
#include "table_route_cipher.h"

void displayMenu() {
    std::cout << "\n=== Шифр табличной маршрутной перестановки ===\n";
    std::cout << "1. Зашифровать текст\n";
    std::cout << "2. Расшифровать текст\n";
    std::cout << "3. Изменить ключ (количество столбцов)\n";
    std::cout << "4. Показать текущий ключ\n";
    std::cout << "5. Выход\n";
    std::cout << "Выберите действие: ";
}

int getIntegerInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод. Пожалуйста, введите число.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
    return value;
}

void handleCipherOperation(TableRouteCipher* cipher, bool isEncryption) {
    std::string text;
    std::string operationName = isEncryption ? "шифрования" : "расшифрования";
    
    std::cout << "Введите текст для " << operationName << ": ";
    std::getline(std::cin, text);
    
    try {
        std::string result;
        if (isEncryption) {
            result = cipher->encrypt(text);
        } else {
            result = cipher->decrypt(text);
        }
        
        std::cout << (isEncryption ? "Зашифрованный текст: " : "Расшифрованный текст: ") 
                  << result << std::endl;
                  
    } catch (const InvalidKeyException& e) {
        std::cout << "Ошибка ключа: " << e.what() << std::endl;
    } catch (const InvalidTextException& e) {
        std::cout << "Ошибка текста: " << e.what() << std::endl;
    } catch (const NonEnglishTextException& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    } catch (const CipherOperationException& e) {
        std::cout << "Ошибка операции: " << e.what() << std::endl;
    } catch (const CipherException& e) {
        std::cout << "Ошибка шифра: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Неизвестная ошибка: " << e.what() << std::endl;
    }
}

int main() {
    // Настройка локализации для поддержки русского языка
    setlocale(LC_ALL, "Russian");
    
    TableRouteCipher* cipher = nullptr;
    int key = 0;
    
    // Установка начального ключа
    while (cipher == nullptr) {
        key = getIntegerInput("Введите начальный ключ (количество столбцов): ");
        
        try {
            cipher = new TableRouteCipher(key);
            std::cout << "Ключ установлен: " << key << " столбцов\n";
        } catch (const InvalidKeyException& e) {
            std::cout << "Ошибка ключа: " << e.what() << "\n";
        } catch (const CipherException& e) {
            std::cout << "Ошибка шифра: " << e.what() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Неизвестная ошибка: " << e.what() << "\n";
        }
    }
    
    int choice;
    
    do {
        displayMenu();
        choice = getIntegerInput("");
        
        switch (choice) {
            case 1:
                handleCipherOperation(cipher, true);
                break;
                
            case 2:
                handleCipherOperation(cipher, false);
                break;
                
            case 3: {
                int new_key = getIntegerInput("Введите новый ключ (количество столбцов): ");
                
                try {
                    delete cipher;
                    cipher = new TableRouteCipher(new_key);
                    key = new_key;
                    std::cout << "Ключ успешно изменен на: " << key << " столбцов\n";
                } catch (const InvalidKeyException& e) {
                    std::cout << "Ошибка ключа: " << e.what() << "\n";
                    // Восстанавливаем старый шифр
                    cipher = new TableRouteCipher(key);
                } catch (const CipherException& e) {
                    std::cout << "Ошибка шифра: " << e.what() << "\n";
                    cipher = new TableRouteCipher(key);
                }
                break;
            }
            
            case 4:
                std::cout << "Текущий ключ: " << cipher->getColumns() << " столбцов\n";
                break;
                
            case 5:
                std::cout << "Выход из программы...\n";
                break;
                
            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
                break;
        }
    } while (choice != 5);
    
    delete cipher;
    return 0;
}
#include "hasher.h" 
#include <openssl/evp.h> 
#include <openssl/err.h> // для отладки
#include <fstream> 
#include <iomanip> //шестнадцатеричный формат
#include <sstream> 
#include <vector> 



namespace { // Открывает анонимное пространство имён для вспомогательных функций CRC32, чтобы они были локальны для этого файла
    // Таблица CRC32 (полином 0xEDB88320)
    uint32_t crc32_table[256]; // Объявляет массив для таблицы CRC32, используемой для ускорения вычислений
    bool crc32_table_initialized = false; 

    void init_crc32_table() { 
        for (uint32_t i = 0; i < 256; ++i) { // Перебирает все возможные байты (0–255)
            uint32_t crc = i; // Инициализирует crc текущим байтом
            for (int j = 0; j < 8; ++j) { // Выполняет 8 итераций для каждого бита
                if (crc & 1) { // Проверяет, установлен ли младший бит
                    crc = (crc >> 1) ^ 0xEDB88320; // Сдвигает вправо и применяет полином
                }
                else { // Если младший бит не установлен
                    crc >>= 1; // Просто сдвигает вправо
                } 
            } 
            crc32_table[i] = crc; 
        } 
        crc32_table_initialized = true; 
    } 

    // Вычисляет CRC32 для буфера
    uint32_t compute_crc32(const std::vector<unsigned char>& buffer) { 
        if (!crc32_table_initialized) { 
            init_crc32_table(); 
        } 
        uint32_t crc = 0xFFFFFFFF; // Инициализирует crc начальным значением (все биты 1)
        for (unsigned char byte : buffer) { 
            crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ byte]; 
        } 
        return crc ^ 0xFFFFFFFF; 
    } 
} 


std::string compute_hash(const std::string& file_path, HashAlgorithm algo) { 
    std::ifstream file(file_path, std::ios::binary); 
    if (!file.is_open()) { 
        return ""; 
    } 


    const size_t buffer_size = 4096; 
    std::vector<unsigned char> buffer(buffer_size); 



    EVP_MD_CTX* ctx = EVP_MD_CTX_new(); // Создаёт новый контекст EVP для хеширования
    if (!ctx) { // Проверяет, удалось ли создать контекст
        file.close();
        return ""; 
    } 

    const EVP_MD* md = nullptr; 
    if (algo == HashAlgorithm::MD5) { 
        md = EVP_md5(); // Устанавливает метод MD5
    }
    else if (algo == HashAlgorithm::SHA256) { 
        md = EVP_sha256(); // Устанавливает метод SHA-256
    }
    else if (algo == HashAlgorithm::SHA512) { 
        md = EVP_sha512(); // Устанавливает метод SHA-512
    } 

    if (md && EVP_DigestInit_ex(ctx, md, nullptr) != 1) { 
        EVP_MD_CTX_free(ctx); // Освобождает контекст при ошибке
        file.close(); 
        return ""; 
    } 



    while (file) { 
        file.read(reinterpret_cast<char*>(buffer.data()), buffer_size); 
        std::streamsize bytes_read = file.gcount(); // Получает количество прочитанных байтов
        if (bytes_read > 0) { // Проверяет, были ли прочитаны данные
            if (md && EVP_DigestUpdate(ctx, buffer.data(), bytes_read) != 1) { // Обновляет хеш
                EVP_MD_CTX_free(ctx); // Освобождает контекст при ошибке
                file.close(); 
                return ""; 
            } 
        } 
    } 


    std::string hash;
    if (md) { 
        unsigned char hash_value[EVP_MAX_MD_SIZE]; 
        unsigned int hash_length; 
        if (EVP_DigestFinal_ex(ctx, hash_value, &hash_length) != 1) { 
            EVP_MD_CTX_free(ctx); // Освобождает контекст
            file.close(); 
            return ""; 
        } 
        std::stringstream ss; 
        for (unsigned int i = 0; i < hash_length; ++i) { 
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash_value[i]; // Форматирует байт как два шестнадцатеричных символа
        } 
        hash = ss.str();
    } 


    else if (algo == HashAlgorithm::CRC32) { 
        uint32_t crc = 0; 
        file.clear(); // Сбрасывает флаги состояния файла
        file.seekg(0); // Возвращает указатель в начало файла
        while (file) { // Читает файл заново
            file.read(reinterpret_cast<char*>(buffer.data()), buffer_size); // Читает блок
            std::streamsize bytes_read = file.gcount(); // Получает байты
            if (bytes_read > 0) { // Проверяет данные
                buffer.resize(bytes_read); // Урезает буфер
                crc = compute_crc32(buffer); // Вычисляет CRC32
            } 
        } 
        std::stringstream ss; 
        ss << std::hex << std::setw(8) << std::setfill('0') << crc; // Форматирует CRC32 как 8-символьную строку
        hash = ss.str(); 
    } 


    // Освобождение ресурсов и закрытие файла
    EVP_MD_CTX_free(ctx); // Освобождает контекст EVP
    file.close(); 
    return hash; 
} 
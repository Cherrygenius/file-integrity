#include "dispatcher.h" // Подключает dispatcher.h для доступа к объявлению dispatch
#include "hasher.h" // Подключает hasher.h для доступа к compute_hash и HashAlgorithm
#include <iostream> // Подключает библиотеку для вывода сообщений в консоль

// Пустая строка для читаемости

int dispatch(const Arguments& args) { // Определяет функцию dispatch, принимающую Arguments и возвращающую код завершения
    // Диспетчеризация по режиму
    if (args.mode == "hash") { // Проверяет, является ли режим hash
        // Преобразует строковый алгоритм в HashAlgorithm
        HashAlgorithm algo; // Объявляет переменную для хранения алгоритма
        if (args.algo == "md5") { // Проверяет, указан ли md5
            algo = HashAlgorithm::MD5; // Устанавливает MD5
        }
        else if (args.algo == "sha256") { // Проверяет sha256
            algo = HashAlgorithm::SHA256; // Устанавливает SHA-256
        }
        else if (args.algo == "sha512") { // Проверяет sha512
            algo = HashAlgorithm::SHA512; // Устанавливает SHA-512
        }
        else if (args.algo == "crc32") { // Проверяет crc32
            algo = HashAlgorithm::CRC32; // Устанавливает CRC32
        }
        else { // Если алгоритм неизвестен (не должно произойти)
            std::cerr << "Ошибка: неизвестный алгоритм " << args.algo << std::endl; // Выводит ошибку
            return 1; // Возвращает код ошибки
        } // Завершает проверку алгоритма

        // Вычисляет хеш файла
        std::string hash = compute_hash(args.file, algo); // Вызывает compute_hash для файла и алгоритма
        if (hash.empty()) { // Проверяет, вернулся ли пустой хеш (ошибка)
            std::cerr << "Ошибка: не удалось вычислить хеш для файла " << args.file << std::endl; // Выводит ошибку
            return 1; // Возвращает код ошибки
        } // Завершает проверку

        // Выводит хеш в формате, совместимом с md5sum
        std::cout << hash << "  " << args.file << std::endl; // Выводит хеш и путь файла
        return 0; // Возвращает 0, указывая на успех
    }
    else if (args.mode == "compare") { // Проверяет режим compare
        std::cout << "Режим compare: сравнение файлов " << args.file // Выводит сообщение (заглушка)
            << " и " << args.file2 << " с алгоритмом " << args.algo << std::endl; // Добавляет детали
        // TODO: Вызвать функцию сравнения файлов
        return 0; // Возвращает 0 (заглушка)
    }
    else if (args.mode == "scan") { // Проверяет режим scan
        std::cout << "Режим scan: сканирование директории " << args.dir // Выводит сообщение (заглушка)
            << " с алгоритмом " << args.algo; // Добавляет алгоритм
        if (!args.output.empty()) { // Проверяет, указан ли --output
            std::cout << ", сохранение в " << args.output; // Добавляет информацию
        } // Завершает проверку
        std::cout << std::endl; // Добавляет перевод строки
        // TODO: Вызвать функцию сканирования
        return 0; // Возвращает 0 (заглушка)
    }
    else if (args.mode == "check") { // Проверяет режим check
        std::cout << "Режим check: проверка изменений в директории " << args.dir // Выводит сообщение (заглушка)
            << " с файлом хешей " << args.hash_file // Добавляет файл хешей
            << " и алгоритмом " << args.algo << std::endl; // Добавляет алгоритм
        // TODO: Вызвать функцию проверки изменений
        return 0; // Возвращает 0 (заглушка)
    } // Завершает блок условий

    // На случай ошибки (не должно произойти, так как режим проверяется в parse_arguments)
    std::cerr << "Ошибка: неизвестный режим " << args.mode << std::endl; // Выводит ошибку
    return 1; // Возвращает код ошибки
} // Завершает функцию dispatch
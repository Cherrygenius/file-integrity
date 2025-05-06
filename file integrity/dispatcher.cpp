#include "dispatcher.h" 
#include "hasher.h" 
#include <iostream> 
#include <filesystem> // Подключает библиотеку для работы с файловой системой (рекурсивный обход директории)
#include <fstream> // для записи в файл
#include <map> // для хранения хешей
#include <sstream> // для работы со строками


namespace fs = std::filesystem;


int dispatch(const Arguments& args) { 

    if (args.mode == "hash") { 
       
        HashAlgorithm algo;  
        if (args.algo == "md5") { 
            algo = HashAlgorithm::MD5; // Устанавливает MD5
        }
        else if (args.algo == "sha256") { 
            algo = HashAlgorithm::SHA256; // Устанавливает SHA-256
        }
        else if (args.algo == "sha512") { 
            algo = HashAlgorithm::SHA512; // Устанавливает SHA-512
        }
        else if (args.algo == "crc32") { 
            algo = HashAlgorithm::CRC32; // Устанавливает CRC32
        }
        else { // Если алгоритм неизвестен (не должно произойти)
            std::cerr << "Ошибка: неизвестный алгоритм " << args.algo << std::endl; 
            return 1; 
        } 

        // Вычисляет хеш файла
        std::string hash = compute_hash(args.file, algo); 
        if (hash.empty()) { 
            std::cerr << "Ошибка: не удалось вычислить хеш для файла " << args.file << std::endl; 
            return 1; 
        } 

        
        std::cout << hash << "  " << args.file << std::endl; 
        return 0; 
    }
    else if (args.mode == "compare") { 
		HashAlgorithm algo;
		if (args.algo == "md5") { 
			algo = HashAlgorithm::MD5; 
		}
		else if (args.algo == "sha256") { 
			algo = HashAlgorithm::SHA256; 
		}
		else if (args.algo == "sha512") { 
			algo = HashAlgorithm::SHA512; 
		}
		else if (args.algo == "crc32") { 
			algo = HashAlgorithm::CRC32; 
		}
		else { // Если алгоритм неизвестен (не должно произойти)
			std::cerr << "Ошибка: неизвестный алгоритм " << args.algo << std::endl; 
			return 1; 
		} 
		std::string hash1 = compute_hash(args.file, algo); 
		std::string hash2 = compute_hash(args.file2, algo); 
		if (hash1.empty() || hash2.empty()) { // Проверяет, вернулся ли пустой хеш (ошибка)
			std::cerr << "Ошибка: не удалось вычислить хеш для одного из файлов" << std::endl; 
			return 1; 
		} 
		if (hash1 == hash2) { 
			std::cout << "Файлы идентичны" << std::endl; 
            std::cout << "Хеш: " << hash1 << std::endl;
		}
		else {
			std::cout << "Файлы различаются" << std::endl;
            std::cout << "Хеш 1: " << hash1 << std::endl;
            std::cout << "Хеш 2: " << hash2 << std::endl;
		} 
		return 0; 
    }
	else if (args.mode == "scan") { // Проверяет режим scan
		HashAlgorithm algo;
		if (args.algo == "md5") { // Устанавливает MD5
			algo = HashAlgorithm::MD5;
		}
		else if (args.algo == "sha256") { // Устанавливает SHA-256
			algo = HashAlgorithm::SHA256;
		}
		else if (args.algo == "sha512") { // Устанавливает SHA-512
			algo = HashAlgorithm::SHA512;
		}
		else if (args.algo == "crc32") { // Устанавливает CRC32
			algo = HashAlgorithm::CRC32;
		}
		else { // Если алгоритм неизвестен (не должно произойти)
			std::cerr << "Ошибка: неизвестный алгоритм " << args.algo << std::endl;
			return 1;
		}
		if (!fs::exists(args.dir) || !fs::is_directory(args.dir)) { // Проверяет, существует ли директория
			std::cerr << "Ошибка: директория " << args.dir << " не существует или не является директорией" << std::endl;
			return 1;
		}
		std::ofstream out_file; // Открывает файл для записи
		if (!args.output.empty()) {
			out_file.open(args.output, std::ios::out | std::ios::trunc);
			if (!out_file.is_open()) { // Проверяет, открылся ли файл
				std::cerr << "Ошибка: не удалось открыть файл " << args.output << " для записи" << std::endl;
				return 1;
			}
		} // Проверяет, открылся ли файл

		for (const auto& entry : fs::recursive_directory_iterator(args.dir)) { // Рекурсивно обходит директорию
			if (entry.is_regular_file()) { // Проверяет, является ли элемент файлом
				std::string file_path = entry.path().string(); // Получает путь к файлу
				std::string hash = compute_hash(file_path, algo); // Вычисляет хеш файла
				if (hash.empty()) {
					std::cerr << "Ошибка: не удалось вычислить хеш для файла " << file_path << std::endl; // Выводит ошибку
					continue; // Пропускает файл
				}

				std::string output_line = hash + "  " + file_path; // Форматирует строку для вывода

				if (out_file.is_open()) {
					out_file << output_line << std::endl; // Записывает строку в файл
				}
				else {
					std::cout << output_line << std::endl; // Выводит строку на экран
				}
			}
		}

		if (out_file.is_open()) { // Проверяет, открылся ли файл
			out_file.close(); // Закрывает файл
		}
		return 0; 
	}
    else if (args.mode == "check") { // Проверяет режим "check"
        // Преобразует строковый алгоритм в перечисление HashAlgorithm
        HashAlgorithm algo; // Объявляет переменную для хранения алгоритма
        if (args.algo == "md5") { // Проверяет, указан ли алгоритм "md5"
            algo = HashAlgorithm::MD5; // Устанавливает значение MD5
        }
        else if (args.algo == "sha256") { // Проверяет, указан ли "sha256"
            algo = HashAlgorithm::SHA256; // Устанавливает значение SHA-256
        }
        else if (args.algo == "sha512") { // Проверяет, указан ли "sha512"
            algo = HashAlgorithm::SHA512; // Устанавливает значение SHA-512
        }
        else if (args.algo == "crc32") { // Проверяет, указан ли "crc32"
            algo = HashAlgorithm::CRC32; // Устанавливает значение CRC32
        }
        else { // Обрабатывает случай неизвестного алгоритма
            std::cerr << "Ошибка: неизвестный алгоритм " << args.algo << std::endl;
            return 1;
        }

        // Проверяет, существует ли директория
        if (!fs::exists(args.dir) || !fs::is_directory(args.dir)) {
            std::cerr << "Ошибка: директория " << args.dir << " не существует или не является директорией" << std::endl;
            return 1;
        }

        // Открывает файл с сохранёнными хешами
        std::ifstream hash_file(args.hash_file);
        if (!hash_file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл с хешами " << args.hash_file << std::endl;
            return 1;
        }

        // Читает сохранённые хеши в map: путь -> хеш
        std::map<std::string, std::string> saved_hashes;
        std::string line;
        while (std::getline(hash_file, line)) {
            std::istringstream iss(line);
            std::string hash, path;
            iss >> hash; // Читает хеш (первое слово)
            std::getline(iss >> std::ws, path); // Читает остаток строки (путь), пропуская пробелы
            if (!hash.empty() && !path.empty()) {
                saved_hashes[path] = hash;
            }
        }
        hash_file.close();

        // Собирает текущие хеши файлов в директории
        std::map<std::string, std::string> current_hashes;
        for (const auto& entry : fs::recursive_directory_iterator(args.dir)) {
            if (entry.is_regular_file()) {
                std::string file_path = entry.path().string();
                std::string hash = compute_hash(file_path, algo);
                if (hash.empty()) {
                    std::cerr << "Предупреждение: не удалось вычислить хеш для файла " << file_path << std::endl;
                    continue;
                }
                current_hashes[file_path] = hash;
            }
        }

        // Сравнивает хеши и выводит изменения
        bool has_changes = false;

        // Проверяет изменённые и удалённые файлы
        for (const auto& [path, saved_hash] : saved_hashes) {
            auto it = current_hashes.find(path);
            if (it == current_hashes.end()) {
                std::cout << "Удалён: " << path << std::endl;
                std::cout << std::endl;
                has_changes = true;
            }
            else if (it->second != saved_hash) {
                std::cout << "Изменён: " << path << std::endl;
                std::cout << "  Старый хеш: " << saved_hash << std::endl;
                std::cout << "  Новый хеш: " << it->second << std::endl;
                std::cout << std::endl;
                has_changes = true;
            }
        }

        // Проверяет добавленные файлы
        for (const auto& [path, hash] : current_hashes) {
            if (saved_hashes.find(path) == saved_hashes.end()) {
                std::cout << "Добавлен: " << path << std::endl;
                std::cout << "  Хеш: " << hash << std::endl;
                std::cout << std::endl;
                has_changes = true;
            }
        }

        // Если изменений нет, выводит сообщение
        if (!has_changes) {
            std::cout << "Изменений не обнаружено в директории " << args.dir << std::endl;
            std::cout << std::endl;
        }

        return 0; // Успешное завершение
        } // Завершает блок условий

        // Обработка случая неизвестного режима (не должно происходить из-за проверки в parse_arguments)
        std::cerr << "Ошибка: неизвестный режим " << args.mode << std::endl; // Выводит сообщение об ошибке
        return 1; // Возвращает код ошибки
} // Завершает определение функции dispatch
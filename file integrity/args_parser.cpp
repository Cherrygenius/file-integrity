#include "args_parser.h"  
#include <iostream>  
#include <argparse/argparse.hpp>  

bool parse_arguments(int argc, char* argv[], Arguments& args) {
    argparse::ArgumentParser program("filecheck", "1.0");

    program.add_argument("mode")
        .help("Режим работы: hash, compare, scan, check")
        .action([](const std::string& value) {
        if (value != "hash" && value != "compare" && value != "scan" && value != "check") {
            throw std::runtime_error("Недопустимый режим: " + value + ". Допустимые режимы: hash, compare, scan, check.");
        }
        return value;
            });


    program.add_argument("--file")
        .help("Путь к файлу для хеширования или сравнения");
    program.add_argument("--file2")
        .help("Путь к второму файлу для сравнения");
    program.add_argument("--dir")
        .help("Путь к директории для сканирования");
    program.add_argument("--algo")
        .default_value("sha256")
        .help("Алгоритм хеширования: md5, sha1, sha256");
    program.add_argument("--output")
        .help("Путь к файлу для сохранения хешей (для scan)");
    program.add_argument("--hash_file")
        .help("Путь к файлу с хешами для проверки(для check)");


    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << "Ошибка: " << err.what() << std::endl;
        std::cerr << program;
        return false;
    }

    args.mode = program.get<std::string>("mode");
    args.algo = program.get<std::string>("algo");
    if (program.present("--file")) args.file = program.get<std::string>("--file");
    if (program.present("--file2")) args.file2 = program.get<std::string>("--file2");
    if (program.present("--dir")) args.dir = program.get<std::string>("--dir");
    if (program.present("--output")) args.output = program.get<std::string>("--output");
    if (program.present("--hash_file")) args.hash_file = program.get<std::string>("--hash_file");


    if (args.mode == "hash") {
        if (args.file.empty() || args.algo.empty()) {
            std::cerr << "Ошибка: Укажите --file и --algo для режима hash" << std::endl;
            std::cerr << program;
            return false;
        }
    }
    else if (args.mode == "compare") {
        if (args.file.empty() || args.file2.empty()) {
            std::cerr << "Ошибка: Укажите --file и --file2 для режима compare" << std::endl;
            std::cerr << program;
            return false;
        }
    }
    else if (args.mode == "scan") {
        if (args.dir.empty()) {
            std::cerr << "Ошибка: Укажите --dir для режима scan" << std::endl;
            std::cerr << program;
            return false;
        }
    }
    else if (args.mode == "check") {
        if (args.dir.empty() || args.hash_file.empty()) {
            std::cerr << "Ошибка: Укажите --dir и --hash-file для режима check" << std::endl;
            std::cerr << program;
            return false;
        }
    }


    if (args.algo != "md5" && args.algo != "sha256" && args.algo != "sha512" && args.algo != "crc32") {
        std::cerr << "Ошибка: Неверный алгоритм: " << args.algo << ". Допустимые: md5, sha256, sha512, crc32." << std::endl;
        std::cerr << program;
        return false;
    }
}
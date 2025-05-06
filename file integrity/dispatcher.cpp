#include "dispatcher.h" 
#include "hasher.h" 
#include <iostream> 
#include <filesystem> // ���������� ���������� ��� ������ � �������� �������� (����������� ����� ����������)
#include <fstream> // ��� ������ � ����
#include <map> // ��� �������� �����
#include <sstream> // ��� ������ �� ��������


namespace fs = std::filesystem;


int dispatch(const Arguments& args) { 

    if (args.mode == "hash") { 
       
        HashAlgorithm algo;  
        if (args.algo == "md5") { 
            algo = HashAlgorithm::MD5; // ������������� MD5
        }
        else if (args.algo == "sha256") { 
            algo = HashAlgorithm::SHA256; // ������������� SHA-256
        }
        else if (args.algo == "sha512") { 
            algo = HashAlgorithm::SHA512; // ������������� SHA-512
        }
        else if (args.algo == "crc32") { 
            algo = HashAlgorithm::CRC32; // ������������� CRC32
        }
        else { // ���� �������� ���������� (�� ������ ���������)
            std::cerr << "������: ����������� �������� " << args.algo << std::endl; 
            return 1; 
        } 

        // ��������� ��� �����
        std::string hash = compute_hash(args.file, algo); 
        if (hash.empty()) { 
            std::cerr << "������: �� ������� ��������� ��� ��� ����� " << args.file << std::endl; 
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
		else { // ���� �������� ���������� (�� ������ ���������)
			std::cerr << "������: ����������� �������� " << args.algo << std::endl; 
			return 1; 
		} 
		std::string hash1 = compute_hash(args.file, algo); 
		std::string hash2 = compute_hash(args.file2, algo); 
		if (hash1.empty() || hash2.empty()) { // ���������, �������� �� ������ ��� (������)
			std::cerr << "������: �� ������� ��������� ��� ��� ������ �� ������" << std::endl; 
			return 1; 
		} 
		if (hash1 == hash2) { 
			std::cout << "����� ���������" << std::endl; 
            std::cout << "���: " << hash1 << std::endl;
		}
		else {
			std::cout << "����� �����������" << std::endl;
            std::cout << "��� 1: " << hash1 << std::endl;
            std::cout << "��� 2: " << hash2 << std::endl;
		} 
		return 0; 
    }
	else if (args.mode == "scan") { // ��������� ����� scan
		HashAlgorithm algo;
		if (args.algo == "md5") { // ������������� MD5
			algo = HashAlgorithm::MD5;
		}
		else if (args.algo == "sha256") { // ������������� SHA-256
			algo = HashAlgorithm::SHA256;
		}
		else if (args.algo == "sha512") { // ������������� SHA-512
			algo = HashAlgorithm::SHA512;
		}
		else if (args.algo == "crc32") { // ������������� CRC32
			algo = HashAlgorithm::CRC32;
		}
		else { // ���� �������� ���������� (�� ������ ���������)
			std::cerr << "������: ����������� �������� " << args.algo << std::endl;
			return 1;
		}
		if (!fs::exists(args.dir) || !fs::is_directory(args.dir)) { // ���������, ���������� �� ����������
			std::cerr << "������: ���������� " << args.dir << " �� ���������� ��� �� �������� �����������" << std::endl;
			return 1;
		}
		std::ofstream out_file; // ��������� ���� ��� ������
		if (!args.output.empty()) {
			out_file.open(args.output, std::ios::out | std::ios::trunc);
			if (!out_file.is_open()) { // ���������, �������� �� ����
				std::cerr << "������: �� ������� ������� ���� " << args.output << " ��� ������" << std::endl;
				return 1;
			}
		} // ���������, �������� �� ����

		for (const auto& entry : fs::recursive_directory_iterator(args.dir)) { // ���������� ������� ����������
			if (entry.is_regular_file()) { // ���������, �������� �� ������� ������
				std::string file_path = entry.path().string(); // �������� ���� � �����
				std::string hash = compute_hash(file_path, algo); // ��������� ��� �����
				if (hash.empty()) {
					std::cerr << "������: �� ������� ��������� ��� ��� ����� " << file_path << std::endl; // ������� ������
					continue; // ���������� ����
				}

				std::string output_line = hash + "  " + file_path; // ����������� ������ ��� ������

				if (out_file.is_open()) {
					out_file << output_line << std::endl; // ���������� ������ � ����
				}
				else {
					std::cout << output_line << std::endl; // ������� ������ �� �����
				}
			}
		}

		if (out_file.is_open()) { // ���������, �������� �� ����
			out_file.close(); // ��������� ����
		}
		return 0; 
	}
    else if (args.mode == "check") { // ��������� ����� "check"
        // ����������� ��������� �������� � ������������ HashAlgorithm
        HashAlgorithm algo; // ��������� ���������� ��� �������� ���������
        if (args.algo == "md5") { // ���������, ������ �� �������� "md5"
            algo = HashAlgorithm::MD5; // ������������� �������� MD5
        }
        else if (args.algo == "sha256") { // ���������, ������ �� "sha256"
            algo = HashAlgorithm::SHA256; // ������������� �������� SHA-256
        }
        else if (args.algo == "sha512") { // ���������, ������ �� "sha512"
            algo = HashAlgorithm::SHA512; // ������������� �������� SHA-512
        }
        else if (args.algo == "crc32") { // ���������, ������ �� "crc32"
            algo = HashAlgorithm::CRC32; // ������������� �������� CRC32
        }
        else { // ������������ ������ ������������ ���������
            std::cerr << "������: ����������� �������� " << args.algo << std::endl;
            return 1;
        }

        // ���������, ���������� �� ����������
        if (!fs::exists(args.dir) || !fs::is_directory(args.dir)) {
            std::cerr << "������: ���������� " << args.dir << " �� ���������� ��� �� �������� �����������" << std::endl;
            return 1;
        }

        // ��������� ���� � ����������� ������
        std::ifstream hash_file(args.hash_file);
        if (!hash_file.is_open()) {
            std::cerr << "������: �� ������� ������� ���� � ������ " << args.hash_file << std::endl;
            return 1;
        }

        // ������ ���������� ���� � map: ���� -> ���
        std::map<std::string, std::string> saved_hashes;
        std::string line;
        while (std::getline(hash_file, line)) {
            std::istringstream iss(line);
            std::string hash, path;
            iss >> hash; // ������ ��� (������ �����)
            std::getline(iss >> std::ws, path); // ������ ������� ������ (����), ��������� �������
            if (!hash.empty() && !path.empty()) {
                saved_hashes[path] = hash;
            }
        }
        hash_file.close();

        // �������� ������� ���� ������ � ����������
        std::map<std::string, std::string> current_hashes;
        for (const auto& entry : fs::recursive_directory_iterator(args.dir)) {
            if (entry.is_regular_file()) {
                std::string file_path = entry.path().string();
                std::string hash = compute_hash(file_path, algo);
                if (hash.empty()) {
                    std::cerr << "��������������: �� ������� ��������� ��� ��� ����� " << file_path << std::endl;
                    continue;
                }
                current_hashes[file_path] = hash;
            }
        }

        // ���������� ���� � ������� ���������
        bool has_changes = false;

        // ��������� ��������� � �������� �����
        for (const auto& [path, saved_hash] : saved_hashes) {
            auto it = current_hashes.find(path);
            if (it == current_hashes.end()) {
                std::cout << "�����: " << path << std::endl;
                std::cout << std::endl;
                has_changes = true;
            }
            else if (it->second != saved_hash) {
                std::cout << "������: " << path << std::endl;
                std::cout << "  ������ ���: " << saved_hash << std::endl;
                std::cout << "  ����� ���: " << it->second << std::endl;
                std::cout << std::endl;
                has_changes = true;
            }
        }

        // ��������� ����������� �����
        for (const auto& [path, hash] : current_hashes) {
            if (saved_hashes.find(path) == saved_hashes.end()) {
                std::cout << "��������: " << path << std::endl;
                std::cout << "  ���: " << hash << std::endl;
                std::cout << std::endl;
                has_changes = true;
            }
        }

        // ���� ��������� ���, ������� ���������
        if (!has_changes) {
            std::cout << "��������� �� ���������� � ���������� " << args.dir << std::endl;
            std::cout << std::endl;
        }

        return 0; // �������� ����������
        } // ��������� ���� �������

        // ��������� ������ ������������ ������ (�� ������ ����������� ��-�� �������� � parse_arguments)
        std::cerr << "������: ����������� ����� " << args.mode << std::endl; // ������� ��������� �� ������
        return 1; // ���������� ��� ������
} // ��������� ����������� ������� dispatch
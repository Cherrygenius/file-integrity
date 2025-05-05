#include "dispatcher.h" // ���������� dispatcher.h ��� ������� � ���������� dispatch
#include "hasher.h" // ���������� hasher.h ��� ������� � compute_hash � HashAlgorithm
#include <iostream> // ���������� ���������� ��� ������ ��������� � �������

// ������ ������ ��� ����������

int dispatch(const Arguments& args) { // ���������� ������� dispatch, ����������� Arguments � ������������ ��� ����������
    // ��������������� �� ������
    if (args.mode == "hash") { // ���������, �������� �� ����� hash
        // ����������� ��������� �������� � HashAlgorithm
        HashAlgorithm algo; // ��������� ���������� ��� �������� ���������
        if (args.algo == "md5") { // ���������, ������ �� md5
            algo = HashAlgorithm::MD5; // ������������� MD5
        }
        else if (args.algo == "sha256") { // ��������� sha256
            algo = HashAlgorithm::SHA256; // ������������� SHA-256
        }
        else if (args.algo == "sha512") { // ��������� sha512
            algo = HashAlgorithm::SHA512; // ������������� SHA-512
        }
        else if (args.algo == "crc32") { // ��������� crc32
            algo = HashAlgorithm::CRC32; // ������������� CRC32
        }
        else { // ���� �������� ���������� (�� ������ ���������)
            std::cerr << "������: ����������� �������� " << args.algo << std::endl; // ������� ������
            return 1; // ���������� ��� ������
        } // ��������� �������� ���������

        // ��������� ��� �����
        std::string hash = compute_hash(args.file, algo); // �������� compute_hash ��� ����� � ���������
        if (hash.empty()) { // ���������, �������� �� ������ ��� (������)
            std::cerr << "������: �� ������� ��������� ��� ��� ����� " << args.file << std::endl; // ������� ������
            return 1; // ���������� ��� ������
        } // ��������� ��������

        // ������� ��� � �������, ����������� � md5sum
        std::cout << hash << "  " << args.file << std::endl; // ������� ��� � ���� �����
        return 0; // ���������� 0, �������� �� �����
    }
    else if (args.mode == "compare") { // ��������� ����� compare
        std::cout << "����� compare: ��������� ������ " << args.file // ������� ��������� (��������)
            << " � " << args.file2 << " � ���������� " << args.algo << std::endl; // ��������� ������
        // TODO: ������� ������� ��������� ������
        return 0; // ���������� 0 (��������)
    }
    else if (args.mode == "scan") { // ��������� ����� scan
        std::cout << "����� scan: ������������ ���������� " << args.dir // ������� ��������� (��������)
            << " � ���������� " << args.algo; // ��������� ��������
        if (!args.output.empty()) { // ���������, ������ �� --output
            std::cout << ", ���������� � " << args.output; // ��������� ����������
        } // ��������� ��������
        std::cout << std::endl; // ��������� ������� ������
        // TODO: ������� ������� ������������
        return 0; // ���������� 0 (��������)
    }
    else if (args.mode == "check") { // ��������� ����� check
        std::cout << "����� check: �������� ��������� � ���������� " << args.dir // ������� ��������� (��������)
            << " � ������ ����� " << args.hash_file // ��������� ���� �����
            << " � ���������� " << args.algo << std::endl; // ��������� ��������
        // TODO: ������� ������� �������� ���������
        return 0; // ���������� 0 (��������)
    } // ��������� ���� �������

    // �� ������ ������ (�� ������ ���������, ��� ��� ����� ����������� � parse_arguments)
    std::cerr << "������: ����������� ����� " << args.mode << std::endl; // ������� ������
    return 1; // ���������� ��� ������
} // ��������� ������� dispatch
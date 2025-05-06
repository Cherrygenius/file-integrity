#include "hasher.h" 
#include <openssl/evp.h> 
#include <openssl/err.h> // ��� �������
#include <fstream> 
#include <iomanip> //����������������� ������
#include <sstream> 
#include <vector> 



namespace { // ��������� ��������� ������������ ��� ��� ��������������� ������� CRC32, ����� ��� ���� �������� ��� ����� �����
    // ������� CRC32 (������� 0xEDB88320)
    uint32_t crc32_table[256]; // ��������� ������ ��� ������� CRC32, ������������ ��� ��������� ����������
    bool crc32_table_initialized = false; 

    void init_crc32_table() { 
        for (uint32_t i = 0; i < 256; ++i) { // ���������� ��� ��������� ����� (0�255)
            uint32_t crc = i; // �������������� crc ������� ������
            for (int j = 0; j < 8; ++j) { // ��������� 8 �������� ��� ������� ����
                if (crc & 1) { // ���������, ���������� �� ������� ���
                    crc = (crc >> 1) ^ 0xEDB88320; // �������� ������ � ��������� �������
                }
                else { // ���� ������� ��� �� ����������
                    crc >>= 1; // ������ �������� ������
                } 
            } 
            crc32_table[i] = crc; 
        } 
        crc32_table_initialized = true; 
    } 

    // ��������� CRC32 ��� ������
    uint32_t compute_crc32(const std::vector<unsigned char>& buffer) { 
        if (!crc32_table_initialized) { 
            init_crc32_table(); 
        } 
        uint32_t crc = 0xFFFFFFFF; // �������������� crc ��������� ��������� (��� ���� 1)
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



    EVP_MD_CTX* ctx = EVP_MD_CTX_new(); // ������ ����� �������� EVP ��� �����������
    if (!ctx) { // ���������, ������� �� ������� ��������
        file.close();
        return ""; 
    } 

    const EVP_MD* md = nullptr; 
    if (algo == HashAlgorithm::MD5) { 
        md = EVP_md5(); // ������������� ����� MD5
    }
    else if (algo == HashAlgorithm::SHA256) { 
        md = EVP_sha256(); // ������������� ����� SHA-256
    }
    else if (algo == HashAlgorithm::SHA512) { 
        md = EVP_sha512(); // ������������� ����� SHA-512
    } 

    if (md && EVP_DigestInit_ex(ctx, md, nullptr) != 1) { 
        EVP_MD_CTX_free(ctx); // ����������� �������� ��� ������
        file.close(); 
        return ""; 
    } 



    while (file) { 
        file.read(reinterpret_cast<char*>(buffer.data()), buffer_size); 
        std::streamsize bytes_read = file.gcount(); // �������� ���������� ����������� ������
        if (bytes_read > 0) { // ���������, ���� �� ��������� ������
            if (md && EVP_DigestUpdate(ctx, buffer.data(), bytes_read) != 1) { // ��������� ���
                EVP_MD_CTX_free(ctx); // ����������� �������� ��� ������
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
            EVP_MD_CTX_free(ctx); // ����������� ��������
            file.close(); 
            return ""; 
        } 
        std::stringstream ss; 
        for (unsigned int i = 0; i < hash_length; ++i) { 
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash_value[i]; // ����������� ���� ��� ��� ����������������� �������
        } 
        hash = ss.str();
    } 


    else if (algo == HashAlgorithm::CRC32) { 
        uint32_t crc = 0; 
        file.clear(); // ���������� ����� ��������� �����
        file.seekg(0); // ���������� ��������� � ������ �����
        while (file) { // ������ ���� ������
            file.read(reinterpret_cast<char*>(buffer.data()), buffer_size); // ������ ����
            std::streamsize bytes_read = file.gcount(); // �������� �����
            if (bytes_read > 0) { // ��������� ������
                buffer.resize(bytes_read); // ������� �����
                crc = compute_crc32(buffer); // ��������� CRC32
            } 
        } 
        std::stringstream ss; 
        ss << std::hex << std::setw(8) << std::setfill('0') << crc; // ����������� CRC32 ��� 8-���������� ������
        hash = ss.str(); 
    } 


    // ������������ �������� � �������� �����
    EVP_MD_CTX_free(ctx); // ����������� �������� EVP
    file.close(); 
    return hash; 
} 
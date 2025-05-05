#ifndef HASHER_H
#define HASHER_H
#include <string>

enum class HashAlgorithm {
	MD5,
	SHA256,
	SHA512,
	CRC32
};

std::string compute_hash(const std::string& filePath, HashAlgorithm algo);

#endif // HASHER_H
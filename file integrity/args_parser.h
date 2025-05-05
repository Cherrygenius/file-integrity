#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H
#include <string>

struct Arguments
{
	std::string mode;
	std::string file;
	std::string file2;
	std::string dir;
	std::string algo;
	std::string output;
	std::string hash_file;
};

bool parse_arguments(int argc, char* argv[], Arguments& args);


#endif // !ARGS_PARSER_H



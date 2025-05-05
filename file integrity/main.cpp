#include "args_parser.h"
#include "dispatcher.h" 
#include <clocale>


int main(int argc, char* argv[]) { 
    setlocale(LC_ALL, "Russian");
    Arguments args; 

    if (!parse_arguments(argc, argv, args)) {
        return 1; 
    } 

    return dispatch(args); 
} 
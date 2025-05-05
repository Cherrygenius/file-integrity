cmake_minimum_required(VERSION 3.15)

# Укажите имя проекта
project(FileIntegrity LANGUAGES CXX)

# Укажите путь к vcpkg.cmake
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Укажите исходные файлы
set(SOURCES
    main.cpp
    args_parser.cpp
    dispatcher.cpp
    hasher.cpp
)

# Укажите заголовочные файлы (опционально, для IDE)
set(HEADERS
    args_parser.h
    dispatcher.h
    hasher.h
)

# Создайте исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Найдите и подключите OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Если используется argparse через vcpkg
find_package(argparse CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE argparse::argparse)

# Укажите дополнительные пути к include, если нужно
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

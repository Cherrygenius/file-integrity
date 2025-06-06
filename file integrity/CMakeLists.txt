cmake_minimum_required(VERSION 3.15)

# ������� ��� �������
project(FileIntegrity LANGUAGES CXX)

# ������� ���� � vcpkg.cmake
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")

# ���������� �������� C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ������� �������� �����
set(SOURCES
    main.cpp
    args_parser.cpp
    dispatcher.cpp
    hasher.cpp
)

# ������� ������������ ����� (�����������, ��� IDE)
set(HEADERS
    args_parser.h
    dispatcher.h
    hasher.h
)

# �������� ����������� ����
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# ������� � ���������� OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# ���� ������������ argparse ����� vcpkg
find_package(argparse CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE argparse::argparse)

# ������� �������������� ���� � include, ���� �����
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

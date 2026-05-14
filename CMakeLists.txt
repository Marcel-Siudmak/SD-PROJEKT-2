# 1. Wymagana wersja CMake
cmake_minimum_required(VERSION 3.10)

# 2. Nazwa projektu 
project(SD_PROJEKT_2 VERSION 1.0 LANGUAGES CXX)

# 3. Konfiguracja standardu C++ (C++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 4. Automatyczne wyszukiwanie wszystkich plików .cpp w folderze src
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 5. Dodanie folderu src do ścieżek poszukiwań nagłówków (.hpp)
# Dzięki temu w kodzie piszesz #include "ArrayList.hpp" zamiast #include "src/ArrayList.hpp"
include_directories(src)

# 6. Tworzenie pliku wykonywalnego
add_executable(mini_projekt_2 ${SOURCES})

# 7. Przekazanie do kodu C++ bezwzględnej ścieżki do głównego katalogu projektu
# Dzięki temu program zawsze wie, gdzie utworzyć bazowe foldery 'data' i 'results', 
# niezależnie od tego, z jakiego miejsca (np. z build/) został uruchomiony.
target_compile_definitions(mini_projekt_2 PRIVATE PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# 8. Flagi ostrzeżeń (Warnings) - pomagają wyłapać błędy w kodzie na etapie kompilacji
if(MSVC)
    target_compile_options(mini_projekt_2 PRIVATE /W4)
else()
    target_compile_options(mini_projekt_2 PRIVATE -Wall -Wextra -Wpedantic)
endif()
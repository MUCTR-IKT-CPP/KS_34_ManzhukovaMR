cmake_minimum_required(VERSION 3.16)
project(GameFactory LANGUAGES CXX)

# Требуем C++20 (необходимо для концептов)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Создаем исполняемый файл
add_executable(game main.cpp
    entities.hpp)

# Если у вас старый MinGW на Windows, установим кодировку для вывода русских букв в консоль
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(game PRIVATE -finput-charset=UTF-8 -fexec-charset=CP1251)
endif()

# Включаем предупреждения (по желанию)
if(MSVC)
    target_compile_options(game PRIVATE /W4)
else()
    target_compile_options(game PRIVATE -Wall -Wextra)
endif()
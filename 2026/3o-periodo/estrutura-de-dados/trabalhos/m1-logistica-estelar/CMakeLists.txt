cmake_minimum_required(VERSION 3.14)
project(M1-LogisticaEstelar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. A MÁGICA: O CMake vai no GitHub, baixa a Raylib e configura sozinho
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# 2. Pega todos os seus arquivos .cpp que estão na pasta (app.cpp, TAD_Nave.cpp, etc)
file(GLOB SOURCES "*.cpp")

# 3. Cria o seu executável chamado "app"
add_executable(app ${SOURCES})

# 4. Junta a Raylib no seu projeto
target_link_libraries(app PRIVATE raylib)
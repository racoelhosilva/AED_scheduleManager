cmake_minimum_required(VERSION 3.26)
project(AED2324_PRJ1_G15)

set(CMAKE_CXX_STANDARD 17)

add_executable(AED2324_PRJ1_G15 src/main.cpp
        src/Aula.cpp
        src/Aula.h
        src/Turma.cpp
        src/Turma.h
        src/Estudante.h
        src/Estudante.cpp
        src/Gestor.cpp
        src/Gestor.h
        src/Interface.cpp
        src/Interface.h
        src/Pedido.cpp
        src/Pedido.h
)

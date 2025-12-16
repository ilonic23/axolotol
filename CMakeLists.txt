cmake_minimum_required(VERSION 4.0)
project(Axolotol C)

set(CMAKE_C_STANDARD 11)

add_executable(Axolotol main.c
        lexer.h
        token.h
        lexer.c
        token.c
        ast.h
        ast.c
        parser.h
        parser.c
        visitor.h
        visitor.c)

configure_file(example.axo "${CMAKE_CURRENT_BINARY_DIR}/example.axo" COPYONLY)

cmake_minimum_required(VERSION 3.26)
project(NHF C)

set(CMAKE_C_STANDARD 11)

add_executable(NHF main.c
        hatter.c
        hatter.h
        pvpjatek.c
        pvpjatek.h
        pvejatek.c
        pvejatek.h)

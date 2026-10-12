cmake_minimum_required(VERSION 3.28)
project(ProjekatOOP1_)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProjekatOOP1_ main.cpp
        core/Lexer.cpp
        core/Lexer.h
        commands/Command.cpp
        commands/Command.h
        core/File_Handler.cpp
        core/File_Handler.h
        commands/Echo.cpp
        commands/Echo.h
        core/Machine.cpp
        core/Machine.h
        commands/Prompt.cpp
        commands/Prompt.h
        commands/Time.cpp
        commands/Time.h
        commands/Date.cpp
        commands/Date.h
        commands/Touch.cpp
        commands/Touch.h
        commands/Truncate.cpp
        commands/Truncate.h
        commands/Rm.cpp
        commands/Rm.h
        commands/Wc.cpp
        commands/Wc.h
        commands/Tr.cpp
        commands/Tr.h
        commands/Head.cpp
        commands/Head.h
        commands/Batch.cpp
        commands/Batch.h
        utils/Exceptions.h)



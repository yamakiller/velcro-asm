CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(PROJ_NAME VelcroAsm)

#调试版本增加后缀
SET(CMAKE_DEBUG_POSTFIX "_debug")

#设置编译输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/binaries)
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/binaries)

#设置编译输出目录为默认库搜索路径
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/binaries)
#设置头文件搜索目录
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

#设置不同平台的编辑参数
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    #在Windows平台下值考虑MSVC编译器
    MESSAGE(STATUS "Platform Window")

    IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        MESSAGE(STATUS "Compile Tool mingw64")

        SET(CMAKE_CXX_COMPILER g++)
        SET(CMAKE_C_COMPILER gcc)
    
        SET(CMAKE_CXX_FLAGS_DEBUG	"-std=c++17 -O0 -Wall -march=pentium4 -mmmx -g2 -ggdb")
        SET(CMAKE_CXX_FLAGS_RELEASE	"-std=c++17 -Wall -o3")
    ELSE()
        MESSAGE(STATUS "Compile Tool msvc")
        ADD_DEFINITIONS(-D_CRT_SECURE_NOWARNINGS)
        #这是为UNICODE编码模式
        ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
        
        SET(CMAKE_CXX_STANDARD 17)
        SET(CMAKE_C_STANDARD 17)
        SET(CMAKE_CXX_FLAGS_DEBUG	"/D_DEBUG /MDd /Zi /Ob0 /Od /RTC1")
        SET(CMAKE_CXX_FLAGS_RELEASE	"/MD /O2 /Ob2 /D NDEBUG")

        ADD_LINK_OPTIONS(/SAFESEH:NO)

        ADD_COMPILE_OPTIONS("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        ADD_COMPILE_OPTIONS("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

        ADD_COMPILE_OPTIONS(/Zc:preprocessor /wd5105)
    ENDIF()
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "Platform Linux")
    MESSAGE(STATUS "Compile Tool gcc/g++")

    SET(CMAKE_CXX_COMPILER g++)
    SET(CMAKE_C_COMPILER gcc)

    SET(CMAKE_CXX_FLAGS_DEBUG	"-std=c++17 -O0 -Wall -march=pentium4 -mmmx -g2 -ggdb")
    SET(CMAKE_CXX_FLAGS_RELEASE	"-std=c++17 -Wall -o3")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    MESSAGE(STATUS "Platform FreeBSD")
    MESSAGE(STATUS "Compile Tool gcc/g++")

    SET(CMAKE_CXX_COMPILER g++)
    SET(CMAKE_C_COMPILER gcc)

    SET(CMAKE_CXX_FLAGS_DEBUG	"-std=c++17 -O0 -Wall -march=pentium4 -mmmx -g2 -ggdb")
    SET(CMAKE_CXX_FLAGS_RELEASE	"-std=c++17 -Wall -o3")
ELSE()
    # 考虑支持 Mac 系统
    MESSAGE(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Windows")

FILE (GLOB VELCRO_ASM_SOURCES
    "asm/arch.h"
    "asm/arch.cc"
    "general/error.h"
    "general/error.cc"
    )


ADD_LIBRARY(${PROJ_NAME} SHARED
    ${VELCRO_ASM_SOURCES})
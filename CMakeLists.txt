cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(depixelize)
set(CMAKE_CXX_STANDARD 14)

# Set the custom install dir for Windows here
set(GLUT_ROOT_PATH "C:/Users/Rounaq/Downloads/freeglut-MSVC-3.0.0-2.mp/freeglut")

if(WIN32 AND "${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    # 32 bit machine
    set(IS_32 ON)
else()
    set(IS_32 OFF)
endif()

# Resolve GLUT
find_package(GLUT QUIET)
if(NOT GLUT_FOUND)
    if(WIN32)
        if(NOT DEFINED GLUT_ROOT_PATH)
            message(FATAL_ERROR "GLUT_ROOT_PATH not defined. Please download freeglut-3 and pass -DGLUT_ROOT_PATH=<path/to/downloaded/glut/installation>")
        endif()
        set(X64_SUFFIX_PATH "x64")
        if(IS_32)
            set(X64_SUFFIX_PATH "")
        endif()   
        # Find the header
        find_path(GLUT_INCLUDE_DIR GL/freeglut.h
            HINTS
            "${GLUT_ROOT_PATH}/include"
            "${GLUT_ROOT_PATH}")
        if(NOT GLUT_INCLUDE_DIR)
            message(FATAL_ERROR "Couldn't find GL/freeglut.h in the installation at ${GLUT_ROOT_PATH}")
        endif()
        find_library(GLUT_LIBRARY freeglut glut
            HINTS
            ${GLUT_ROOT_PATH}/lib/${X64_SUFFIX_PATH})
        if(NOT GLUT_LIBRARY)
            message(FATAL_ERROR "Couldn't find freeglut.lib in the installation at ${GLUT_ROOT_PATH}")
        endif()
        find_file(GLUT_LIBRARY_DLL freeglut.dll
            HINTS
            ${GLUT_ROOT_PATH}/bin/${X64_SUFFIX_PATH}
        )
        if(NOT GLUT_LIBRARY_DLL)
            message(FATAL_ERROR "Couldn't find freeglut.dll in the installation at ${GLUT_ROOT_PATH}")
        endif()
        set(GLUT_LIBRARIES ${GLUT_LIBRARY})
    else()
        message(FATAL_ERROR "GLUT not found. Please install it via a package manager like apt")
    endif()
endif()

if(NOT WIN32)
    find_package(OpenGL REQUIRED)
endif()

add_executable(depixelize
    src/graph.cpp
    src/image.cpp
    src/spline.cpp
    src/voronoi.cpp
    src/main.x.cpp
)
if(WIN32)
    target_include_directories(depixelize PRIVATE "${GLUT_INCLUDE_DIR}")
    target_link_libraries(depixelize PRIVATE "${GLUT_LIBRARIES}")
    if(DEFINED GLUT_LIBRARY_DLL)
        add_custom_command(TARGET depixelize POST_BUILD COMMAND
            ${CMAKE_COMMAND} -E copy_if_different "${GLUT_LIBRARY_DLL}" $<TARGET_FILE_DIR:depixelize>)
    endif()
else()
    target_link_libraries(depixelize PRIVATE GLUT::GLUT OpenGL::GL)
endif()
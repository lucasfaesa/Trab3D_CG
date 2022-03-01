cmake_minimum_required(VERSION 3.21)
project(Trabalhocg)

set(CMAKE_CXX_STANDARD 14)
set(GLUT_INCLUDE_DIR "C:/MinGW/freeglut/include")
set(OPENGL_LIBRARY_DIR "C:/MinGW/freeglut/lib")
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
if(NOT GLUT_FOUND)
    message(ERROR "GLUT not found!")
endif(NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
if(NOT OPENGL_FOUND)
    message(ERROR "OPENGL not found!")
endif(NOT OPENGL_FOUND)

add_executable(Trabalhocg main.cpp player.cpp player.h tinyxml2.cpp tinyxml2.h Cenario.cpp Cenario.h tiro.cpp tiro.h enemy.cpp enemy.h enemyTiro.cpp enemyTiro.h)

target_link_libraries(Trabalhocg ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

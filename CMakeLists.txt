cmake_minimum_required(VERSION 3.5)
project(Flood CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(${PROJECT_NAME} flood.cpp)

target_link_libraries( ${PROJECT_NAME} ${OPENGL_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} ${GLUT_LIBRARY} )

project(Generator CXX)
add_executable(${PROJECT_NAME} map_generator.cpp)

target_link_libraries( ${PROJECT_NAME} ${OPENGL_LIBRARIES} )
target_link_libraries( ${PROJECT_NAME} ${GLUT_LIBRARY} )

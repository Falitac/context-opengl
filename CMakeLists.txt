cmake_minimum_required(VERSION 3.10)

project(context_opengl VERSION 1.0)

file(GLOB_RECURSE sources "src/*.cc")
add_executable(context_opengl ${sources})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
#file(COPY binaries/*.dll DESTINATION ${CMAKE_BINARY_DIR})


set_property(TARGET context_opengl PROPERTY CXX_STANDARD 20)

target_link_libraries(context_opengl sfml-graphics sfml-window sfml-system)
target_link_libraries(context_opengl glew32) 
target_link_libraries(context_opengl opengl32) 


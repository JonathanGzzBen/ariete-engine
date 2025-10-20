cmake_minimum_required(VERSION 3.11)

project(ariete-engine)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(ariete-engine src/main.cpp src/font.cpp src/graphic_context.cpp src/mesh.cpp src/program.cpp src/texture.cpp src/vertex_array_object.cpp)
set_target_properties(ariete-engine PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)
target_include_directories(ariete-engine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include" ${Stb_INCLUDE_DIR})
target_link_libraries(ariete-engine PRIVATE GLEW::GLEW glfw glm::glm)

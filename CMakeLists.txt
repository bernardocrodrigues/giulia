cmake_minimum_required(VERSION 3.10)

#specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(Giulia VERSION 0.1.0
                LANGUAGES C CXX)

# add the scr folder
add_subdirectory(src/app)
add_subdirectory(src/cl)
add_subdirectory(src/gl)
add_subdirectory(src/vendor/stb)
add_subdirectory(src/vendor/glm)
add_subdirectory(src/vendor/imgui)
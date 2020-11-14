set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

add_executable(Giulia giulia.cpp window_handler.cpp renderer.cpp)

target_compile_options(Giulia PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(Giulia
                              ${GLEW_LIBRARIES}
                              ${GLFW_LIBRARIES}
                              ${OPENGL_LIBRARIES}
                              GLUtils
                              CLUtils
                              OpenCL
                              ImGUI
                     )

target_include_directories(Giulia
                                PRIVATE
                                ${OPENGL_INCLUDE_DIRS}
                                ${PROJECT_SOURCE_DIR}/src/app
                                ${PROJECT_SOURCE_DIR}/src/utils
                                ${PROJECT_SOURCE_DIR}/src/gl
                                ${PROJECT_SOURCE_DIR}/src/cl
                                ${PROJECT_SOURCE_DIR}/src/vendor/glm
                                ${PROJECT_SOURCE_DIR}/src/vendor/imgui
                          )
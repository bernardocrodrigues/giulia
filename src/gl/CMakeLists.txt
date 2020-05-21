add_library(GLUtils glutils.cpp)

# include_directories(${PROJECT_SOURCE_DIR}/src/vendor/stb)
# include_directories(${PROJECT_SOURCE_DIR}/src/vendor/glm)


set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(PkgConfig REQUIRED)
# pkg_search_module(GLFW REQUIRED glfw3)

target_link_libraries(GLUtils ${OPENGL_LIBRARIES})

target_include_directories(GLUtils
                                PRIVATE
                                ${PROJECT_SOURCE_DIR}/src/vendor/stb
                                ${PROJECT_SOURCE_DIR}/src/vendor/glm
                                ${PROJECT_SOURCE_DIR}/src/utils
                          )

target_link_libraries(GLUtils STBImage)


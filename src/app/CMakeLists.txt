
# include_directories(${PROJECT_SOURCE_DIR}/src/cl)
# include_directories(${PROJECT_SOURCE_DIR}/src/gl)
# include_directories(${PROJECT_SOURCE_DIR}/src/vendor/glm)
# include_directories(${PROJECT_SOURCE_DIR}/src/vendor/imgui)

# set(OpenGL_GL_PREFERENCE GLVND)

# # add the executable
# add_executable(ProcessArray process_array.cpp)
# add_executable(ProcessMultiArray process_multi_array.cpp)

# configure_file(version.h.in version.h)

# target_include_directories(ProcessArray PUBLIC
#                            "${PROJECT_BINARY_DIR}"
#                           )

# target_include_directories(ProcessMultiArray PUBLIC
#                            "${PROJECT_BINARY_DIR}"
#                           )

# target_link_libraries(ProcessArray CLUtils)
# target_link_libraries(ProcessArray OpenCL)

# target_link_libraries(ProcessMultiArray CLUtils)
# target_link_libraries(ProcessMultiArray OpenCL)

# find_package(SDL2 REQUIRED)

# include_directories(SDLtest ${SDL2_INCLUDE_DIRS})
# add_executable(SDLtest sdl_test.cpp)
# target_link_libraries(SDLtest ${SDL2_LIBRARIES})

# # Simple GL Example
# add_executable(GLtest gl_test.cpp)
# find_package(GLUT REQUIRED)
# include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
# target_link_libraries(GLtest ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

# # Modern GL Example

# add_executable(GLFWtest glfw_test.cpp)

# target_include_directories(GLFWtest PUBLIC
#                            "${PROJECT_BINARY_DIR}"
#                           )

# include_directories(${GLFW_INCLUDE_DIRS})
# target_link_libraries(GLFWtest ${GLEW_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} CLUtils OpenCL GLUtils ImGUI)
# # target_link_libraries(GLFWtest GLUtils)



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
                              ImGUI
                     )

target_include_directories(Giulia
                                PRIVATE
                                ${PROJECT_SOURCE_DIR}/src/app
                                ${PROJECT_SOURCE_DIR}/src/utils
                                ${PROJECT_SOURCE_DIR}/src/gl
                                ${PROJECT_SOURCE_DIR}/src/vendor/glm
                                ${PROJECT_SOURCE_DIR}/src/vendor/imgui
                          )

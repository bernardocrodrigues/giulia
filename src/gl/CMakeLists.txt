add_library(GLUtils glutils.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src/vendor)
target_link_libraries(GLUtils STBImage)


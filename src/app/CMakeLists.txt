
include_directories(${PROJECT_SOURCE_DIR}/src/cl)

# add the executable
add_executable(Giulia hello.cpp)

configure_file(version.h.in version.h)

target_include_directories(Giulia PUBLIC
                           "${PROJECT_BINARY_DIR}"
                          )

target_link_libraries(Giulia CLUtils)
target_link_libraries(Giulia OpenCL)
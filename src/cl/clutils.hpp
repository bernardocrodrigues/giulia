#define CL_TARGET_OPENCL_VERSION 120

#include <CL/cl.hpp>

cl::Program create_program(const std::string filename);
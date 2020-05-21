#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_TARGET_OPENCL_VERSION 120
#define CL_HPP_ENABLE_EXCEPTIONS


#include <CL/cl2.hpp>

cl::Program create_program(const std::string filename);
cl::Program create_program(const std::string filename,
                           cl_context_properties GLXContext,
                           cl_context_properties X11Display);
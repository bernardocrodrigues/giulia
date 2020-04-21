
#include "src/app/version.h"
#include "clutils.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

    auto program = create_program("src/cl/kernels/array.cl");
    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto device = program.getInfo<CL_PROGRAM_DEVICES>().front();
    cl_int err;

    std::vector<int> vec(10);
    std::fill(vec.begin(),vec.end(), 1);

    cl::Buffer input(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * vec.size(), vec.data());
    cl::Buffer output(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * vec.size());

    cl::Kernel kernel(program, "ProcessArray", &err);
    kernel.setArg(0, input);
    kernel.setArg(1, output);

    cl::CommandQueue queue(context, device);
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(vec.size()));
    queue.enqueueReadBuffer(output, CL_FALSE, 0, sizeof(int) * vec.size(), vec.data());

    cl::finish();
    std::cin.get();

    return 0;
}

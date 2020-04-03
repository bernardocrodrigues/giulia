
#include "src/app/version.h"
#include "clutils.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

    auto program = create_program("src/cl/hello.cl");
    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto device = program.getInfo<CL_PROGRAM_DEVICES>().front();
    cl_int err;
    char buf[16];

    cl::Buffer memBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf));
    cl::Kernel kernel(program, "HelloWorld", &err);
    kernel.setArg(0, memBuffer);

    cl::CommandQueue queue(context, device);
    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(memBuffer, CL_TRUE, 0, sizeof(buf), buf);

    std::cout << buf << std::endl;

    return 0;
}

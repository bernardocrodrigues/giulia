
#include "src/app/version.h"
#include "clutils.hpp"
#include <iostream>
#include <array>
#include <chrono>
#include <thread>

int main(int argc, char *argv[]) {

    auto program = create_program("src/cl/kernels/multiarray.cl");
    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto device = program.getInfo<CL_PROGRAM_DEVICES>().front();
    cl_int err = 0;

    const int rows = 100;
    const int cols = 100;
    const int total = rows * cols;

    static std::array<std::array<int,cols>,rows> arr;

    for(int i = 0; i < rows; i++){
        arr[i].fill(10);
    }

    cl::Buffer buffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * total, arr.data(), &err);
    cl::Kernel kernel(program, "ProcessMultiArray", &err);
    auto workgroup_size = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);

    kernel.setArg(0, buffer);
    std::cout << err << std::endl;

    cl::CommandQueue queue(context, device);
    queue.enqueueWriteBuffer(buffer, CL_TRUE, 0, total, arr.data());
    auto start = std::chrono::high_resolution_clock::now();
    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(cols,rows), cl::NDRange(1,10));

    queue.enqueueReadBuffer(buffer, CL_TRUE, 0, sizeof(int) * total, arr.data());
    // cl::finish();

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n" ;


    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }


    return 0;
}

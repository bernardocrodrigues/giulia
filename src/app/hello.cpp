
#include "src/app/version.h"
#include "clutils.hpp"
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>
#include <vector>


int main(int argc, char *argv[]) {

    shout();

    std::vector<cl::Platform> all_platforms;
    std::vector<cl::Device> devices;
    cl::Platform::get(&all_platforms);

    if (all_platforms.size()==0) {
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    std::cout<<"Platforms: " << all_platforms.size()<<"\n";

    auto platform = all_platforms.front();

    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    for(int i = 0; i < devices.size(); i++ ){
        auto vendor = devices[i].getInfo<CL_DEVICE_VENDOR>();
        auto version = devices[i].getInfo<CL_DEVICE_VERSION>();

        std::cout<<"Device: " << vendor <<"\n";
        std::cout<<"Version: " << version <<"\n";
    }

    auto device = devices.front();

    std::ifstream helloWorldFile("src/cl/hello.cl");
    std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));

    cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length()+1));
    cl::Context context(device);
    cl::Program program(context, sources);

    auto err = program.build("-cl-std=CL1.2");

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

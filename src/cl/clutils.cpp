
#define CL_TARGET_OPENCL_VERSION 120

#include <CL/cl.hpp>
#include <string>
#include <fstream>
#include <iostream>

cl::Program create_program(const std::string filename){

    std::vector<cl::Platform> all_platforms;
    std::vector<cl::Device> devices;
    cl::Platform::get(&all_platforms);

    if (all_platforms.size()==0) {
        std::cout<<"No platforms found. Check OpenCL installation!\n";
        exit(1);
    }

    auto platform = all_platforms.front();
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    auto device = devices.front();
    auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
    auto version = device.getInfo<CL_DEVICE_VERSION>();
    auto name = device.getInfo<CL_DEVICE_NAME>();

    std::cout<<"Using: " << vendor << " "<< name << " (" << version << ")" << std::endl;

    std::ifstream src_file(filename);

    if (!src_file.is_open()){
        std::cout << "Error opening source file!" << std::endl;
        exit(1);
    }

    std::string src(std::istreambuf_iterator<char>(src_file), (std::istreambuf_iterator<char>()));
    cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length()+1));
    cl::Context context(device);
    cl::Program program(context, sources);
    program.build("-cl-std=CL1.2");

    return program;
}
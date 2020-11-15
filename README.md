

## Dependencies

Tested build environments:

- Ubuntu 20.04
- Ubuntu 20.10

```bash
# Install OpenGL related dependencies
sudo apt-get install mesa-utils freeglut3-dev libglew-dev libglfw3-dev

# Install OpenCL related dependencies
sudo apt install opencl-headers ocl-icd-opencl-dev
```

## Build

```bash
# Fetch
git clone https://github.com/bernardocrodrigues/giulia.git giulia && cd giulia

# Configure
mkdir build && cd build
cmake ../ 

# Build
make
```

## Run

```bash
./src/app/Giulia
```

## Future work

- Create a static target for releases
  - There's no standalone bin being produced so the app can be easily distributed on-line. You'll have to install all build dependencies and build it locally to run Giulia
- Improve way bin reads OpenGL & OpenCL kernels 
  - Maybe put them as static strings on a header?
- Implement remaining OpenCL features
  - Double precision
  - Color schemes
  - Full screen Viewing
- Intelligent frame drawing
  - Only draw a new frame if something has changed; there's no point in redraw the same frames over and over
- Low Resolution rendering
  - This could be used to improve panning on compute costly regions, especially when using double precision.
- Support for variable screen size
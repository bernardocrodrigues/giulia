![](https://static.wixstatic.com/media/7afe00_67bb7afa8cb34690a312d56f74e74eb3~mv2.png/v1/fill/w_1909,h_695,al_b,q_90,usm_0.66_1.00_0.01/7afe00_67bb7afa8cb34690a312d56f74e74eb3~mv2.webp)

# Giulia

A **G**PU accelerated J**ulia** Set fractal interactive viewer implemented in C++ that supports rendering through OpenGL or OpenCL. Giulia also features a Mandelbrot renderer.

Check out these sources if you wish to learn about the Julia Sets, fractals and their relation to the Mandelbrot Set:

- [Julia set - Wikipedia](https://en.wikipedia.org/wiki/Julia_set)
- [The Julia Sets: How it Works, and Why it's Amazing! - Youtube](https://youtu.be/mg4bp7G0D3s)
- [What's so special about the Mandelbrot Set?  - Youtube](https://youtu.be/FFftmWSzgmk)
- [The Mandelbrot Set - Youtube](https://youtu.be/NGMRB4O922I)
- [Filled Julia Set - Youtube](https://youtu.be/oCkQ7WK7vuY)

### This project features

- Julia and Mandelbrot renderings
- Real-time Rendering
- Dynamic parameters
  - Panning & Zooming
  - Single or Double precision
  - Number of calculations per pixel
  - Compute mode (OpenGL or OpenCL)
- Imgui based GUI 
- C++14 standard
- CMake based
- OpenGL & OpenCL Interoperation

### Special Thanks 

[The Cherno](https://github.com/TheCherno) for his awesome [Youtube Channel](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw) and [OpenGL tutorials.](https://youtu.be/W3gAzLwfIP0)

## How to use it

The Jiulia Set is defined according to the following equation: 

![{\displaystyle f(z)=z^{n}+c}](https://wikimedia.org/api/rest_v1/media/math/render/svg/ba2279a150b7a781d3fbb68fa774633aad99f8e2)

Giulia lets you tweak both n and c and see the result in real-time.

#### Move the red cursor on the Mandelbrot Set to choose a value of *c*.

![](https://static.wixstatic.com/media/7afe00_5fbcef7ace374f60b38ec6a0199e3542~mv2.gif)

#### Hold SPACEBAR + click + drag to pan around. Use the scroll-wheel to zoom in and out.

![](https://static.wixstatic.com/media/7afe00_49d7af7ead8e4490ba17ca10f4b42e82~mv2.gif)

#### Use the Slider to adjust the number of iterations on each pixel. This impacts performance.

![](https://static.wixstatic.com/media/7afe00_44290f2b4ce5426fa6990e49baa84fce~mv2.gif)

#### Use the Slider to change color presets. OpenGL only.

![](https://static.wixstatic.com/media/7afe00_7ee2f970713b463cbf803265c9aea04b~mv2.gif)

#### Use the check boxes to change rendering modes. There are no visual differences but trust me, under the hood this is completely different.

![](https://static.wixstatic.com/media/7afe00_800349c03fef46b7ba3d806512b2fdae~mv2.gif)

#### Use the check boxes to switch precision modes. Only needed if you zoom in enough. This has absurd impact on performance. OpenGL only.

![](https://static.wixstatic.com/media/7afe00_8d46e635bd4442f88237bb09d5e417a4~mv2.gif)

#### Use the check boxes to toggle Full screen mode. OpenGL only.

![](https://static.wixstatic.com/media/7afe00_1183f86e039047a9a0916c0f21be8579~mv2.gif)

#### Use the Slider to change the value of *z*

![](https://static.wixstatic.com/media/7afe00_9c51194efefb4ae2ae5890838be3c517~mv2.gif)

## Dependencies

Tested build environments:

- Ubuntu 20.04
- Ubuntu 20.10

```bash
# Install OpenGL related dependencies
sudo apt install mesa-utils freeglut3-dev libglew-dev libglfw3-dev

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
- Implement CI
- Print the value of *c* used in the Julia rendering
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

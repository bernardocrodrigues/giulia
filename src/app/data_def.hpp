#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX

#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_TARGET_OPENCL_VERSION 120
#define CL_HPP_ENABLE_EXCEPTIONS

#define WIDTH 2000
#define HEIGHT 1000

enum class mouse_state_t { MOUSE_RELEASSED, MOUSE_JUST_CLICKED, MOUSE_CLICKED };

enum class compute_mode_t { OPENGL_MODE, OPENCL_MODE };

enum class precision_mode_t { SINGLE_MODE, DOUBLE_MODE };

enum class compute_target_t { MANDELBROT, JULIA };

enum class window_region_t {
  LEFT,
  RIGHT,
  FULL,
};

enum class fullscreen_mode_t { NONE, MANDELBROT, JULIA };

struct mouse_coodenate {
  double x;
  double y;
};

struct complex_number {
  double real;
  double imaginary;
};

const float left_half_positions[] = {
    0.0,                0.0,            0.0,    0.0,
    ((float) WIDTH)/2,  0.0,            1.0,    0.0,
    ((float) WIDTH)/2,  (float) HEIGHT, 1.0,    1.0,
    0.0,                (float) HEIGHT, 0.0,    1.0
};

const float right_half_positions[] = {
    ((float) WIDTH)/2,  0.0,
    ((float) WIDTH),    0.0,
    ((float) WIDTH),    (float) HEIGHT,
    ((float) WIDTH)/2,  (float) HEIGHT
};

const float full_screen_positions[] = {
    0.0,                0.0,
    ((float) WIDTH),    0.0,
    ((float) WIDTH),    (float) HEIGHT,
    0.0,                (float) HEIGHT
};

const float logo_scale = 0.3;
const float logo_positions[] = {
    0,      0,      0.0,    0.0,    // 0
    430,    0,      1.0,    0.0,    // 1
    430,    180,    1.0,    1.0,    // 2
    0,      180,    0.0,    1.0     // 3
};

const float cursor_scale = 0.5;
const float cursor_position[] = {
    25,     43.03,   // 1
    50,     0,       // 2
    25,     14.43,   // 3
    0,      0        // 4
};

const unsigned int indices[] =
{ 0, 1, 2,
    2, 3, 0 };

const unsigned int cursor_indices[] =
{ 3, 2, 0, 3, 2, 1};

#endif
#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "window_exceptions.hpp"
#include "data_def.hpp"
#include <memory>

namespace Window {

class Pimpl;

class Handler {
 public:
  Handler(complex_number start_position = {-2.2, -1.5}, complex_number cursor_start_position = {0.0, 0.0});
  ~Handler();

  compute_mode_t get_selected_compute_mode();

  bool user_has_not_exited();
  void poll_events();
  void swap_buffers();

  void render_imgui();

  complex_number get_left_position();
  complex_number get_right_position();
  complex_number get_cursor_position();
  double get_x_range();
  double get_y_range();

  bool render_requested();

 private:
  Pimpl* pimpl_;

};


}  // namespace Window

#endif
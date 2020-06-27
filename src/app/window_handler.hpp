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
  Handler(complex_number start_position_on_left = {-2.2, -1.5}, complex_number start_position_on_right = {-1.41, -1.53}, complex_number cursor_start_position = {0.0, 0.0});
  ~Handler();

  compute_mode_t get_selected_compute_mode();
  precision_mode_t get_selected_precision_mode();
  int get_iter_number();
  int get_color_preset_number();

  bool user_has_not_exited();
  void poll_events();
  void swap_buffers();

  void render_imgui();

  complex_number get_left_position();
  complex_number get_right_position();
  complex_number get_cursor_position();
  double get_x_range_on_right();
  double get_x_range_on_left();

  bool render_requested();

 private:
  Pimpl* pimpl_;

};


}  // namespace Window

#endif
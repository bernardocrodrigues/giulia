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
  Handler();
  ~Handler();

  compute_mode_t get_selected_compute_mode();

  bool user_has_not_exited();
  void poll_events();
  void swap_buffers();

  void render_imgui();

 private:
  Pimpl* pimpl_;

};


}  // namespace Window

#endif
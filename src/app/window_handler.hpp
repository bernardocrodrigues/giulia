#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "window_exceptions.hpp"

namespace Window {

class Handler {
 public:
  Handler();
  ~Handler();

  inline bool user_has_not_exited() {return !glfwWindowShouldClose(window); };
  inline void poll_events() { glfwPollEvents(); };
  inline void swap_buffers() { glfwSwapBuffers(window);};

  void render_imgui();

 private:
  GLFWwindow* window;
};


}  // namespace Window

#endif
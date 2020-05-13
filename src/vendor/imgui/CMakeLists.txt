# include_directories(${PROJECT_SOURCE_DIR}/src/vendor/imgui)
add_library(ImGUI imgui.cpp imgui_impl_glfw.cpp imgui_impl_opengl3.cpp imgui_widgets.cpp imgui_draw.cpp imgui_demo.cpp)

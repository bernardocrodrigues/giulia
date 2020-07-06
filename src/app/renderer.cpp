
#include "renderer.hpp"
#include "log.hpp"

// #define CL_HPP_ENABLE_EXCEPTIONS
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "clutils.hpp"
#include "glutils.hpp"
// #include <CL/cl2.hpp>


namespace Renderer{

class Pimpl{
    public:
        Pimpl(cl_context_properties context,  cl_context_properties display);
        ~Pimpl();
        void Draw(const VertexArray* va, const IndexBuffer* ib, const Shader* shader) const;
        void DrawGL(window_region_t region, compute_target_t target, complex_number position, complex_number c, int iter, precision_mode_t precision, double range_x, int color_preset, int exponent) const;
        void DrawCL(window_region_t region, compute_target_t target, complex_number position, complex_number c, int iter, precision_mode_t precision, double range_x, int color_preset, int exponent) const;
        void DrawSide(bool ) const;
        void Clear() const;
        mouse_coodenate ComplexNumber2Coodnate(complex_number origin, complex_number position, double range_x, double range_y) const;

        VertexBufferLayout layout;
        VertexBufferLayout logo_layout;

        VertexArray     *logo_va;
        VertexBuffer    *logo_vb;
        IndexBuffer     *logo_ib;

        VertexArray     *left_half_va;
        VertexBuffer    *left_half_vb;
        IndexBuffer     *left_half_ib;

        VertexArray     *right_half_va;
        VertexBuffer    *right_half_vb;
        IndexBuffer     *right_half_ib;

        VertexArray     *full_screen_va;
        VertexBuffer    *full_screen_vb;
        IndexBuffer     *full_screen_ib;

        VertexArray     *cursor_va;
        VertexBuffer    *cursor_vb;
        IndexBuffer     *cursor_ib;

        Shader          *texture_shader;
        Shader          *single_precision_shader;
        Shader          *double_precision_shader;
        Shader          *cursor_shader;

        Texture         *opengl_logo;
        Texture         *opencl_logo;

        cl::Program     *opencl_program;
        cl::Kernel      *opencl_kernel;
        cl::Context     opencl_context;
        cl::Device      opencl_device;

        cl::CommandQueue *opencl_queue;
        Texture         *opengl_texture;
        cl::ImageGL     *opencl_texture;

};

Pimpl::Pimpl(cl_context_properties context, cl_context_properties display) {
  layout.Push<float>(2);
  logo_layout.Push<float>(2);
  logo_layout.Push<float>(2);

  logo_va = new VertexArray();
  logo_vb = new VertexBuffer(logo_positions, 16 * sizeof(float));
  logo_va->AddBuffer(*logo_vb, logo_layout);
  logo_ib = new IndexBuffer(indices, 6);

  left_half_va = new VertexArray();
  left_half_vb = new VertexBuffer(left_half_positions, 8 * sizeof(float));
  left_half_va->AddBuffer(*left_half_vb, layout);
  left_half_ib = new IndexBuffer(indices, 6);

  right_half_va = new VertexArray();
  right_half_vb = new VertexBuffer(right_half_positions, 8 * sizeof(float));
  right_half_va->AddBuffer(*right_half_vb, layout);
  right_half_ib = new IndexBuffer(indices, 6);

  full_screen_va = new VertexArray();
  full_screen_vb = new VertexBuffer(full_screen_positions, 8 * sizeof(float));
  full_screen_va->AddBuffer(*full_screen_vb, layout);
  full_screen_ib = new IndexBuffer(indices, 6);

  cursor_va = new VertexArray();
  cursor_vb = new VertexBuffer(cursor_position, 8 * sizeof(float));
  cursor_va->AddBuffer(*cursor_vb, layout);
  cursor_ib = new IndexBuffer(cursor_indices, 6);

  texture_shader = new Shader("src/gl/shaders/texture.shader");
  single_precision_shader = new Shader(
                     "src/gl/shaders/single_precision.shader");
  double_precision_shader = new Shader(
                     "src/gl/shaders/double_precision.shader");
  cursor_shader = new Shader("src/gl/shaders/pointer.shader");

  opengl_logo = new Texture("res/opengl_logo.png");
  opencl_logo = new Texture("res/opencl_logo.png");

  // int err;

  int err = create_program("src/cl/kernels/fractal.cl", context, display, &opencl_program);
  // auto a = create_program("src/cl/kernels/fractal.cl", context, display);

  switch (err) {
    case -1000:
      LOG_ERROR(
          "OpenCL and OpenGL Devices are not the same! This can happen on "
          "computers with more than one GPU. Please enforce that the "
          "application is running completly in the ONE DEVICE!");
      exit(-1);
    case 0:
    break;
    default:
      LOG_ERROR("Unable to create OpenCL Program. Error: " << err);
      exit(-1);
  }


  try {
  //   // opencl_context = (cl::Context*) malloc(sizeof(cl::Context));
    opencl_context = opencl_program->getInfo<CL_PROGRAM_CONTEXT>();
    opencl_device = opencl_program->getInfo<CL_PROGRAM_DEVICES>().front();
    opencl_kernel = new cl::Kernel(*opencl_program, "Fractal", &err);
  } catch (std::exception& e) {
    LOG_ERROR("Unable to create OpenCL Kernel. Error: " << err);
    exit(0);
  }

  if (err) {
    LOG_ERROR("Unable to create OpenCL Kernel. Error: " << err);
    exit(0);
  }

  auto vendor = opencl_device.getInfo<CL_DEVICE_VENDOR>();
  auto version = opencl_device.getInfo<CL_DEVICE_VERSION>();
  auto name = opencl_device.getInfo<CL_DEVICE_NAME>();
  auto profile = opencl_device.getInfo<CL_DEVICE_PROFILE>();

  LOG_INFO("OpenCL Info \n       Vendor: " << vendor <<
          "\n       Renderer: " << name <<
          "\n       OpenGL Version: " << version <<
          "\n       Profile: " << profile);

  opencl_queue = new cl::CommandQueue(opencl_context, opencl_device, 0,  &err);
  // opencl_queue = new cl::CommandQueue(a.getInfo<CL_PROGRAM_CONTEXT>(), a.getInfo<CL_PROGRAM_DEVICES>().front(), 0,  &err);

  if (err) {
    LOG_ERROR("Unable to create OpenCL Command Queue. Error: " << err);
    exit(0);
  }

  opengl_texture = new Texture(100, 100, 10);
  opencl_texture = new cl::ImageGL(opencl_context, CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, opengl_texture->GetRenderId() ,&err);

  if (err) {
    LOG_ERROR("Unable to create OpenCL Texture. Error: " << err);
    exit(0);
  }


  print_opengl_info();
}

Pimpl::~Pimpl() { }

void Pimpl::Draw(const VertexArray* va, const IndexBuffer* ib, const Shader* shader) const {
    shader->Bind();
    va->Bind();
    ib->Bind();
    GlCall(glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr));
}

void Pimpl::Clear() const {
    GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

Handler::Handler(cl_context_properties context,  cl_context_properties display) : pimpl_(new Pimpl(context, display)) {
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

Handler::~Handler(){ }

void Handler::Clear() const {
    pimpl_->Clear();
}

void Handler::DrawLogo(compute_mode_t& mode) {

    glm::mat4 proj = glm::ortho(0.0, (double) WIDTH/logo_scale,
                                0.0, (double) HEIGHT/logo_scale,
                                0.0, 2.0);

    glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(10,10,0));
    glm::mat4 mvp = proj * view;

    if (mode == compute_mode_t::OPENGL_MODE){
        pimpl_->opengl_logo->Bind();
    } else {
        pimpl_->opencl_logo->Bind();
    }

    pimpl_->texture_shader->Bind();
    pimpl_->texture_shader->SetUniform1i("u_texture", 0);
    pimpl_->texture_shader->SetUniformMat4f("u_MVP", mvp);
    GlCall(glEnable(GL_BLEND));
    pimpl_->Draw(pimpl_->logo_va, pimpl_->logo_ib, pimpl_->texture_shader);
    GlCall(glDisable(GL_BLEND));
}

void Handler::Draw(window_region_t region, compute_mode_t& mode, compute_target_t target, complex_number position, complex_number c, int iter, precision_mode_t precision, double range_x, int color_preset, int exponent) {

  if (mode == compute_mode_t::OPENGL_MODE) {
    pimpl_->DrawGL(region, target, position, c, iter, precision, range_x, color_preset, exponent);
  } else {
    pimpl_->DrawCL(region, target, position, c, iter, precision, range_x, color_preset, exponent);
  }

}

mouse_coodenate Pimpl::ComplexNumber2Coodnate(complex_number origin, complex_number position, double range_x, double range_y) const {
    complex_number delta = {position.real - origin.real, position.imaginary - origin.imaginary};
    return {(delta.real * WIDTH/2)/range_x, (delta.imaginary * HEIGHT)/range_y};
}

void Handler::DrawCursor(complex_number origin, complex_number position, double range) {

    glm::mat4 proj = glm::ortho(0.0, (double) WIDTH/cursor_scale,
                                0.0, (double) HEIGHT/cursor_scale,
                                0.0, 2.0);

    double aspect_ratio = 1;

    mouse_coodenate offset = pimpl_->ComplexNumber2Coodnate(origin, position, range, range * aspect_ratio);

    if( offset.x <= WIDTH/2) {
        glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(offset.x/cursor_scale, offset.y/cursor_scale ,0));
        glm::mat4 mvp = proj * view;

        pimpl_->cursor_shader->Bind();
        pimpl_->cursor_shader->SetUniformMat4f("u_MVP", mvp);
        pimpl_->Draw(pimpl_->cursor_va, pimpl_->cursor_ib, pimpl_->cursor_shader);
    }
}

void Pimpl::DrawGL(window_region_t region, compute_target_t target,
              complex_number position, complex_number c, int iter,
              precision_mode_t precision, double range_x, int color_preset,
              int exponent) const {
  double aspect_ratio = 1;
  double range_y = range_x * aspect_ratio;

  Shader *shader;

  if (precision == precision_mode_t::SINGLE_MODE) {
    shader = single_precision_shader;
  } else {
    shader = double_precision_shader;
  }

  glm::mat4 proj_screen =
      glm::ortho(0.0, (double)WIDTH, 0.0, (double)HEIGHT, 0.0, 2.0);

  shader->Bind();
  shader->SetUniform2d("down_left", position.real, position.imaginary);
  shader->SetUniform1f("range_x", (float)range_x);
  shader->SetUniform1f("range_y", (float)range_y);
  shader->SetUniform2ui("render_resolution", WIDTH / 2, HEIGHT);
  shader->SetUniformMat4f("u_MVP", proj_screen);
  shader->SetUniform1ui("iter", iter);
  shader->SetUniform1ui("color_preset", color_preset);
  shader->SetUniform1ui("exponent", exponent);

  if (target == compute_target_t::MANDELBROT) {
    shader->SetUniform1ui("mode_", 0);
  } else {
    shader->SetUniform1ui("mode_", 1);
    shader->SetUniform2d("c", c.real, c.imaginary);
  }

  switch (region) {
    case window_region_t::LEFT:
      shader->SetUniform2ui("render_offset", 0, 0);
      Draw(left_half_va, left_half_ib, shader);
      break;
    case window_region_t::RIGHT:
      shader->SetUniform2ui("render_offset", 1024, 0);
      Draw(right_half_va, right_half_ib, shader);
      break;
    case window_region_t::FULL:
      shader->SetUniform2ui("render_offset", 0, 0);
      Draw(full_screen_va, full_screen_ib, shader);
      break;
    default:
      break;
  }
}

void Pimpl::DrawCL(window_region_t region, compute_target_t target,
                   complex_number position, complex_number c, int iter,
                   precision_mode_t precision, double range_x, int color_preset,
                   int exponent) const {

  try {
    glFinish();
    cl::Event ev;
    std::vector<cl::Memory> objs;
    objs.clear();
    objs.push_back(*opencl_texture);
    cl_int res = opencl_queue->enqueueAcquireGLObjects(&objs, NULL, &ev);
    ev.wait();
    if (res != CL_SUCCESS) {
      std::cout << "Failed acquiring GL object: " << res << std::endl;
      exit(-1);
    }

    // cl::NDRange local(10, 10);
    // cl::NDRange global(100, 100);

    // kernel.setArg(0, tex);
    // queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local);

    // queue.enqueueReleaseGLObjects(&objs);
    // ev.wait();
    // if (res != CL_SUCCESS) {
    //   std::cout << "Failed releasing GL object: " << res << std::endl;
    //   exit(247);
    // }
    // queue.finish();
  } catch (cl::Error err) {
        std::cout << err.what() << "(" << err.err() << ")" << std::endl;
  }
  // LOG_INFO("A");
}
}

#include "renderer.hpp"
#include "data_def.hpp"
#include "log.hpp"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace Renderer{

class Pimpl{
    public:
        Pimpl();
        ~Pimpl();
        void Draw(const VertexArray* va, const IndexBuffer* ib, const Shader* shader) const;
        void DrawSide(bool ) const;
        void Clear() const;
        mouse_coodenate ComplexNumber2Coodnate(complex_number origin, complex_number position) const;

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

};

Pimpl::Pimpl() {

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
    single_precision_shader = new Shader("src/gl/shaders/single_precision.shader");
    double_precision_shader = new Shader("src/gl/shaders/double_precision.shader");
    cursor_shader = new Shader("src/gl/shaders/pointer.shader");

    opengl_logo = new Texture("res/opengl_logo.png");
    opencl_logo = new Texture("res/opencl_logo.png");

    // glm::mat4 proj_screen = glm::ortho(0.0, (double) WIDTH, 0.0, (double) HEIGHT, 0.0, 2.0);

    // single_precision_shader->Bind();
    // single_precision_shader->SetUniform2ui("screen_resolution", WIDTH/2, HEIGHT);
    // single_precision_shader->SetUniformMat4f("u_MVP", proj_screen);
    // single_precision_shader->SetUniform1ui("mode_", 0);

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

Handler::Handler() : pimpl_(new Pimpl()) {
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

    if (mode == OPENGL_MODE){
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

void Handler::Draw(window_region_t region, compute_mode_t& mode, compute_target_t target, complex_number position, complex_number c, int iter, precision_mode_t precision) {

    double aspect_ratio = 1;
    double range_x = 3;
    double range_y = range_x * aspect_ratio;


    Shader *shader;

    if (precision == SINGLE_MODE) {
        shader = pimpl_->single_precision_shader;
    } else {
        shader = pimpl_->double_precision_shader;
    }

    glm::mat4 proj_screen = glm::ortho(0.0, (double) WIDTH, 0.0, (double) HEIGHT, 0.0, 2.0);

    shader->Bind();
    shader->SetUniform2d("down_left", position.real, position.imaginary);
    shader->SetUniform1f("range_x", (float) range_x);
    shader->SetUniform1f("range_y", (float) range_y);
    shader->SetUniform2ui("render_resolution", WIDTH/2, HEIGHT);
    shader->SetUniformMat4f("u_MVP", proj_screen);
    shader->SetUniform1ui("iter", iter);

    if (target == MANDELBROT){
        shader->SetUniform1ui("mode_", 0);
    } else {
        shader->SetUniform1ui("mode_", 1);
        shader->SetUniform2d("c", c.real, c.imaginary);
    }

    switch (region) {
      case LEFT:
        shader->SetUniform2ui("render_offset", 0, 0);
        pimpl_->Draw(pimpl_->left_half_va, pimpl_->left_half_ib,
                     shader);
        break;
      case RIGHT:
        shader->SetUniform2ui("render_offset",
                                                       WIDTH / 2, 0);
        pimpl_->Draw(pimpl_->right_half_va, pimpl_->right_half_ib,
                     shader);
        break;
      case FULL:
        shader->SetUniform2ui("render_offset", 0, 0);
        pimpl_->Draw(pimpl_->full_screen_va, pimpl_->full_screen_ib,
                     shader);
        break;
      default:
        break;
    }
}

mouse_coodenate Pimpl::ComplexNumber2Coodnate(complex_number origin, complex_number position) const {
    complex_number delta = {position.real - origin.real, position.imaginary - origin.imaginary};
    return {(delta.real * WIDTH/2)/3.0, (delta.imaginary * HEIGHT)/3.0,};
}

void Handler::DrawCursor(complex_number origin, complex_number position) {

    glm::mat4 proj = glm::ortho(0.0, (double) WIDTH/cursor_scale,
                                0.0, (double) HEIGHT/cursor_scale,
                                0.0, 2.0);


    mouse_coodenate offset = pimpl_->ComplexNumber2Coodnate(origin, position);

    if( offset.x <= WIDTH/2) {
        glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(offset.x/cursor_scale, offset.y/cursor_scale ,0));
        glm::mat4 mvp = proj * view;

        pimpl_->cursor_shader->Bind();
        pimpl_->cursor_shader->SetUniformMat4f("u_MVP", mvp);
        pimpl_->Draw(pimpl_->cursor_va, pimpl_->cursor_ib, pimpl_->cursor_shader);
    }
}

}
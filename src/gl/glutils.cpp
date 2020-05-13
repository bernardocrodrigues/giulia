#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glutils.hpp"
#include "stb_image.h"

void GLClearError(){
    while (glGetError());
}

bool GLCheckError(const char* func, const char* file, int line){
    while (GLenum error = glGetError())
    {
        std::cout << "[ERROR 0x" << std::hex << error << "] " << func << ":" << file << ":" << std::dec << line <<std::endl;
        return false;
    }
    return true;
}

VertexBuffer::VertexBuffer(const void* data, const uint size){
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer(){
    GlCall(glDeleteBuffers(1, &m_renderer_id));
}

void VertexBuffer::Bind() const{
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
}

void VertexBuffer::Unbind() const {
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

IndexBuffer::IndexBuffer(const uint* data, uint count):
    m_count(count)
{
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer(){
    GlCall(glDeleteBuffers(1, &m_renderer_id));
}

void IndexBuffer::Bind() const {
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
}

void IndexBuffer::Unbind() const {
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

template<>
void VertexBufferLayout::Push<float> (uint count)
{
    m_elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
void VertexBufferLayout::Push<uint> (uint count){
    m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;

}

template<>
void VertexBufferLayout::Push<u_char>(uint count){
    m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}

VertexArray::VertexArray() {
    GlCall(glGenVertexArrays(1, &m_renderer_id));
}

VertexArray::~VertexArray() {
    GlCall(glDeleteVertexArrays(1, &m_renderer_id));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, VertexBufferLayout& layout) {
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    intptr_t offset = 0;
    for(uint i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        GlCall(glEnableVertexAttribArray(i));
        GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    GlCall(glBindVertexArray(m_renderer_id));
}

void VertexArray::Unbind() const {
    GlCall(glBindVertexArray(0));
}

Shader::Shader(const std::string& filepath)
    : m_renderer_id(0), m_filepath(filepath) {
    ShaderSource source = ParseShader(filepath);
    m_renderer_id = CreateShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader() {
    GlCall(glDeleteProgram(m_renderer_id));
}

uint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    uint program = glCreateProgram();
    uint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    uint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));

    GlCall(glLinkProgram(program));
    GlCall(glValidateProgram(program));

    GlCall(glDeleteShader(vs));
    GlCall(glDeleteShader(fs));

    return program;
}

ShaderSource Shader::ParseShader(const std::string& filepath) {
    enum class ShaderType{
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }else{
            ss[(int)type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

uint Shader::CompileShader(uint type, const std::string& source) {
    uint id = glCreateShader(type);
    const char* src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, nullptr));
    GlCall(glCompileShader(id));

    int result;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result){
        int length;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char));
        GlCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " Shader!" << std::endl;
        std::cout << message << std::endl;
        GlCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

void Shader::Bind() const {
    GlCall(glUseProgram(m_renderer_id));
}

void Shader::Unbind() const {
    GlCall(glUseProgram(0));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix) {
    GlCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GlCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1) {
    GlCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform1f(const std::string& name, float v0) {
    GlCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniform2d(const std::string& name, double v0, double v1) {
    GlCall(glUniform2d(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform1d(const std::string& name, double v0) {
    GlCall(glUniform1d(GetUniformLocation(name), v0));
}

void Shader::SetUniform2ui(const std::string& name, unsigned int v0, unsigned int v1) {
    GlCall(glUniform2ui(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform1ui(const std::string& name, unsigned int v0) {
    GlCall(glUniform1ui(GetUniformLocation(name), v0));
}

void Shader::SetUniform1i(const std::string& name, int v0) {
    GlCall(glUniform1i(GetUniformLocation(name), v0));
}

int Shader::GetUniformLocation(const std::string& name ) {

    if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end()){
        return m_uniform_location_cache[name];
    }

    GlCall(int location = glGetUniformLocation(m_renderer_id, name.c_str()));
    if (location == -1)
        std::cout << "Warning! Uniform " << name << " doesn't exist!" << std::endl;

    m_uniform_location_cache[name] = location;
    return location;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

    shader.Bind();
    va.Bind();
    ib.Bind();
    GlCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const {
    GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

Texture::Texture(const std::string& path)
    : m_file_path(path), m_buffer(nullptr),
      m_width(0), m_height(0), m_bpp(0) {

    stbi_set_flip_vertically_on_load(1);
    m_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

    GlCall(glGenTextures(1, &m_renderer));
    GlCall(glBindTexture(GL_TEXTURE_2D, m_renderer));

    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer));
    GlCall(glBindTexture(GL_TEXTURE_2D, 0));

    if(m_buffer)
        stbi_image_free(m_buffer);

}

Texture::~Texture() {
    GlCall(glDeleteTextures(1, &m_renderer));
}

void Texture::Bind(unsigned int slot) const {
    GlCall(glActiveTexture(GL_TEXTURE0 + slot));
    GlCall(glBindTexture(GL_TEXTURE_2D, m_renderer));
}

void Texture::Unbind() const {
    GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}

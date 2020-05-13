
#include <signal.h>
#include <vector>
#include <GL/glew.h>
#include <unordered_map>
#include "glm.hpp"


#define ASSERT(x) if(!(x)){raise(SIGTRAP);}

#define GlCall(x) GLClearError();\
    x;\
    ASSERT(GLCheckError(#x, __BASE_FILE__, __LINE__))


void GLClearError();
bool GLCheckError(const char* func, const char* file, int line);

class VertexBuffer{
    private:
        uint m_renderer_id;
    public:
        VertexBuffer(const void* data, const uint size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
};

class IndexBuffer{
    private:
        uint m_renderer_id;
        uint m_count;
    public:
        IndexBuffer(const uint* data, uint count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline uint getCount() const { return m_count; }
};

struct VertexBufferElement{
    uint type;
    uint count;
    uint normalized;

    static uint GetSizeOfType( uint type ) {
        switch (type)
        {
            case GL_FLOAT:             return 4;
            case GL_UNSIGNED_INT:      return 4;
            case GL_UNSIGNED_BYTE:     return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout{
    private:
        std::vector<VertexBufferElement> m_elements;
        uint m_stride;
    public:
        VertexBufferLayout()
            : m_stride(0) {}

        template<typename T>
        void Push(uint count)
        {
            static_assert(true);
        }

        inline const std::vector<VertexBufferElement> GetElements() const { return m_elements; }
        inline const int GetStride() const { return m_stride; }
};

class VertexArray{
    private:
        uint m_renderer_id;
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, VertexBufferLayout& layout);

        void Bind() const;
        void Unbind() const;
};

struct ShaderSource{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader{
    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniform2f(const std::string& name, float v0, float v1);
        void SetUniform1f(const std::string& name, float v0);

        void SetUniform2d(const std::string& name, double v0, double v1);
        void SetUniform1d(const std::string& name, double v0);

        void SetUniform2ui(const std::string& name, unsigned int v0, unsigned int v1);
        void SetUniform1ui(const std::string& name, unsigned int v0);

        void SetUniform1i(const std::string& name, int v0);

    private:
        uint m_renderer_id;
        std::string m_filepath;
        std::unordered_map<std::string, int> m_uniform_location_cache;

        ShaderSource ParseShader(const std::string& filepath);
        uint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        uint CompileShader(uint type, const std::string& source);
        int GetUniformLocation(const std::string& name );

};

class Renderer{
    public:
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void Clear() const;
};

class Texture{
    private:
        unsigned int m_renderer;
        std::string m_file_path;
        unsigned char* m_buffer;
        int m_width, m_height, m_bpp;
    public:
        Texture(const std::string& path);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        inline int GetWidth() const { return m_width; }
        inline int GetHeight() const { return m_height; }
};
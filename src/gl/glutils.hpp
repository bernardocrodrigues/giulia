
#include <signal.h>
#include <vector>
#include <GL/glew.h>


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
    uint count;
    uint type;
    bool normalized;
};

class VertexBufferLayout{
    private:
        std::vector<VertexBufferElement> m_elements;
        uint m_stride;
    public:
        VertexBufferLayout()
            : m_stride(0) {}

        template<typename T>
        void Push(int count)
        {
            static_assert(true);
        }
    
        template<>
        void Push<float>(int count)
        {
            // m_elements.push_back({GL_FLOAT, count, false});
            // m_stride += sizeof(GLfloat);
        }

        // template<>
        // void Push<uint>(int count){
        //     m_elements.push_back({GL_UNSIGNED_INT, count, false});
        // }

        // template<>
        // void Push<>(u_char count){
        //     m_elements.push_back({GL_UNSIGNED_BYTE, count, true});
        // }

};

class VertexArray{
    private:

    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, VertexBufferLayout layout);
};